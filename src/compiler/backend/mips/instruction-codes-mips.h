// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_COMPILER_BACKEND_MIPS_INSTRUCTION_CODES_MIPS_H_
#define V8_COMPILER_BACKEND_MIPS_INSTRUCTION_CODES_MIPS_H_

namespace v8 {
namespace internal {
namespace compiler {

// MIPS-specific opcodes that specify which assembly sequence to emit.
// Most opcodes specify a single instruction.

#define TARGET_ARCH_OPCODE_LIST(V) \
  V(MipsAdd)                       \
  V(MipsAddOvf)                    \
  V(MipsSub)                       \
  V(MipsSubOvf)                    \
  V(MipsMul)                       \
  V(MipsMulOvf)                    \
  V(MipsMulHigh)                   \
  V(MipsMulHighU)                  \
  V(MipsDiv)                       \
  V(MipsDivU)                      \
  V(MipsMod)                       \
  V(MipsModU)                      \
  V(MipsAnd)                       \
  V(MipsOr)                        \
  V(MipsNor)                       \
  V(MipsXor)                       \
  V(MipsClz)                       \
  V(MipsCtz)                       \
  V(MipsPopcnt)                    \
  V(MipsLsa)                       \
  V(MipsShl)                       \
  V(MipsShr)                       \
  V(MipsSar)                       \
  V(MipsShlPair)                   \
  V(MipsShrPair)                   \
  V(MipsSarPair)                   \
  V(MipsExt)                       \
  V(MipsIns)                       \
  V(MipsRor)                       \
  V(MipsMov)                       \
  V(MipsTst)                       \
  V(MipsCmp)                       \
  V(MipsCmpS)                      \
  V(MipsAddS)                      \
  V(MipsSubS)                      \
  V(MipsMulS)                      \
  V(MipsDivS)                      \
  V(MipsAbsS)                      \
  V(MipsSqrtS)                     \
  V(MipsMaxS)                      \
  V(MipsMinS)                      \
  V(MipsCmpD)                      \
  V(MipsAddD)                      \
  V(MipsSubD)                      \
  V(MipsMulD)                      \
  V(MipsDivD)                      \
  V(MipsModD)                      \
  V(MipsAbsD)                      \
  V(MipsSqrtD)                     \
  V(MipsMaxD)                      \
  V(MipsMinD)                      \
  V(MipsNegS)                      \
  V(MipsNegD)                      \
  V(MipsAddPair)                   \
  V(MipsSubPair)                   \
  V(MipsMulPair)                   \
  V(MipsMaddS)                     \
  V(MipsMaddD)                     \
  V(MipsMsubS)                     \
  V(MipsMsubD)                     \
  V(MipsFloat32RoundDown)          \
  V(MipsFloat32RoundTruncate)      \
  V(MipsFloat32RoundUp)            \
  V(MipsFloat32RoundTiesEven)      \
  V(MipsFloat64RoundDown)          \
  V(MipsFloat64RoundTruncate)      \
  V(MipsFloat64RoundUp)            \
  V(MipsFloat64RoundTiesEven)      \
  V(MipsCvtSD)                     \
  V(MipsCvtDS)                     \
  V(MipsTruncWD)                   \
  V(MipsRoundWD)                   \
  V(MipsFloorWD)                   \
  V(MipsCeilWD)                    \
  V(MipsTruncWS)                   \
  V(MipsRoundWS)                   \
  V(MipsFloorWS)                   \
  V(MipsCeilWS)                    \
  V(MipsTruncUwD)                  \
  V(MipsTruncUwS)                  \
  V(MipsCvtDW)                     \
  V(MipsCvtDUw)                    \
  V(MipsCvtSW)                     \
  V(MipsCvtSUw)                    \
  V(MipsLb)                        \
  V(MipsLbu)                       \
  V(MipsSb)                        \
  V(MipsLh)                        \
  V(MipsUlh)                       \
  V(MipsLhu)                       \
  V(MipsUlhu)                      \
  V(MipsSh)                        \
  V(MipsUsh)                       \
  V(MipsLw)                        \
  V(MipsUlw)                       \
  V(MipsSw)                        \
  V(MipsUsw)                       \
  V(MipsLwc1)                      \
  V(MipsUlwc1)                     \
  V(MipsSwc1)                      \
  V(MipsUswc1)                     \
  V(MipsLdc1)                      \
  V(MipsUldc1)                     \
  V(MipsSdc1)                      \
  V(MipsUsdc1)                     \
  V(MipsFloat64ExtractLowWord32)   \
  V(MipsFloat64ExtractHighWord32)  \
  V(MipsFloat64InsertLowWord32)    \
  V(MipsFloat64InsertHighWord32)   \
  V(MipsFloat64SilenceNaN)         \
  V(MipsFloat32Max)                \
  V(MipsFloat64Max)                \
  V(MipsFloat32Min)                \
  V(MipsFloat64Min)                \
  V(MipsPush)                      \
  V(MipsPeek)                      \
  V(MipsStoreToStackSlot)          \
  V(MipsByteSwap32)                \
  V(MipsStackClaim)                \
  V(MipsSeb)                       \
  V(MipsSeh)                       \
  V(MipsSync)                      \
  V(MipsS128Zero)                  \
  V(MipsI32x4Splat)                \
  V(MipsI32x4ExtractLane)          \
  V(MipsI32x4ReplaceLane)          \
  V(MipsI32x4Add)                  \
  V(MipsI32x4Sub)                  \
  V(MipsF64x2Abs)                  \
  V(MipsF64x2Neg)                  \
  V(MipsF64x2Sqrt)                 \
  V(MipsF64x2Add)                  \
  V(MipsF64x2Sub)                  \
  V(MipsF64x2Mul)                  \
  V(MipsF64x2Div)                  \
  V(MipsF64x2Min)                  \
  V(MipsF64x2Max)                  \
  V(MipsF64x2Eq)                   \
  V(MipsF64x2Ne)                   \
  V(MipsF64x2Lt)                   \
  V(MipsF64x2Le)                   \
  V(MipsF64x2Pmin)                 \
  V(MipsF64x2Pmax)                 \
  V(MipsF64x2Ceil)                 \
  V(MipsF64x2Floor)                \
  V(MipsF64x2Trunc)                \
  V(MipsF64x2NearestInt)           \
  V(MipsF64x2ConvertLowI32x4S)     \
  V(MipsF64x2ConvertLowI32x4U)     \
  V(MipsF64x2PromoteLowF32x4)      \
  V(MipsI64x2Add)                  \
  V(MipsI64x2Sub)                  \
  V(MipsI64x2Mul)                  \
  V(MipsI64x2Neg)                  \
  V(MipsI64x2Shl)                  \
  V(MipsI64x2ShrS)                 \
  V(MipsI64x2ShrU)                 \
  V(MipsI64x2BitMask)              \
  V(MipsI64x2Eq)                   \
  V(MipsI64x2Ne)                   \
  V(MipsI64x2GtS)                  \
  V(MipsI64x2GeS)                  \
  V(MipsI64x2Abs)                  \
  V(MipsI64x2SConvertI32x4Low)     \
  V(MipsI64x2SConvertI32x4High)    \
  V(MipsI64x2UConvertI32x4Low)     \
  V(MipsI64x2UConvertI32x4High)    \
  V(MipsI64x2ExtMulLowI32x4S)      \
  V(MipsI64x2ExtMulHighI32x4S)     \
  V(MipsI64x2ExtMulLowI32x4U)      \
  V(MipsI64x2ExtMulHighI32x4U)     \
  V(MipsF32x4Splat)                \
  V(MipsF32x4ExtractLane)          \
  V(MipsF32x4ReplaceLane)          \
  V(MipsF32x4SConvertI32x4)        \
  V(MipsF32x4UConvertI32x4)        \
  V(MipsF32x4DemoteF64x2Zero)      \
  V(MipsI32x4Mul)                  \
  V(MipsI32x4MaxS)                 \
  V(MipsI32x4MinS)                 \
  V(MipsI32x4Eq)                   \
  V(MipsI32x4Ne)                   \
  V(MipsI32x4Shl)                  \
  V(MipsI32x4ShrS)                 \
  V(MipsI32x4ShrU)                 \
  V(MipsI32x4MaxU)                 \
  V(MipsI32x4MinU)                 \
  V(MipsF64x2Splat)                \
  V(MipsF64x2ExtractLane)          \
  V(MipsF64x2ReplaceLane)          \
  V(MipsF32x4Abs)                  \
  V(MipsF32x4Neg)                  \
  V(MipsF32x4Sqrt)                 \
  V(MipsF32x4Add)                  \
  V(MipsF32x4Sub)                  \
  V(MipsF32x4Mul)                  \
  V(MipsF32x4Div)                  \
  V(MipsF32x4Max)                  \
  V(MipsF32x4Min)                  \
  V(MipsF32x4Eq)                   \
  V(MipsF32x4Ne)                   \
  V(MipsF32x4Lt)                   \
  V(MipsF32x4Le)                   \
  V(MipsF32x4Pmin)                 \
  V(MipsF32x4Pmax)                 \
  V(MipsF32x4Ceil)                 \
  V(MipsF32x4Floor)                \
  V(MipsF32x4Trunc)                \
  V(MipsF32x4NearestInt)           \
  V(MipsI32x4SConvertF32x4)        \
  V(MipsI32x4UConvertF32x4)        \
  V(MipsI32x4Neg)                  \
  V(MipsI32x4GtS)                  \
  V(MipsI32x4GeS)                  \
  V(MipsI32x4GtU)                  \
  V(MipsI32x4GeU)                  \
  V(MipsI32x4Abs)                  \
  V(MipsI32x4BitMask)              \
  V(MipsI32x4DotI16x8S)            \
  V(MipsI32x4ExtMulLowI16x8S)      \
  V(MipsI32x4ExtMulHighI16x8S)     \
  V(MipsI32x4ExtMulLowI16x8U)      \
  V(MipsI32x4ExtMulHighI16x8U)     \
  V(MipsI32x4TruncSatF64x2SZero)   \
  V(MipsI32x4TruncSatF64x2UZero)   \
  V(MipsI32x4ExtAddPairwiseI16x8S) \
  V(MipsI32x4ExtAddPairwiseI16x8U) \
  V(MipsI16x8Splat)                \
  V(MipsI16x8ExtractLaneU)         \
  V(MipsI16x8ExtractLaneS)         \
  V(MipsI16x8ReplaceLane)          \
  V(MipsI16x8Neg)                  \
  V(MipsI16x8Shl)                  \
  V(MipsI16x8ShrS)                 \
  V(MipsI16x8ShrU)                 \
  V(MipsI16x8Add)                  \
  V(MipsI16x8AddSatS)              \
  V(MipsI16x8Sub)                  \
  V(MipsI16x8SubSatS)              \
  V(MipsI16x8Mul)                  \
  V(MipsI16x8MaxS)                 \
  V(MipsI16x8MinS)                 \
  V(MipsI16x8Eq)                   \
  V(MipsI16x8Ne)                   \
  V(MipsI16x8GtS)                  \
  V(MipsI16x8GeS)                  \
  V(MipsI16x8AddSatU)              \
  V(MipsI16x8SubSatU)              \
  V(MipsI16x8MaxU)                 \
  V(MipsI16x8MinU)                 \
  V(MipsI16x8GtU)                  \
  V(MipsI16x8GeU)                  \
  V(MipsI16x8RoundingAverageU)     \
  V(MipsI16x8Abs)                  \
  V(MipsI16x8BitMask)              \
  V(MipsI16x8Q15MulRSatS)          \
  V(MipsI16x8ExtMulLowI8x16S)      \
  V(MipsI16x8ExtMulHighI8x16S)     \
  V(MipsI16x8ExtMulLowI8x16U)      \
  V(MipsI16x8ExtMulHighI8x16U)     \
  V(MipsI16x8ExtAddPairwiseI8x16S) \
  V(MipsI16x8ExtAddPairwiseI8x16U) \
  V(MipsI8x16Splat)                \
  V(MipsI8x16ExtractLaneU)         \
  V(MipsI8x16ExtractLaneS)         \
  V(MipsI8x16ReplaceLane)          \
  V(MipsI8x16Neg)                  \
  V(MipsI8x16Shl)                  \
  V(MipsI8x16ShrS)                 \
  V(MipsI8x16Add)                  \
  V(MipsI8x16AddSatS)              \
  V(MipsI8x16Sub)                  \
  V(MipsI8x16SubSatS)              \
  V(MipsI8x16MaxS)                 \
  V(MipsI8x16MinS)                 \
  V(MipsI8x16Eq)                   \
  V(MipsI8x16Ne)                   \
  V(MipsI8x16GtS)                  \
  V(MipsI8x16GeS)                  \
  V(MipsI8x16ShrU)                 \
  V(MipsI8x16AddSatU)              \
  V(MipsI8x16SubSatU)              \
  V(MipsI8x16MaxU)                 \
  V(MipsI8x16MinU)                 \
  V(MipsI8x16GtU)                  \
  V(MipsI8x16GeU)                  \
  V(MipsI8x16RoundingAverageU)     \
  V(MipsI8x16Abs)                  \
  V(MipsI8x16Popcnt)               \
  V(MipsI8x16BitMask)              \
  V(MipsS128And)                   \
  V(MipsS128Or)                    \
  V(MipsS128Xor)                   \
  V(MipsS128Not)                   \
  V(MipsS128Select)                \
  V(MipsS128AndNot)                \
  V(MipsI64x2AllTrue)              \
  V(MipsI32x4AllTrue)              \
  V(MipsI16x8AllTrue)              \
  V(MipsI8x16AllTrue)              \
  V(MipsV128AnyTrue)               \
  V(MipsS32x4InterleaveRight)      \
  V(MipsS32x4InterleaveLeft)       \
  V(MipsS32x4PackEven)             \
  V(MipsS32x4PackOdd)              \
  V(MipsS32x4InterleaveEven)       \
  V(MipsS32x4InterleaveOdd)        \
  V(MipsS32x4Shuffle)              \
  V(MipsS16x8InterleaveRight)      \
  V(MipsS16x8InterleaveLeft)       \
  V(MipsS16x8PackEven)             \
  V(MipsS16x8PackOdd)              \
  V(MipsS16x8InterleaveEven)       \
  V(MipsS16x8InterleaveOdd)        \
  V(MipsS16x4Reverse)              \
  V(MipsS16x2Reverse)              \
  V(MipsS8x16InterleaveRight)      \
  V(MipsS8x16InterleaveLeft)       \
  V(MipsS8x16PackEven)             \
  V(MipsS8x16PackOdd)              \
  V(MipsS8x16InterleaveEven)       \
  V(MipsS8x16InterleaveOdd)        \
  V(MipsI8x16Shuffle)              \
  V(MipsI8x16Swizzle)              \
  V(MipsS8x16Concat)               \
  V(MipsS8x8Reverse)               \
  V(MipsS8x4Reverse)               \
  V(MipsS8x2Reverse)               \
  V(MipsS128Load8Splat)            \
  V(MipsS128Load16Splat)           \
  V(MipsS128Load32Splat)           \
  V(MipsS128Load64Splat)           \
  V(MipsS128Load8x8S)              \
  V(MipsS128Load8x8U)              \
  V(MipsS128Load16x4S)             \
  V(MipsS128Load16x4U)             \
  V(MipsS128Load32x2S)             \
  V(MipsS128Load32x2U)             \
  V(MipsMsaLd)                     \
  V(MipsMsaSt)                     \
  V(MipsI32x4SConvertI16x8Low)     \
  V(MipsI32x4SConvertI16x8High)    \
  V(MipsI32x4UConvertI16x8Low)     \
  V(MipsI32x4UConvertI16x8High)    \
  V(MipsI16x8SConvertI8x16Low)     \
  V(MipsI16x8SConvertI8x16High)    \
  V(MipsI16x8SConvertI32x4)        \
  V(MipsI16x8UConvertI32x4)        \
  V(MipsI16x8UConvertI8x16Low)     \
  V(MipsI16x8UConvertI8x16High)    \
  V(MipsI8x16SConvertI16x8)        \
  V(MipsI8x16UConvertI16x8)        \
  V(MipsWord32AtomicPairLoad)      \
  V(MipsWord32AtomicPairStore)     \
  V(MipsWord32AtomicPairAdd)       \
  V(MipsWord32AtomicPairSub)       \
  V(MipsWord32AtomicPairAnd)       \
  V(MipsWord32AtomicPairOr)        \
  V(MipsWord32AtomicPairXor)       \
  V(MipsWord32AtomicPairExchange)  \
  V(MipsWord32AtomicPairCompareExchange)

// Addressing modes represent the "shape" of inputs to an instruction.
// Many instructions support multiple addressing modes. Addressing modes
// are encoded into the InstructionCode of the instruction and tell the
// code generator after register allocation which assembler method to call.
//
// We use the following local notation for addressing modes:
//
// R = register
// O = register or stack slot
// D = double register
// I = immediate (handle, external, int32)
// MRI = [register + immediate]
// MRR = [register + register]
// TODO(plind): Add the new r6 address modes.
#define TARGET_ADDRESSING_MODE_LIST(V) \
  V(MRI) /* [%r0 + K] */               \
  V(MRR) /* [%r0 + %r1] */

}  // namespace compiler
}  // namespace internal
}  // namespace v8

#endif  // V8_COMPILER_BACKEND_MIPS_INSTRUCTION_CODES_MIPS_H_
