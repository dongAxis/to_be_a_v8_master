// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_EXECUTION_PPC_FRAME_CONSTANTS_PPC_H_
#define V8_EXECUTION_PPC_FRAME_CONSTANTS_PPC_H_

#include "src/base/bits.h"
#include "src/base/macros.h"
#include "src/codegen/register.h"
#include "src/execution/frame-constants.h"

namespace v8 {
namespace internal {

class EntryFrameConstants : public AllStatic {
 public:
  // Need to take constant pool into account.
  static constexpr int kCallerFPOffset =
      FLAG_enable_embedded_constant_pool.value() ? -4 * kSystemPointerSize
                                                 : -3 * kSystemPointerSize;
};

class WasmCompileLazyFrameConstants : public TypedFrameConstants {
 public:
  static constexpr int kNumberOfSavedGpParamRegs = 7;
  static constexpr int kNumberOfSavedFpParamRegs = 8;

  // FP-relative.
  // The instance is pushed as part of the saved registers. Being in {r10}, it
  // is the first register pushed (highest register code in
  // {wasm::kGpParamRegisters}).
  static constexpr int kWasmInstanceOffset = TYPED_FRAME_PUSHED_VALUE_OFFSET(0);
  static constexpr int kFixedFrameSizeFromFp =
      TypedFrameConstants::kFixedFrameSizeFromFp +
      kNumberOfSavedGpParamRegs * kSystemPointerSize +
      kNumberOfSavedFpParamRegs * kDoubleSize +
      kNumberOfSavedFpParamRegs * kSimd128Size;
};

// Frame constructed by the {WasmDebugBreak} builtin.
// After pushing the frame type marker, the builtin pushes all Liftoff cache
// registers (see liftoff-assembler-defs.h).
class WasmDebugBreakFrameConstants : public TypedFrameConstants {
 public:
  static constexpr RegList kPushedGpRegs = {r3, r4, r5,  r6,  r7,
                                            r8, r9, r10, r11, cp};

  static constexpr DoubleRegList kPushedFpRegs = {d0, d1, d2, d3,  d4,  d5, d6,
                                                  d7, d8, d9, d10, d11, d12};

  static constexpr Simd128RegList kPushedSimd128Regs = {
      v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12};

  static constexpr int kNumPushedGpRegisters = kPushedGpRegs.Count();
  static constexpr int kNumPushedFpRegisters = kPushedFpRegs.Count();

  static constexpr int kLastPushedGpRegisterOffset =
      -TypedFrameConstants::kFixedFrameSizeFromFp -
      kSystemPointerSize * kNumPushedGpRegisters;
  static constexpr int kLastPushedFpRegisterOffset =
      kLastPushedGpRegisterOffset - kDoubleSize * kNumPushedFpRegisters;

  // Offsets are fp-relative.
  static int GetPushedGpRegisterOffset(int reg_code) {
    DCHECK_NE(0, kPushedGpRegs.bits() & (1 << reg_code));
    uint32_t lower_regs =
        kPushedGpRegs.bits() & ((uint32_t{1} << reg_code) - 1);
    return kLastPushedGpRegisterOffset +
           base::bits::CountPopulation(lower_regs) * kSystemPointerSize;
  }

  static int GetPushedFpRegisterOffset(int reg_code) {
    DCHECK_NE(0, kPushedFpRegs.bits() & (1 << reg_code));
    uint32_t lower_regs =
        kPushedFpRegs.bits() & ((uint32_t{1} << reg_code) - 1);
    return kLastPushedFpRegisterOffset +
           base::bits::CountPopulation(lower_regs) * kSimd128Size;
  }
};

}  // namespace internal
}  // namespace v8

#endif  // V8_EXECUTION_PPC_FRAME_CONSTANTS_PPC_H_
