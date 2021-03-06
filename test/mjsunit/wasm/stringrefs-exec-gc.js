// Copyright 2022 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Flags: --experimental-wasm-stringref --experimental-wasm-gc

d8.file.execute("test/mjsunit/wasm/wasm-module-builder.js");

let kSig_w_v = makeSig([], [kWasmStringRef]);
let kSig_w_ii = makeSig([kWasmI32, kWasmI32], [kWasmStringRef]);

function encodeWtf8(str) {
  // String iterator coalesces surrogate pairs.
  let out = [];
  for (let codepoint of str) {
    codepoint = codepoint.codePointAt(0);
    if (codepoint <= 0x7f) {
      out.push(codepoint);
    } else if (codepoint <= 0x7ff) {
      out.push(0xc0 | (codepoint >> 6));
      out.push(0x80 | (codepoint & 0x3f));
    } else if (codepoint <= 0xffff) {
      out.push(0xe0 | (codepoint >> 12));
      out.push(0x80 | ((codepoint >> 6) & 0x3f));
      out.push(0x80 | (codepoint & 0x3f));
    } else if (codepoint <= 0x10ffff) {
      out.push(0xf0 | (codepoint >> 18));
      out.push(0x80 | ((codepoint >> 12) & 0x3f));
      out.push(0x80 | ((codepoint >> 6) & 0x3f));
      out.push(0x80 | (codepoint & 0x3f));
    } else {
      throw new Error("bad codepoint " + codepoint);
    }
  }
  return out;
}

let interestingStrings = ['',
                          'ascii',
                          'latin \xa9 1',
                          'two \ucccc byte',
                          'surrogate \ud800\udc000 pair',
                          'isolated \ud800 leading',
                          'isolated \udc00 trailing',
                          '\ud800 isolated leading at beginning',
                          '\udc00 isolated trailing at beginning',
                          'isolated leading at end \ud800',
                          'isolated trailing at end \udc00',
                          'swapped surrogate \udc00\ud800 pair'];

function IsSurrogate(codepoint) {
  return 0xD800 <= codepoint && codepoint <= 0xDFFF
}
function HasIsolatedSurrogate(str) {
  for (let codepoint of str) {
    let value = codepoint.codePointAt(0);
    if (IsSurrogate(value)) return true;
  }
  return false;
}
function ReplaceIsolatedSurrogates(str, replacement='\ufffd') {
  let replaced = '';
  for (let codepoint of str) {
    replaced +=
      IsSurrogate(codepoint.codePointAt(0)) ? replacement : codepoint;
  }
  return replaced;
}

function makeWtf8TestDataSegment() {
  let data = []
  let valid = {};
  let invalid = {};

  for (let str of interestingStrings) {
    let bytes = encodeWtf8(str);
    valid[str] = { offset: data.length, length: bytes.length };
    for (let byte of bytes) {
      data.push(byte);
    }
  }
  for (let bytes of ['trailing high byte \xa9',
                     'interstitial high \xa9 byte',
                     'invalid \xc0 byte',
                     'invalid three-byte \xed\xd0\x80',
                     'surrogate \xed\xa0\x80\xed\xb0\x80 pair']) {
    invalid[bytes] = { offset: data.length, length: bytes.length };
    for (let i = 0; i < bytes.length; i++) {
      data.push(bytes.charCodeAt(i));
    }
  }

  return { valid, invalid, data: Uint8Array.from(data) };
};

(function TestStringNewWtf8Array() {
  let builder = new WasmModuleBuilder();

  let data = makeWtf8TestDataSegment();
  let data_index = builder.addPassiveDataSegment(data.data);

  let ascii_data_index =
      builder.addPassiveDataSegment(Uint8Array.from(encodeWtf8("ascii")));

  let i8_array = builder.addArray(kWasmI8, true);

  let make_i8_array = builder.addFunction(
      "make_i8_array", makeSig([], [wasmRefType(i8_array)]))
    .addBody([
      ...wasmI32Const(0),
      ...wasmI32Const(data.data.length),
      kGCPrefix, kExprArrayNewDataStatic, i8_array, data_index
    ]).index;

  for (let [policy, name] of [[kWtf8PolicyAccept, "new_wtf8"],
                              [kWtf8PolicyReject, "new_utf8"],
                              [kWtf8PolicyReplace, "new_utf8_sloppy"]]) {
    builder.addFunction(name, kSig_w_ii)
      .exportFunc()
      .addBody([
        kExprCallFunction, make_i8_array,
        kExprLocalGet, 0, kExprLocalGet, 1,
        kGCPrefix, kExprStringNewWtf8Array, policy
      ]);
  }

  builder.addFunction("bounds_check", kSig_w_ii)
    .exportFunc()
    .addBody([
      ...wasmI32Const(0),
      ...wasmI32Const("ascii".length),
      kGCPrefix, kExprArrayNewDataStatic, i8_array, ascii_data_index,
      kExprLocalGet, 0, kExprLocalGet, 1,
      kGCPrefix, kExprStringNewWtf8Array, kWtf8PolicyAccept
    ]);

  let instance = builder.instantiate();
  for (let [str, {offset, length}] of Object.entries(data.valid)) {
    let start = offset;
    let end = offset + length;
    assertEquals(str, instance.exports.new_wtf8(start, end));
    if (HasIsolatedSurrogate(str)) {
      assertThrows(() => instance.exports.new_utf8(start, end),
                   WebAssembly.RuntimeError, "invalid UTF-8 string");

      // Isolated surrogates have the three-byte pattern ED [A0,BF]
      // [80,BF].  When the sloppy decoder gets to the second byte, it
      // will reject the sequence, and then retry parsing at the second
      // byte.  Seeing the second byte can't start a sequence, it
      // replaces the second byte and continues with the next, which
      // also can't start a sequence.  The result is that one isolated
      // surrogate is replaced by three U+FFFD codepoints.
      assertEquals(ReplaceIsolatedSurrogates(str, '\ufffd\ufffd\ufffd'),
                   instance.exports.new_utf8_sloppy(start, end));
    } else {
      assertEquals(str, instance.exports.new_utf8(start, end));
      assertEquals(str, instance.exports.new_utf8_sloppy(start, end));
    }
  }
  for (let [str, {offset, length}] of Object.entries(data.invalid)) {
    let start = offset;
    let end = offset + length;
    assertThrows(() => instance.exports.new_wtf8(start, end),
                 WebAssembly.RuntimeError, "invalid WTF-8 string");
    assertThrows(() => instance.exports.new_utf8(start, end),
                 WebAssembly.RuntimeError, "invalid UTF-8 string");
  }

  assertEquals("ascii", instance.exports.bounds_check(0, "ascii".length));
  assertEquals("", instance.exports.bounds_check("ascii".length,
                                                 "ascii".length));
  assertEquals("i", instance.exports.bounds_check("ascii".length - 1,
                                                  "ascii".length));
  assertThrows(() => instance.exports.bounds_check(0, 100),
               WebAssembly.RuntimeError, "array element access out of bounds");
  assertThrows(() => instance.exports.bounds_check("ascii".length,
                                                   "ascii".length + 1),
               WebAssembly.RuntimeError, "array element access out of bounds");
})();
