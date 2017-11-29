#ifndef LX_ASM_H
#define LX_ASM_H

#include <array>

#include <cstdint>

#include "Bytes.h"
#include "Operands.h"
#include "InstructionFormat.h"
#include "MemoryOperation.h"
#include "Opcode.h"

template <Immediate_type<1>::type x> constexpr Immediate<1, x, false> Imm8{};
template <Immediate_type<2>::type x> constexpr Immediate<2, x, false> Imm16{};
template <Immediate_type<4>::type x> constexpr Immediate<4, x, false> Imm32{};
template <Immediate_type<8>::type x> constexpr Immediate<8, x, false> Imm64{};
constexpr auto VarImm8  = Immediate<1, 0x0, true>{};
constexpr auto VarImm16 = Immediate<2, 0x0, true>{};
constexpr auto VarImm32 = Immediate<4, 0x0, true>{};
constexpr auto VarImm64 = Immediate<8, 0x0, true>{};

constexpr auto al = Reg8<0>{}; constexpr auto ax = Reg16<0>{}; constexpr auto eax = Reg32<0>{}; constexpr auto rax = Reg64<0>{};
constexpr auto cl = Reg8<1>{}; constexpr auto cx = Reg16<1>{}; constexpr auto ecx = Reg32<1>{}; constexpr auto rcx = Reg64<1>{};
constexpr auto dl = Reg8<2>{}; constexpr auto dx = Reg16<2>{}; constexpr auto edx = Reg32<2>{}; constexpr auto rdx = Reg64<2>{};
constexpr auto bl = Reg8<3>{}; constexpr auto bx = Reg16<3>{}; constexpr auto ebx = Reg32<3>{}; constexpr auto rbx = Reg64<3>{};
constexpr auto ah = Reg8<4>{}; constexpr auto sp = Reg16<4>{}; constexpr auto esp = Reg32<4>{}; constexpr auto rsp = Reg64<4>{};
constexpr auto ch = Reg8<5>{}; constexpr auto bp = Reg16<5>{}; constexpr auto ebp = Reg32<5>{}; constexpr auto rbp = Reg64<5>{};
constexpr auto dh = Reg8<6>{}; constexpr auto si = Reg16<6>{}; constexpr auto esi = Reg32<6>{}; constexpr auto rsi = Reg64<6>{};
constexpr auto bh = Reg8<7>{}; constexpr auto di = Reg16<7>{}; constexpr auto edi = Reg32<7>{}; constexpr auto rdi = Reg64<7>{};

constexpr auto r8  = Reg64<8+0>{};
constexpr auto r9  = Reg64<8+1>{};
constexpr auto r10 = Reg64<8+2>{};
constexpr auto r11 = Reg64<8+3>{};
constexpr auto r12 = Reg64<8+4>{};
constexpr auto r13 = Reg64<8+5>{};
constexpr auto r14 = Reg64<8+6>{};
constexpr auto r15 = Reg64<8+7>{};

#endif //LX_ASM_H
