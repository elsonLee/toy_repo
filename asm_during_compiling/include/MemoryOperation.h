#ifndef MEMORY_OPERATION_H
#define MEMORY_OPERATION_H

template <size_t size, typename T>
struct _
{
    static_assert(size == 1 || size == 2 || size == 4 || size == 8);
    template <size_t s, size_t i>
    constexpr auto operator[] (Register<s, i>) const
    {
        using reg = Register<s, i>;
        return Memory<size, reg, NoReg, NoScale, NoDisp>{};
    }

    template <size_t s, typename r1, typename r2, typename scale, typename disp>
    constexpr auto operator[] (Memory<s, r1, r2, scale, disp>) {
	using mem = Memory<size, r1, r2, scale, disp>;
	return mem{};
    }

    template <size_t imms, typename Immediate_type<imms>::type x, bool is_var>
    constexpr auto operator[] (Immediate<imms, x, is_var>)
    {
	using disp = Displacement<imms, x, is_var>;
	return Memory<size, NoReg, NoReg, NoScale, disp>{};
    }
};

_<1, int8_t > b_{};
_<2, int16_t> w_{};
_<4, int32_t> d_{};
_<8, int64_t> q_{};

template <char x, char... xs>
constexpr auto get_first_ch ()
{
    return x;
}

template <char... n>
constexpr auto operator""_x ()
{
    static_assert(sizeof...(n) == 1);
    constexpr auto c = get_first_ch<n...>();
    if constexpr (c == '1') {
        return Immediate<1, 1>{};
    } else if constexpr (c == '2') {
        return Immediate<1, 2>{};
    } else  if constexpr (c == '4') {
        return Immediate<1, 4>{};
    } else  if constexpr (c == '8') {
        return Immediate<1, 8>{};
    } else {

    }
}

// TODO
template <size_t s, size_t i,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto operator+ (Register<s, i>, Immediate<imms, x, is_var>)
{
    using r = Register<s, i>;
    return Memory<s, r, NoReg, NoScale, Displacement<imms, x, is_var>>{};
}

// reg * scale
template <size_t s, size_t i,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto operator* (Register<s, i>, Immediate<imms, x, is_var>)
{
    using reg = Register<s, i>;
    return Memory<s, NoReg, reg, Scale<x>, NoDisp>{};
}

template <size_t imms, typename Immediate_type<imms>::type x, bool is_var,
	  size_t s, size_t i>
constexpr auto operator* (Immediate<imms, x, is_var> imm, Register<s, i> reg)
{
    return reg * imm;
}

template <size_t s, typename r1, typename r2, typename scale,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto operator+ (Memory<s, r1, r2, scale, NoDisp>, Immediate<imms, x, is_var>)
{
    using disp = Displacement<imms, x, is_var>;
    return Memory<s, r1, r2, scale, disp>{};
}

// base + index
template <size_t s1, size_t i1, size_t s2, size_t i2>
constexpr auto operator+ (Register<s1, i1>, Register<s2, i2>)
{
    using r1 = Register<s1, i1>;
    using r2 = Register<s2, i2>;
    return Memory<1, r1, r2, NoScale, NoDisp>{};   // memory size is not important here
}

template <size_t s1, size_t i1, size_t s2, typename r2, typename scale>
constexpr auto operator+ (Register<s1, i1>, Memory<s2, NoReg, r2, scale, NoDisp>)
{
    using r1 = Register<s1, i1>;
    return Memory<1, r1, r2, scale, NoDisp>{}; // memory size is not important here
}

//! mem <--> reg
template <size_t memsize, typename r1, typename r2, typename scale, typename disp, size_t regsize, size_t i>
constexpr auto modrm (Memory<memsize, r1, r2, scale, disp>, Register<regsize, i>)
{
    using mem = Memory<memsize, r1, r2, scale, disp>;
    using reg = Register<regsize, i>;
    if constexpr (r1::index == 0b100)
    {
	// NOTE: not support!
    }
    else if constexpr (r2::index == 0b101)
    {
	// NOTE: not support!
    }
    else {
	return modrm<disp::mode, reg::index, 0b100>() + sib(mem{}) + to_bytes(disp{});
    }
}

#endif //MEMORY_OPERATION_H
