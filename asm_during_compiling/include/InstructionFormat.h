#ifndef INSTRUCTION_FORMAT_H
#define INSTRUCTION_FORMAT_H

static const uint8_t non_default_operand_size = 2;   // 16-bit
static const uint8_t non_default_addressing_size = 4;   // 32-bit

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

//! prefix_66
template <size_t s, size_t i>
constexpr auto prefix_66 (Register<s, i>)
{
    if constexpr (s == non_default_operand_size) {
	return prefix<'\x66'>();
    } else {
	return Bytes<ByteArray<>, FlagArray<>>{};
    }
}

template <size_t s, typename r1, typename r2, typename scale, typename disp>
constexpr auto prefix_66 (Memory<s, r1, r2, scale, disp>)
{
    if constexpr (s == non_default_operand_size) {
	return prefix<'\x66'>();
    } else {
	return Bytes<ByteArray<>, FlagArray<>>{};
    }
}

//! prefix_67
template <size_t s, typename r1, typename r2, typename scale, typename disp>
constexpr auto prefix_67 (Memory<s, r1, r2, scale, disp>)
{
    if constexpr (r1::size == non_default_addressing_size) {
	return prefix<'\x67'>();
    } else {
	return Bytes<ByteArray<>, FlagArray<>>{};
    }
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
    using mem = Memory<s, r1, NoReg, NoScale, disp>;
    using reg = Register<s, i>;
    return prefix_67(mem{}) + prefix_66(mem{}) + rex<w, get_rex_r(reg{}), 0, get_rex_b(r1{})>();
}

// rex [base + disp]
template <uint8_t w, uint8_t digit, size_t s, typename r1, typename disp>
constexpr auto rex (Memory<s, r1, NoReg, NoScale, disp>)
{
    using mem = Memory<s, r1, NoReg, NoScale, disp>;
    static_assert(digit >= 0 && digit <= 7);
    return prefix_67(mem{}) + prefix_66(mem{}) + rex<w, digit, 0, get_rex_b(r1{})>();
}

//NOTE: imm is not needed
// rex [base + disp], imm
template <uint8_t w, uint8_t digit, size_t s, typename r1, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto rex (Memory<s, r1, NoReg, NoScale, disp>, Immediate<imms, x, is_var>)
{
    using mem = Memory<s, r1, NoReg, NoScale, disp>;
    using imm = Immediate<imms, x, is_var>;
    static_assert(mem::size >= imm::size);
    return rex<w, digit>(mem{});
}

// figure 2-5.
template <uint8_t w, size_t s1, size_t i1, size_t s2, size_t i2>
constexpr auto rex (Register<s1, i1>, Register<s2, i2>)
{
    using r1 = Register<s1, i1>;
    using r2 = Register<s2, i2>;
    static_assert(r1::size == r2::size);
    return prefix_66(r1{}) + rex<w, get_rex_r(r2{}), 0, get_rex_b(r1{})>();
}

// figure 2-6.
template <uint8_t w, size_t s, typename r1, typename r2, typename scale, typename disp, size_t i>
constexpr auto rex (Memory<s, r1, r2, scale, disp>, Register<s, i>)
{
    using mem = Memory<s, r1, r2, scale, disp>;
    using reg = Register<s, i>;
    return prefix_67(mem{}) + prefix_66(mem{}) + rex<w, get_rex_r(reg{}), get_rex_x(r2{}), get_rex_b(r1{})>();
}

template <uint8_t w, uint8_t digit, size_t s, typename r1, typename r2, typename scale, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto rex (Memory<s, r1, r2, scale, disp>, Immediate<imms, x, is_var>)
{
    using mem = Memory<s, r1, NoReg, NoScale, disp>;
    using imm = Immediate<imms, x, is_var>;
    static_assert(mem::size >= imm::size);
    static_assert(digit >= 0 && digit <= 7);
    return prefix_67(mem{}) + prefix_66(mem{}) + rex<w, digit, get_rex_x(r2{}), get_rex_b(r1{})>();
}

// Figure 2-7.
template <uint8_t w, size_t s, size_t i>
constexpr auto rex (Register<s, i>)
{
    using reg = Register<s, i>;
    return prefix_66(reg{}) + rex<w, 0, 0, get_rex_b(reg{})>();
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

//! reg
template <uint8_t reg, size_t s, size_t i>
constexpr auto modrm (Register<s, i>)
{
    return modrm<0b11, reg, Register<s, i>::index % 8>();
}

//! reg <--> imm
//NOTE: actually, imm is not needed
template <uint8_t reg, size_t s, size_t i,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto modrm (Register<s, i> _reg, Immediate<imms, x, is_var>)
{
    return modrm<reg>(_reg);
}

//! reg <--> reg
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

//! mem
template <uint8_t digit, size_t s, typename r1, typename disp>
constexpr auto modrm (Memory<s, r1, NoReg, NoScale, disp>)
{
    return modrm<disp::mode, digit, r1::index % 8>();
}

//! mem <--> imm
template <uint8_t digit, size_t s, typename r1, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto modrm (Memory<s, r1, NoReg, NoScale, disp>, Immediate<imms, x, is_var>)
{
    using mem = Memory<s, r1, NoReg, NoScale, disp>;
    using imm = Immediate<imms, x, is_var>;
    if constexpr (r1::index % 8 == 0b100)
    // [base] + disp8/32
    {
	return modrm<digit>(mem{}) + sib<0b00, 0b100, 0b100>() + to_bytes(disp{});
    }
    else if constexpr (disp::mode == 0b00 && r1::index % 8 == 0b101)
    // [ebp]
    {
	return modrm<digit>(Memory<s, r1, NoReg, NoScale, Disp8<0x0, false>>{}, imm{});
    }
    else
    {
	return modrm<digit>(mem{}) + to_bytes(disp{});
    }
}

//! mem <--> imm
template <uint8_t digit, size_t s, typename r1, typename r2, typename scale, typename disp,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto modrm (Memory<s, r1, r2, scale, disp>, Immediate<imms, x, is_var>)
{
    static_assert(r1::size == r2::size);
    using mem = Memory<s, r1, r2, scale, disp>;
    using imm = Immediate<imms, x, is_var>;

    // none
    if constexpr (r2::index % 8 == 0b100) {
        return Bytes<ByteArray<>, FlagArray<>>{};
    }


    if constexpr (r1::index % 8 == 0b100)
    // [base + index * scale] + disp8/32
    {
	return modrm<disp::mode, digit, 0b100>() + sib(mem{}) + to_bytes(disp{});
    }
    // [esp + index * scale] + disp8/32
    else if constexpr (disp::mode == 0b00 && r1::index % 8 == 0b101)
    {
	return modrm<digit>(Memory<s, r1, r2, scale, Disp8<0x0, false>>{}, imm{});
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
    if constexpr (r1::index % 8 == 0b100)
    // [base] + disp8/32
    {
    }
    else if constexpr (disp::mode == 0b00 && r1::index % 8 == 0b1001)
    // FIXME: put disp32 other place
    {

    }
    else
    {
	return modrm<disp::mode, reg::index % 8, r1::index % 8>() + to_bytes(disp{});
    }
}

//! mem <--> reg
template <size_t memsize, typename r1, typename r2, typename scale, typename disp, size_t regsize, size_t i>
constexpr auto modrm (Memory<memsize, r1, r2, scale, disp>, Register<regsize, i>)
{
    using mem = Memory<memsize, r1, r2, scale, disp>;
    using reg = Register<regsize, i>;
    if constexpr (r1::index % 8 == 0b100)
    {
	// NOTE: not support!
    }
    else if constexpr (r2::index % 8 == 0b101)
    {
	// NOTE: not support!
    }
    else {
	return modrm<disp::mode, reg::index, 0b100>() + sib(mem{}) + to_bytes(disp{});
    }
}

#endif //INSTRUCTION_FORMAT_H
