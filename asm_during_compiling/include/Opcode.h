#ifndef OPCODE_H
#define OPCODE_H

// ADD_LIKE r8 , imm8  -- op1 /0 ib
// ADD_LIKE r16, imm8  -- op2 /0 ib
// ADD_LIKE r32, imm8  -- op2 /0 ib
// ADD_LIKE r64, imm8  -- REX.W + op2 /0 ib
// ADD_LIKE r16, imm16 -- op3 /0 iw
// ADD_LIKE r32, imm32 -- op3 /0 id
// ADD_LIKE r64, imm32 -- REX.W + op3 /0 id
template <uint8_t digit, uint8_t op_1, uint8_t op_2, uint8_t op_3,
	  size_t s, size_t i,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto ADD_LIKE (Register<s, i> reg, Immediate<imms, x, is_var> imm)
{
    if constexpr (is_r8(reg) && is_imm8(imm))
    {
	return rex<0>(reg) + opcode<op_1>() + modrm<digit>(reg) + to_bytes(imm);
    }
    else if constexpr ((is_r16(reg) && is_imm8(imm)) ||
		       (is_r32(reg) && is_imm8(imm)) ||
		       (is_r64(reg) && is_imm8(imm)))
    {
	return rex<is_r64(reg)?1:0>(reg) + opcode<op_2>() + modrm<digit>(reg) + to_bytes(imm);
    }
    else if constexpr ((is_r16(reg) && is_imm16(imm)) ||
		       (is_r32(reg) && is_imm32(imm)) ||
		       (is_r64(reg) && is_imm32(imm)))
    {
	return rex<is_r64(reg)?1:0>(reg) + opcode<op_3>() + modrm<digit>(reg) + to_bytes(imm);
    }
    else
    {

    }
}

// ADD_LIKE m8,  imm8  -- op1 /0 ib
// ADD_LIKE m16, imm8  -- op2 /0 ib
// ADD_LIKE m32, imm8  -- op2 /0 ib
// ADD_LIKE m64, imm8  -- REX.W + op2 /0 ib
// ADD_LIKE m16, imm16 -- op3 /0 iw
// ADD_LIKE m32, imm32 -- op3 /0 id
// ADD_LIKE m64, imm32 -- REX.W + op3 /0 id
template <uint8_t digit, uint8_t op_1, uint8_t op_2, uint8_t op_3,
	  size_t s, typename r1, typename r2,  typename scale, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto ADD_LIKE (Memory<s, r1, r2, scale, disp> mem, Immediate<imms, x, is_var> imm)
{
    if constexpr (is_m8(mem) && is_imm8(imm))
    {
	return rex<0, 0>(mem) + opcode<op_1>() + modrm<digit>(mem) + to_bytes(imm);
    } 
    else if constexpr ((is_m16(mem) && is_imm8(imm)) ||
		       (is_m32(mem) && is_imm8(imm)) ||
		       (is_m64(mem) && is_imm8(imm)))
    {
	return rex<is_m64(mem)?1:0, 0>(mem) + opcode<op_2>() + modrm<digit>(mem) + to_bytes(imm);
    } 
    else if constexpr ((is_m16(mem) && is_imm16(imm)) ||
		       (is_m32(mem) && is_imm32(imm)) ||
		       (is_m64(mem) && is_imm32(imm)))
    {
	return rex<is_m64(mem)?1:0, 0>(mem) + opcode<op_3>() + modrm<digit>(mem) + to_bytes(imm);
    } 
    else
    {

    }
}

// ADD_LIKE r8, r8   -- op1 /r
// ADD_LIKE r16, r16 -- op2 /r
// ADD_LIKE r32, r32 -- op2 /r
// ADD_LIKE r64, r64 -- REX.W + op2 /r
template <uint8_t op_1, uint8_t op_2,
	  size_t s1, size_t i1, size_t s2, size_t i2>
constexpr auto ADD_LIKE (Register<s1, i1> reg1, Register<s2, i2> reg2)
{
    if constexpr (is_r8(reg1) && is_r8(reg2))
    {
	return rex<0>(reg1, reg2) + opcode<op_1>() + modrm(reg1, reg2);
    }
    else if constexpr ((is_r16(reg1) && is_r16(reg2)) ||
		       (is_r32(reg1) && is_r32(reg2)) ||
		       (is_r64(reg1) && is_r64(reg2)))
    {
	return rex<is_r64(reg1)?1:0>(reg1, reg2) + opcode<op_2>() + modrm(reg1, reg2);
    }
    else
    {

    }
}

// ADD_LIKE m8,  r8  -- op1 /r
// ADD_LIKE m16, r16 -- op2 /r
// ADD_LIKE m32, r32 -- op2 /r
// ADD_LIKE m64, r64 -- REX.W + op2 /r
template <uint8_t op_1, uint8_t op_2,
	  size_t memsize, typename r1, typename r2,  typename scale, typename disp,
	  size_t regsize, size_t i>
constexpr auto ADD_LIKE (Memory<memsize, r1, r2, scale, disp> mem, Register<regsize, i> reg)
{
    if constexpr (is_m8(mem) && is_r8(reg))
    {
	return rex<0>(mem, reg) + opcode<op_1>() + modrm(mem, reg);
    } 
    else if constexpr ((is_m16(mem) && is_r16(reg)) ||
		       (is_m32(mem) && is_r32(reg)) ||
		       (is_m64(mem) && is_r64(reg)))
    {
	return rex<is_m64(mem)?1:0>(mem, reg) + opcode<op_2>() + modrm(mem, reg);
    } 
    else
    {

    }
}

// ADD_LIKE r8,  m8  -- op1 /r
// ADD_LIKE r16, m16 -- op2 /r
// ADD_LIKE r32, m32 -- op2 /r
// ADD_LIKE r64, m64 -- REX.W + op2 /r
template <uint8_t op_1, uint8_t op_2,
	  size_t regsize, size_t i,
	  size_t memsize, typename r1, typename r2,  typename scale, typename disp>
constexpr auto ADD_LIKE (Register<regsize, i> reg, Memory<memsize, r1, r2, scale, disp> mem)
{
    if constexpr (is_r8(reg) && is_m8(mem))
    {
	return rex<0>(mem, reg) + opcode<op_1>() + modrm(mem, reg);
    } 
    else if constexpr ((is_r16(reg) && is_m16(mem)) ||
		       (is_r32(reg) && is_m32(mem)) ||
		       (is_r64(reg) && is_m64(mem)))
    {
	return rex<is_m64(mem)?1:0>(mem, reg) + opcode<op_2>() + modrm(mem, reg);
    } 
    else
    {

    }
}


// ADD r8 , imm8  -- 80 /0 ib
// ADD r16, imm8  -- 83 /0 ib
// ADD r32, imm8  -- 83 /0 ib
// ADD r64, imm8  -- REX.W + 83 /0 ib
// ADD r16, imm16 -- 81 /0 iw
// ADD r32, imm32 -- 81 /0 id
// ADD r64, imm32 -- REX.W + 81 /0 id
template <size_t s, size_t i,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto ADD (Register<s, i> reg, Immediate<imms, x, is_var> imm)
{
    return ADD_LIKE<0, '\x80', '\x83', '\x81'>(reg, imm);
}

// ADD m8,  imm8  -- 80 /0 ib
// ADD m16, imm8  -- 83 /0 ib
// ADD m16, imm16 -- 81 /0 iw
// ADD m32, imm8  -- 83 /0 ib
// ADD m32, imm32 -- 81 /0 id
// ADD m64, imm8  -- REX.W + 83 /0 ib
// ADD m64, imm32 -- REX.W + 81 /0 id
template <size_t s, typename r1, typename r2,  typename scale, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto ADD (Memory<s, r1, r2, scale, disp> mem, Immediate<imms, x, is_var> imm)
{
    return ADD_LIKE<0, '\x80', '\x83', '\x81'>(mem, imm);
}

// ADD r8, r8   -- 00 /r
// ADD r16, r16 -- 01 /r
// ADD r32, r32 -- 01 /r
// ADD r64, r64 -- REX.W + 01 /r
template <size_t s1, size_t i1, size_t s2, size_t i2>
constexpr auto ADD (Register<s1, i1> reg1, Register<s2, i2> reg2)
{
    return ADD_LIKE<'\x00', '\x01'>(reg1, reg2);
}

// ADD m8,  r8  -- 00 /r
// ADD m16, r16 -- 01 /r
// ADD m32, r32 -- 01 /r
// ADD m64, r64 -- REX.W + 01 /r
template <size_t memsize, typename r1, typename r2,  typename scale, typename disp,
	  size_t regsize, size_t i>
constexpr auto ADD (Memory<memsize, r1, r2, scale, disp> mem, Register<regsize, i> reg)
{
    return ADD_LIKE<'\x00', '\x01'>(mem, reg);
}

// ADD r8,  m8  -- 02 /r
// ADD r16, m16 -- 03 /r
// ADD r32, m32 -- 03 /r
// ADD r64, m64 -- REX.W + 03 /r
template <size_t regsize, size_t i,
	  size_t memsize, typename r1, typename r2,  typename scale, typename disp>
constexpr auto ADD (Register<regsize, i> reg, Memory<memsize, r1, r2, scale, disp> mem)
{
    return ADD_LIKE<'\x02', '\x03'>(reg, mem);
}


// CMP r8 , imm8  -- 80 /7 ib
// CMP r16, imm8  -- 83 /7 ib
// CMP r32, imm8  -- 83 /7 ib
// CMP r64, imm8  -- REX.W + 83 /7 ib
// CMP r16, imm16 -- 81 /7 iw
// CMP r32, imm32 -- 81 /7 id
// CMP r64, imm32 -- REX.W + 81 /7 id
template <size_t s, size_t i,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto CMP (Register<s, i> reg, Immediate<imms, x, is_var> imm)
{
    return ADD_LIKE<7, '\x80', '\x83', '\x81'>(reg, imm);
}

// CMP m8,  imm8  -- 80 /7 ib
// CMP m16, imm8  -- 83 /7 ib
// CMP m32, imm8  -- 83 /7 ib
// CMP m64, imm8  -- REX.W + 83 /7 ib
// CMP m16, imm16 -- 81 /7 iw
// CMP m32, imm32 -- 81 /7 id
// CMP m64, imm32 -- REX.W + 81 /7 id
template <size_t s, typename r1, typename r2,  typename scale, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto CMP (Memory<s, r1, r2, scale, disp> mem, Immediate<imms, x, is_var> imm)
{
    return ADD_LIKE<7, '\x80', '\x83', '\x81'>(mem, imm);
}

// CMP r8, r8   -- 38 /r
// CMP r16, r16 -- 39 /r
// CMP r32, r32 -- 39 /r
// CMP r64, r64 -- REX.W + 39 /r
template <size_t s1, size_t i1, size_t s2, size_t i2>
constexpr auto CMP (Register<s1, i1> reg1, Register<s2, i2> reg2)
{
    return ADD_LIKE<'\x38', '\x39'>(reg1, reg2);
}

// CMP m8,  r8  -- 38 /r
// CMP m16, r16 -- 39 /r
// CMP m32, r32 -- 39 /r
// CMP m64, r64 -- REX.W + 39 /r
template <size_t memsize, typename r1, typename r2,  typename scale, typename disp,
	  size_t regsize, size_t i>
constexpr auto CMP (Memory<memsize, r1, r2, scale, disp> mem, Register<regsize, i> reg)
{
    return ADD_LIKE<'\x38', '\x39'>(mem, reg);
}

// CMP r8,  m8  -- 3a /r
// CMP r16, m16 -- 3b /r
// CMP r32, m32 -- 3b /r
// CMP r64, m64 -- REX.W + 3b /r
template <size_t regsize, size_t i,
	  size_t memsize, typename r1, typename r2,  typename scale, typename disp>
constexpr auto CMP (Register<regsize, i> reg, Memory<memsize, r1, r2, scale, disp> mem)
{
    return ADD_LIKE<'\x3a', '\x3b'>(reg, mem);
}

// SUB r8 , imm8  -- 80 /5 ib
// SUB r16, imm8  -- 83 /5 ib
// SUB r32, imm8  -- 83 /5 ib
// SUB r64, imm8  -- REX.W + 83 /5 ib
// SUB r16, imm16 -- 81 /5 iw
// SUB r32, imm32 -- 81 /5 id
// SUB r64, imm32 -- REX.W + 81 /5 id
template <size_t s, size_t i, size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto SUB (Register<s, i> reg, Immediate<imms, x, is_var> imm)
{
    return ADD_LIKE<5, '\x80', '\x83', '\x81'>(reg, imm);
}

// SUB m8,  imm8  -- 80 /5 ib
// SUB m16, imm8  -- 83 /5 ib
// SUB m32, imm8  -- 83 /5 ib
// SUB m64, imm8  -- REX.W + 83 /5 ib
// SUB m16, imm16 -- 81 /5 iw
// SUB m32, imm32 -- 81 /5 id
// SUB m64, imm32 -- REX.W + 81 /5 id
template <size_t s, typename r1, typename r2,  typename scale, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto SUB (Memory<s, r1, r2, scale, disp> mem, Immediate<imms, x, is_var> imm)
{
    return ADD_LIKE<5, '\x80', '\x83', '\x81'>(mem, imm);
}

// SUB r8,  r8  -- 28 /r
// SUB r16, r16 -- 29 /r
// SUB r32, r32 -- 29 /r
// SUB r64, r64 -- REX.W + 29 /r
template <size_t s1, size_t i1, size_t s2, size_t i2>
constexpr auto SUB (Register<s1, i1> reg1, Register<s2, i2> reg2)
{
    return ADD_LIKE<'\x28', '\x29'>(reg1, reg2);
}

// SUB m8,  r8  -- 28 /r
// SUB m16, r16 -- 29 /r
// SUB m32, r32 -- 29 /r
// SUB m64, r64 -- REX.W + 29 /r
template <size_t memsize, typename r1, typename r2,  typename scale, typename disp,
	  size_t regsize, size_t i>
constexpr auto SUB (Memory<memsize, r1, r2, scale, disp> mem, Register<regsize, i> reg)
{
    return ADD_LIKE<'\x28', '\x29'>(mem, reg);
}

// SUB r8,  m8  -- 2a /r
// SUB r16, m16 -- 2b /r
// SUB r32, m32 -- 2b /r
// SUB r64, m64 -- REX.W + 2b /r
template <size_t regsize, size_t i,
	  size_t memsize, typename r1, typename r2,  typename scale, typename disp>
constexpr auto SUB (Register<regsize, i> reg, Memory<memsize, r1, r2, scale, disp> mem)
{
    return ADD_LIKE<'\x2a', '\x2b'>(reg, mem);
}

// DEC r8  -- FE /1
// DEC r16 -- FF /1
// DEC r32 -- FF /1
// DEC r64 -- REX.W + FF /1
template <size_t s, size_t i>
constexpr auto DEC (Register<s, i> reg)
{
    constexpr uint8_t digit = 1; 
    if constexpr (is_r8(reg))
    {
	return rex<0>(reg) + opcode<'\xFE'>() + modrm<digit>(reg);
    }
    else if constexpr (is_r16(reg) || is_r32(reg) || is_r64(reg))
    {
	return rex<is_r64(reg)?1:0>(reg) + opcode<'\xFF'>() + modrm<digit>(reg);
    }
    else
    {

    }
}

// DEC m8  -- FE /1
// DEC m16 -- FF /1
// DEC m32 -- FF /1
// DEC m64 -- REX.W + FF /1
template <size_t memsize, typename r1, typename r2,  typename scale, typename disp>
constexpr auto DEC (Memory<memsize, r1, r2, scale, disp> mem)
{
    constexpr uint8_t digit = 1;
    if constexpr (is_m8(mem))
    {
	return rex<0, 0>(mem) + opcode<'\xFE'>() + modrm<digit>(mem);
    } 
    else if constexpr (is_m16(mem) || is_m32(mem) || is_m64(mem))
    {
	return rex<is_m64(mem)?1:0, 0>(mem) + opcode<'\xFF'>() + modrm<digit>(mem);
    }
    else {

    }
}

// INC r8  -- FE /0
// INC r16 -- FF /0
// INC r32 -- FF /0
// INC r64 -- REX.W + FF /0
template <size_t s, size_t i>
constexpr auto INC (Register<s, i> reg)
{
    constexpr uint8_t digit = 0; 
    if constexpr (is_r8(reg))
    {
	return rex<0>(reg) + opcode<'\xFE'>() + modrm<digit>(reg);
    }
    else if constexpr (is_r16(reg) || is_r32(reg) || is_r64(reg))
    {
	return rex<is_r64(reg)?1:0>(reg) + opcode<'\xFF'>() + modrm<digit>(reg);
    }
    else
    {

    }
}

// INC m8  -- FE /0
// INC m16 -- FF /0
// INC m32 -- FF /0
// INC m64 -- REX.W + FF /0
template <size_t memsize, typename r1, typename r2,  typename scale, typename disp>
constexpr auto INC (Memory<memsize, r1, r2, scale, disp> mem)
{
    constexpr uint8_t digit = 0;
    if constexpr (is_m8(mem))
    {
	return rex<0, digit>(mem) + opcode<'\xFE'>() + modrm<digit>(mem);
    } 
    else if constexpr (is_m16(mem) || is_m32(mem) || is_m64(mem))
    {
	return rex<is_m64(mem)?1:0, 0>(mem) + opcode<'\xFF'>() + modrm<digit>(mem);
    }
    else {

    }
}

//! SYSCALL
constexpr auto SYSCALL ()
{
    return opcode<'\x0F'>() + opcode<'\x05'>();
}


// XOR r8 , imm8  -- 80 /6 ib
// XOR r16, imm8  -- 83 /6 ib
// XOR r32, imm8  -- 83 /6 ib
// XOR r64, imm8  -- REX.W + 83 /6 ib
// XOR r16, imm16 -- 81 /6 iw
// XOR r32, imm32 -- 81 /6 id
// XOR r64, imm32 -- REX.W + 81 /6 id
template <size_t s, size_t i,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto XOR (Register<s, i> reg, Immediate<imms, x, is_var> imm)
{
    return ADD_LIKE<6, '\x80', '\x83', '\x81'>(reg, imm);
}

// XOR m8,  imm8  -- 80 /6 ib
// XOR m16, imm8  -- 83 /6 ib
// XOR m32, imm8  -- 83 /6 ib
// XOR m64, imm8  -- REX.W + 83 /6 ib
// XOR m16, imm16 -- 81 /6 iw
// XOR m32, imm32 -- 81 /6 id
// XOR m64, imm32 -- REX.W + 81 /6 id
template <size_t s, typename r1, typename r2,  typename scale, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto XOR (Memory<s, r1, r2, scale, disp> mem, Immediate<imms, x, is_var> imm)
{
    return ADD_LIKE<6, '\x80', '\x83', '\x81'>(mem, imm);
}

// XOR r8,  r8  -- 30 /r
// XOR r16, r16 -- 31 /r
// XOR r32, r32 -- 31 /r
// XOR r64, r64 -- REX.W + 31 /r
template <size_t s1, size_t i1, size_t s2, size_t i2>
constexpr auto XOR (Register<s1, i1> reg1, Register<s2, i2> reg2)
{
    return ADD_LIKE<'\x30', '\x31'>(reg1, reg2);
}

// XOR m8,  r8  -- 30 /r
// XOR m16, r16 -- 31 /r
// XOR m32, r32 -- 31 /r
// XOR m64, r64 -- REX.W + 31 /r
template <size_t memsize, typename r1, typename r2,  typename scale, typename disp,
	  size_t regsize, size_t i>
constexpr auto XOR (Memory<memsize, r1, r2, scale, disp> mem, Register<regsize, i> reg)
{
    return ADD_LIKE<'\x30', '\x31'>(mem, reg);
}

// XOR r8,  m8  -- 32 /r
// XOR r16, m16 -- 33 /r
// XOR r32, m32 -- 33 /r
// XOR r64, m64 -- REX.W + 33 /r
template <size_t regsize, size_t i,
	  size_t memsize, typename r1, typename r2,  typename scale, typename disp>
constexpr auto XOR (Register<regsize, i> reg, Memory<memsize, r1, r2, scale, disp> mem)
{
    return ADD_LIKE<'\x32', '\x33'>(reg, mem);
}

#endif //OPCODE_H
