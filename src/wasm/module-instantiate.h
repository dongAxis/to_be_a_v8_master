// Copyright 2019 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if !V8_ENABLE_WEBASSEMBLY
#error This header should only be included if WebAssembly is enabled.
#endif  // !V8_ENABLE_WEBASSEMBLY

#ifndef V8_WASM_MODULE_INSTANTIATE_H_
#define V8_WASM_MODULE_INSTANTIATE_H_

#include <stdint.h>

#include "include/v8config.h"
#include "src/base/optional.h"
#include "src/common/message-template.h"
#include "src/wasm/wasm-value.h"

namespace v8 {
namespace internal {

class Isolate;
class JSArrayBuffer;
class JSReceiver;
class WasmModuleObject;
class WasmInstanceObject;
class Zone;

template <typename T>
class Handle;
template <typename T>
class MaybeHandle;

namespace wasm {
class ConstantExpression;
class ErrorThrower;

MaybeHandle<WasmInstanceObject> InstantiateToInstanceObject(
    Isolate* isolate, ErrorThrower* thrower,
    Handle<WasmModuleObject> module_object, MaybeHandle<JSReceiver> imports,
    MaybeHandle<JSArrayBuffer> memory);

// Loads a range of elements from element segment into a table.
// Returns the empty {Optional} if the operation succeeds, or an {Optional} with
// the error {MessageTemplate} if it fails.
base::Optional<MessageTemplate> LoadElemSegment(
    Isolate* isolate, Handle<WasmInstanceObject> instance, uint32_t table_index,
    uint32_t segment_index, uint32_t dst, uint32_t src,
    uint32_t count) V8_WARN_UNUSED_RESULT;

}  // namespace wasm
}  // namespace internal
}  // namespace v8

#endif  // V8_WASM_MODULE_INSTANTIATE_H_
