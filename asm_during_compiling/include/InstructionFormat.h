#ifndef INSTRUCTION_FORMAT_H
#define INSTRUCTION_FORMAT_H

//! rex
template <size_t s, size_t i>
constexpr bool get_rex_r (Register<s, i>)
{
    return Register<s, i>::index > 7;
}

constexpr bool get_rex_r (NoReg)
{
    return false;
}

template <size_t s, size_t i>
constexpr bool get_rex_x (Register<s, i>)
{
    return Register<s, i>::index > 7;
}

constexpr bool get_rex_x (NoReg)
{
    return false;
}

template <size_t s, size_t i>
constexpr bool get_rex_b (Register<s, i>)
{
    return Register<s, i>::index > 7;
}

constexpr bool get_rex_b (NoReg)
{
    return false;
}


//! prefix
template <uint8_t... code>
constexpr auto prefix ()
{
    return Bytes<ByteArray<code...>, decltype(make_flag_array<false, sizeof...(code)>())>{};
}

template <uint8_t w, uint8_t r, uint8_t x, uint8_t b>
constexpr auto rex ()
{ 
    constexpr auto c = w | r | x | b;
    static_assert(c <= 0b1);
    if constexpr (c == 0) {
	return Bytes<ByteArray<>, FlagArray<>>{};
    } else {
	return to_bytes<1, (0b0100 << 4) + (w << 3) + (r << 2) + (x << 1) + b>();
    }
}

// figure 2-4.
template <uint8_t w, size_t s, typename r1, typename disp, size_t i>
constexpr auto rex (Memory<s, r1, NoReg, NoScale, disp>, Register<s, i>)
{
    using reg = Register<s, i>;
    return rex<w, get_rex_r(reg{}), 0, get_rex_b(r1{})>();
}

template <uint8_t w, uint8_t digit, size_t s, typename r1, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto rex (Memory<s, r1, NoReg, NoScale, disp>, Immediate<imms, x, is_var>)
{
    using mem = Memory<s, r1, NoReg, NoScale, disp>;
    using imm = Immediate<imms, x, is_var>;
    static_assert(mem::size >= imm::size);
    static_assert(digit >= 0 && digit <= 7);
    return rex<w, digit, 0, get_rex_b(r1{})>();
}

// figure 2-5.
template <uint8_t w, size_t s1, size_t i1, size_t s2, size_t i2>
constexpr auto rex (Register<s1, i1>, Register<s2, i2>)
{
    using r1 = Register<s1, i1>;
    using r2 = Register<s2, i2>;
    return rex<w, get_rex_r(r2{}), 0, get_rex_b(r1{})>();
}

// figure 2-6.
template <uint8_t w, size_t s, typename r1, typename r2, typename scale, typename disp, size_t i>
constexpr auto rex (Memory<s, r1, r2, scale, disp>, Register<s, i>)
{
    using reg = Register<s, i>;
    return rex<w, get_rex_r(reg{}), get_rex_x(r2{}), get_rex_b(r1{})>();
}

template <uint8_t w, uint8_t digit, size_t s, typename r1, typename r2, typename scale, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto rex (Memory<s, r1, r2, scale, disp>, Immediate<imms, x, is_var>)
{
    using mem = Memory<s, r1, NoReg, NoScale, disp>;
    using imm = Immediate<imms, x, is_var>;
    static_assert(mem::size >= imm::size);
    static_assert(digit >= 0 && digit <= 7);
    return rex<w, digit, get_rex_x(r2{}), get_rex_b(r1{})>();
}

// Figure 2-7.
template <uint8_t w, size_t s, size_t i>
constexpr auto rex (Register<s, i>)
{
    using reg = Register<s, i>;
    return rex<w, 0, 0, get_rex_b(reg{})>();
}

template <uint8_t... code>
constexpr auto opcode () {
    return Bytes<ByteArray<code...>, decltype(make_flag_array<false, sizeof...(code)>())>{};
}

template <uint8_t mode, uint8_t reg, uint8_t rm>
constexpr auto modrm ()
{
    static_assert(mode | (0b11  == 0b11));
    static_assert(reg  | (0b111 == 0b111));
    static_assert(rm   | (0b111 == 0b111));
    return to_bytes<1, (mode << 6) + (reg << 3) + rm>();
}

template <uint8_t scale, uint8_t index, uint8_t base>
constexpr auto sib ()
{
    static_assert(scale | (0b11 == 0b11));
    static_assert(index | (0b111 == 0b111));
    static_assert(base  | (0b111 == 0b111));
    return to_bytes<1, (scale << 6) + (index << 3) + base>();
}

template <size_t s, typename r1, typename r2, typename scale, typename disp>
constexpr auto sib (Memory<s, r1, r2, scale, disp>)
{
    return sib<scale::mode, r2::index, r1::index>();
}

template <uint8_t reg, size_t s, size_t i,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto modrm (Register<s, i>, Immediate<imms, x, is_var>)
{
    return modrm<0b11, reg, Register<s, i>::index % 8>();
}

template <size_t s1, size_t i1, size_t s2, size_t i2>
constexpr auto modrm (Register<s1, i1>, Register<s2, i2>)
{
    return modrm<0b11, Register<s2, i2>::index % 8, Register<s1, i1>::index % 8>();
}

//! [disp32]
template <uint8_t digit, size_t s, typename Displacement_type<4>::type d,
          size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto modrm (Memory<s, NoReg, NoReg, NoScale, Disp32<d, is_var>>, Immediate<imms, x, is_var>)
{
    return modrm<0b00, digit, 0b101>() + to_bytes(Disp32<d, is_var>{});
}

//! mem <--> imm
template <uint8_t digit, size_t s, typename r1, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto modrm (Memory<s, r1, NoReg, NoScale, disp>, Immediate<imms, x, is_var>)
{
    return modrm<disp::mode, digit, r1::index % 8>() + to_bytes(disp{});
}

//! mem <--> imm
template <uint8_t digit, size_t s, typename r1, typename r2, typename scale, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto modrm (Memory<s, r1, r2, scale, disp>, Immediate<imms, x, is_var>)
{
    using mem = Memory<s, r1, r2, scale, disp>;
    if constexpr (r1::index == 0b100)
    {
	// TODO: not support!
    }
    else if constexpr (r2::index == 0b101)
    {
	// TODO: not support!
    }
    else {
	return modrm<disp::mode, digit, 0b100>() + sib(mem{}) + to_bytes(disp{});
    }
}

//! mem <--> reg
template <size_t s, typename r1, typename disp, size_t i>
constexpr auto modrm (Memory<s, r1, NoReg, NoScale, disp>, Register<s, i>)
{
    using reg = Register<s, i>;
    return modrm<disp::mode, reg::index % 8, r1::index % 8>() + to_bytes(disp{});
}

#endif //INSTRUCTION_FORMAT_H