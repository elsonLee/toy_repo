#ifndef OPCODE_H
#define OPCODE_H

//! ADD

// ADD r8 , imm8  -- 80 /0 ib
// ADD r16, imm8  -- 83 /0 ib
// ADD r16, imm16 -- 81 /0 iw
// ADD r32, imm8  -- 83 /0 ib
// ADD r32, imm32 -- 81 /0 id
// ADD r64, imm8  -- REX.W + 83 /0 ib
// ADD r64, imm32 -- REX.W + 81 /0 id
template <size_t s, size_t i,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto ADD (Register<s, i> reg, Immediate<imms, x, is_var> imm)
{
    constexpr int digit = 0;
    if constexpr (is_r8(reg) && is_imm8(imm))
    {
	return rex<0>(reg) + opcode<'\x80'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r16(reg) && is_imm8(imm))
    {
	return rex<0>(reg) + opcode<'\x83'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r16(reg) && is_imm16(imm))
    {
	return rex<0>(reg) + opcode<'\x81'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r32(reg) && is_imm8(imm))
    {
	return rex<0>(reg) + opcode<'\x83'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r32(reg) && is_imm32(imm))
    {
	return rex<0>(reg) + opcode<'\x81'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r64(reg) && is_imm8(imm))
    {
	return rex<1>(reg) + opcode<'\x83'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r64(reg) && is_imm32(imm))
    {
	return rex<1>(reg) + opcode<'\x81'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    } 
    else {
	//return Bytes<ByteArray<>, FlagArray<>>{};
    }
}

// ADD r8, r8   -- 00 /r
// ADD r16, r16 -- 01 /r
// ADD r32, r32 -- 01 /r
// ADD r64, r64 -- REX.W + 01 /r
template <size_t s1, size_t i1, size_t s2, size_t i2>
constexpr auto ADD (Register<s1, i1> reg1, Register<s2, i2> reg2)
{
    if constexpr (is_r8(reg1) && is_r8(reg2))
    {
	return rex<0>(reg1, reg2) + opcode<'\x00'>() + modrm(reg1, reg2);
    }
    else if constexpr (is_r16(reg1) && is_r16(reg2))
    {
	return rex<0>(reg1, reg2) + opcode<'\x01'>() + modrm(reg1, reg2);
    }
    else if constexpr (is_r32(reg1) && is_r32(reg2))
    {
	return rex<0>(reg1, reg2) + opcode<'\x01'>() + modrm(reg1, reg2);
    }
    else if constexpr (is_r64(reg1) && is_r64(reg2))
    {
	return rex<1>(reg1, reg2) + opcode<'\x01'>() + modrm(reg1, reg2);
    }
    else {
	//return Bytes<ByteArray<>, FlagArray<>>{};
    }
}

// add m8, imm8   -- 80 /0 ib
// add m16, imm8  -- 83 /0 ib
// add m16, imm16 -- 81 /0 iw
// add m32, imm8  -- 83 /0 ib
// add m32, imm32 -- 81 /0 id
// add m64, imm8  -- REX.W + 83 /0 ib
// add m64, imm32 -- REX.W + 81 /0 id
template <size_t s, typename r1, typename r2,  typename scale, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto ADD (Memory<s, r1, r2, scale, disp> mem, Immediate<imms, x, is_var> imm)
{
    if constexpr (is_m8(mem) && is_imm8(imm))
    {
	return rex<0, 0>(mem, imm) + opcode<'\x80'>() + modrm<0>(mem, imm) + to_bytes(imm);
    } 
    else if constexpr (is_m16(mem) && is_imm8(imm))
    {
	return rex<0, 0>(mem, imm) + opcode<'\x83'>() + modrm<0>(mem, imm) + to_bytes(imm);
    } 
    else if constexpr (is_m16(mem) && is_imm16(imm))
    {
	return rex<0, 0>(mem, imm) + opcode<'\x81'>() + modrm<0>(mem, imm) + to_bytes(imm);
    } 
    else if constexpr (is_m32(mem) && is_imm8(imm))
    {
	return rex<0, 0>(mem, imm) + opcode<'\x83'>() + modrm<0>(mem, imm) + to_bytes(imm);
    } 
    else if constexpr (is_m32(mem) && is_imm32(imm))
    {
	return rex<0, 0>(mem, imm) + opcode<'\x81'>() + modrm<0>(mem, imm) + to_bytes(imm);
    } 
    else if constexpr (is_m64(mem) && is_imm8(imm))
    {
	return rex<1, 0>(mem, imm) + opcode<'\x83'>() + modrm<0>(mem, imm) + to_bytes(imm);
    } 
    else if constexpr (is_m64(mem) && is_imm32(imm))
    {
	return rex<1, 0>(mem, imm) + opcode<'\x81'>() + modrm<0>(mem, imm) + to_bytes(imm);
    } 
    else {

    }
}


// add m8, r8   -- 00 /r
// add m16, r16 -- 01 /r
// add m32, r32 -- 01 /r
// add m64, r64 -- REX.W + 01 /r
template <size_t memsize, typename r1, typename r2,  typename scale, typename disp,
	  size_t regsize, size_t i>
constexpr auto ADD (Memory<memsize, r1, r2, scale, disp> mem, Register<regsize, i> reg)
{
    if constexpr (is_m8(mem) && is_r8(reg))
    {
	return rex<0>(mem, reg) + opcode<'\x00'>() + modrm(mem, reg);
    } 
    else if constexpr (is_m16(mem) && is_r16(reg))
    {
	return rex<0>(mem, reg) + opcode<'\x01'>() + modrm(mem, reg);
    } 
    else if constexpr (is_m32(mem) && is_r32(reg))
    {
	return rex<0>(mem, reg) + opcode<'\x01'>() + modrm(mem, reg);
    } 
    else if constexpr (is_m64(mem) && is_r64(reg))
    {
	return rex<1>(mem, reg) + opcode<'\x01'>() + modrm(mem, reg);
    } 
    else {

    }
}

//! SUB

// SUB r8 , imm8  -- 80 /5 ib
// SUB r16, imm8  -- 83 /5 ib
// SUB r16, imm16 -- 81 /5 iw
// SUB r32, imm8  -- 83 /5 ib
// SUB r32, imm32 -- 81 /5 id
// SUB r64, imm8  -- REX.W + 83 /5 ib
// SUB r64, imm32 -- REX.W + 81 /5 id
template <size_t s, size_t i, size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto SUB (Register<s, i> reg, Immediate<imms, x, is_var> imm)
{
    //TODO: duplicated with ADD
    constexpr int digit = 5;
    if constexpr (is_r8(reg) && is_imm8(imm))
    {
	return rex<0>(reg) + opcode<'\x80'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r16(reg) && is_imm8(imm))
    {
	return rex<0>(reg) + opcode<'\x83'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r16(reg) && is_imm16(imm))
    {
	return rex<0>(reg) + opcode<'\x81'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r32(reg) && is_imm8(imm))
    {
	return rex<0>(reg) + opcode<'\x83'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r32(reg) && is_imm32(imm))
    {
	return rex<0>(reg) + opcode<'\x81'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r64(reg) && is_imm8(imm))
    {
	return rex<1>(reg) + opcode<'\x83'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r64(reg) && is_imm32(imm))
    {
	return rex<1>(reg) + opcode<'\x81'>() + modrm<digit>(reg, imm) + to_bytes(imm);
    } 
    else {
	//return Bytes<ByteArray<>, FlagArray<>>{};
    }
}

//! INC
// INC r8
// INC r16
// INC r32
// INC r64
template <size_t s, size_t i>
constexpr auto INC (Register<s, i> reg)
{
    constexpr uint8_t digit = 0; 
    if constexpr (is_r8(reg) || is_r16(reg) || is_r32(reg))
    {
	return rex<0>(reg) + opcode<'\xFF'>() + modrm<digit>(reg);
    }
    else if constexpr (is_r16(reg))
    {
	return rex<0>(reg) + opcode<'\xFF'>() + modrm<digit>(reg);
    }
    else if constexpr (is_r64(reg))
    {
	return rex<1>(reg) + opcode<'\xFF'>() + modrm<digit>(reg);
    }
    else
    {

    }
}

// INC m8
// INC m16
// INC m32
// INC m64
template <size_t memsize, typename r1, typename r2,  typename scale, typename disp>
constexpr auto INC (Memory<memsize, r1, r2, scale, disp> mem)
{
    constexpr uint8_t digit = 0;
    if constexpr (is_m8(mem) || is_m16(mem) || is_m32(mem))
    {
	return rex<0, digit>(mem) + opcode<'\xFF'>() + modrm<digit>(mem);
    } 
    else if constexpr (is_m16(mem))
    {
	return rex<0, digit>(mem) + opcode<'\xFF'>() + modrm<digit>(mem);
    }
    else if constexpr (is_m64(mem))
    {
	return rex<1, digit>(mem) + opcode<'\xFF'>() + modrm<digit>(mem);
    } 
    else {

    }
}

#endif //OPCODE_H
