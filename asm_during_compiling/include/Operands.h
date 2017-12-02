#ifndef OPERANDS_H
#define OPERANDS_H

//! Immediate
template <size_t s> struct Immediate_type;
template <> struct Immediate_type<1> { using type = int8_t; };
template <> struct Immediate_type<2> { using type = int16_t; };
template <> struct Immediate_type<4> { using type = int32_t; };
template <> struct Immediate_type<8> { using type = int64_t; };

template <size_t s, typename Immediate_type<s>::type x, bool is_var = false>
struct Immediate {
    using type = decltype(x);
    static constexpr size_t size = s;
    static type value;
    static constexpr bool is_variable = is_var;
};

template <size_t s, typename Immediate_type<s>::type x, bool is_var>
constexpr bool is_imm8  (Immediate<s, x, is_var> imm) { return imm.size == 1; }

template <size_t s, typename Immediate_type<s>::type x, bool is_var>
constexpr bool is_imm16 (Immediate<s, x, is_var> imm) { return imm.size == 2; }

template <size_t s, typename Immediate_type<s>::type x, bool is_var>
constexpr bool is_imm32 (Immediate<s, x, is_var> imm) { return imm.size == 4; }

template <size_t s, typename Immediate_type<s>::type x, bool is_var>
constexpr bool is_imm64 (Immediate<s, x, is_var> imm) { return imm.size == 8; }


//! Register
template <size_t s, size_t i>
struct Register {
    static constexpr size_t size = s;
    static constexpr size_t index = i;
};

//using NoReg  = Register<0, 0>;
struct NoReg { static constexpr size_t size = 0; };
template <size_t i> using Reg8  = Register<1, i>;
template <size_t i> using Reg16 = Register<2, i>;
template <size_t i> using Reg32 = Register<4, i>;
template <size_t i> using Reg64 = Register<8, i>;

template <size_t s , size_t i>
constexpr bool is_r8  (Register<s, i> reg) { return reg.size == 1; }

template <size_t s , size_t i>
constexpr bool is_r16 (Register<s, i> reg) { return reg.size == 2; }

template <size_t s , size_t i>
constexpr bool is_r32 (Register<s, i> reg) { return reg.size == 4; }

template <size_t s , size_t i>
constexpr bool is_r64 (Register<s, i> reg) { return reg.size == 8; }


//! Memory
template <size_t s> struct Displacement_type;
template <>
struct Displacement_type<0> {
    using type = int8_t;
    constexpr static uint8_t mode = 0b00;
};

template <>
struct Displacement_type<1> {
    using type = int8_t;
    constexpr static uint8_t mode = 0b01;
};

template <>
struct Displacement_type<4> {
    using type = int32_t;
    constexpr static uint8_t mode = 0b10;
};

template <size_t s, typename Displacement_type<s>::type x, bool is_var = false>
struct Displacement {
    using type = decltype(x);
    constexpr static size_t size = s; 
    constexpr static type value = x;
    constexpr static uint8_t mode = Displacement_type<s>::mode;
    constexpr static bool is_variable = is_var;
};

using NoDisp =  Displacement<0, 0, false>;
template <typename Displacement_type<1>::type x, bool is_var>
using Disp8  = Displacement<1, x, is_var>;
template <typename Displacement_type<4>::type x, bool is_var>
using Disp32 = Displacement<4, x, is_var>;

template <size_t s, typename Displacement_type<s>::type x, bool is_var>
constexpr auto is_disp8 (Displacement<s, x, is_var>) { return s == 1; }

template <size_t s, typename Displacement_type<s>::type x, bool is_var>
constexpr auto is_disp32 (Displacement<s, x, is_var>) { return s == 4; }

template <size_t s>
struct Scale
{
    static_assert(s == 1 || s == 2 || s == 4 || s == 8);
    constexpr static size_t size = s; 
    constexpr static uint8_t mode = s == 1? 0b00 : (s == 2? 0b01 : (s == 4? 0b10 : 0b11));
};

using NoScale = Scale<1>;
using Scale2  = Scale<2>;
using Scale4  = Scale<4>;
using Scale8  = Scale<8>;

template <size_t s, typename r1, typename r2, typename scale, typename disp>
struct Memory {
    static constexpr size_t size = s;
};

template <typename r1, typename r2, typename scale, typename disp>
using Mem8  = Memory<1, r1, r2, scale, disp>;
template <typename r1, typename r2, typename scale, typename disp>
using Mem16 = Memory<2, r1, r2, scale, disp>;
template <typename r1, typename r2, typename scale, typename disp>
using Mem32 = Memory<4, r1, r2, scale, disp>;
template <typename r1, typename r2, typename scale, typename disp>
using Mem64 = Memory<8, r1, r2, scale, disp>;

template <size_t i> using Reg16 = Register<2, i>;
template <size_t i> using Reg32 = Register<4, i>;
template <size_t i> using Reg64 = Register<8, i>;

template <size_t s, typename r1, typename r2, typename scale, typename disp>
constexpr auto is_m8 (Memory<s, r1, r2, scale, disp> mem)  { return mem.size == 1; }

template <size_t s, typename r1, typename r2, typename scale, typename disp>
constexpr auto is_m16 (Memory<s, r1, r2, scale, disp> mem) { return mem.size == 2; }

template <size_t s, typename r1, typename r2, typename scale, typename disp>
constexpr auto is_m32 (Memory<s, r1, r2, scale, disp> mem) { return mem.size == 4; }

template <size_t s, typename r1, typename r2, typename scale, typename disp>
constexpr auto is_m64 (Memory<s, r1, r2, scale, disp> mem) { return mem.size == 8; }

// to_bytes
template <size_t s, typename Immediate_type<s>::type x, bool is_var>
constexpr auto to_bytes (Immediate<s, x, is_var>)
{
    if constexpr (is_var) {
	using ArgType = typename Immediate<s, x, is_var>::type;
    	return add_args<ArgType>(to_bytes<Immediate<s, x, is_var>::size, x, true>());
    } else {
    	return to_bytes<Immediate<s, x, is_var>::size, x, false>();
    }
}

template <size_t size, typename Displacement_type<size>::type x, bool is_var>
constexpr auto to_bytes (Displacement<size, x, is_var>)
{
    using disp = Displacement<size, x, is_var>;
    if constexpr (is_var) {
	return add_args<typename Displacement<size, x, is_var>::type>(to_bytes<disp::size, x, true>());
    } else {
	return to_bytes<disp::size, x, false>();
    }
}

#endif // OPERANDS_H
