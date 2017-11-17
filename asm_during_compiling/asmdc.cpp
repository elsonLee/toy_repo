#include <iostream>
#include <array>
#include <tuple>
#include <cstdio>
#include <cassert>

//! Immediate
template <size_t s> struct Immediate_type;
template <> struct Immediate_type<1> { using type = int8_t; };
template <> struct Immediate_type<2> { using type = int16_t; };
template <> struct Immediate_type<4> { using type = int32_t; };
template <> struct Immediate_type<8> { using type = int64_t; };

template <size_t s, typename Immediate_type<s>::type x, bool is_var = false>
struct Immediate {
    using type = decltype(x);
    static constexpr size_t size = sizeof(type);
    static constexpr type value = x;
    static constexpr bool is_variable = is_var;
};

template <Immediate_type<1>::type x> constexpr Immediate<1, x, false> Imm8{};
template <Immediate_type<2>::type x> constexpr Immediate<2, x, false> Imm16{};
template <Immediate_type<4>::type x> constexpr Immediate<4, x, false> Imm32{};
template <Immediate_type<8>::type x> constexpr Immediate<8, x, false> Imm64{};
constexpr auto VarImm8  = Immediate<1, 0x0, true>{};
constexpr auto VarImm16 = Immediate<2, 0x0, true>{};
constexpr auto VarImm32 = Immediate<4, 0x0, true>{};
constexpr auto VarImm64 = Immediate<8, 0x0, true>{};

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

using NoReg  = Register<0, 0>;
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

//! Memory
template <size_t size> struct Displacement_type;
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

template <size_t _size, typename Displacement_type<_size>::type x, bool is_var = false>
struct Displacement {
    using type = typename Displacement_type<_size>::type;
    constexpr static type val = x;
    constexpr static size_t size = _size; 
    constexpr static uint8_t mode = Displacement_type<_size>::mode;
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

using NoScale =  Scale<1>;
using Scale2  = Scale<2>;
using Scale4  = Scale<4>;
using Scale8  = Scale<8>;

template <size_t s, typename reg1, typename reg2, typename scale, typename disp>
struct Memory {
    static constexpr size_t size = s;
};

template <typename reg1, typename reg2, typename scale, typename disp>
using Mem8  = Memory<1, reg1, reg2, scale, disp>;
template <typename reg1, typename reg2, typename scale, typename disp>
using Mem16 = Memory<2, reg1, reg2, scale, disp>;
template <typename reg1, typename reg2, typename scale, typename disp>
using Mem32 = Memory<4, reg1, reg2, scale, disp>;
template <typename reg1, typename reg2, typename scale, typename disp>
using Mem64 = Memory<8, reg1, reg2, scale, disp>;

template <size_t i> using Reg16 = Register<2, i>;
template <size_t i> using Reg32 = Register<4, i>;
template <size_t i> using Reg64 = Register<8, i>;

template <size_t s, typename reg1, typename reg2, typename scale, typename disp>
constexpr auto is_m8 (Memory<s, reg1, reg2, scale, disp> mem)  { return mem.size == 1; }

template <size_t s, typename reg1, typename reg2, typename scale, typename disp>
constexpr auto is_m16 (Memory<s, reg1, reg2, scale, disp> mem) { return mem.size == 2; }

template <size_t s, typename reg1, typename reg2, typename scale, typename disp>
constexpr auto is_m32 (Memory<s, reg1, reg2, scale, disp> mem) { return mem.size == 4; }

template <size_t s, typename reg1, typename reg2, typename scale, typename disp>
constexpr auto is_m64 (Memory<s, reg1, reg2, scale, disp> mem) { return mem.size == 8; }

//! Bytes
template <uint8_t... bytes>
struct ByteArray;

template <bool... bytes>
struct FlagArray {};

template <bool... lbytes, bool... rbytes>
constexpr auto operator+ (FlagArray<lbytes...>, FlagArray<rbytes...>) {
    return FlagArray<lbytes..., rbytes...>{};
}

template <bool flag, size_t size>
constexpr auto gen_flag_array ()
{
    if constexpr (size == 1) {
	return FlagArray<flag>{};
    } else {
	return FlagArray<flag>{} + gen_flag_array<flag, size-1>();
    }
};

template <typename T, typename U, typename... Args>
struct Bytes;

template <uint8_t... byte, bool... is_variable, typename... Args>
struct Bytes<ByteArray<byte...>, FlagArray<is_variable...>, Args...> {
    static_assert(sizeof...(byte) == sizeof...(is_variable));
    constexpr static size_t size = sizeof...(byte);
    constexpr static std::array<uint8_t, sizeof...(byte)> data = { byte... };
    constexpr static std::array<bool, sizeof...(is_variable)> flag = { is_variable... };

    static void print () {
	printf("code: ");
	for (auto i = 0u; i < data.size(); ++i) {
	    printf("%02x ", data[i]);
	}
	printf("\n");
	//printf("flag: ");
	//for (auto i = 0u; i < flag.size(); ++i) {
	//    printf("%2c ", flag[i]? 'T' : 'F');
	//}
	//printf("\n");
    }

    static void write (Args... args)
    {
	printf("code: ");
	write_internal(0, args...);
    }

    private:
    template <typename Arg_1st, typename... Arg_rem>
    static void write_internal (uint8_t idx, Arg_1st arg, Arg_rem... args)
    {
	assert(idx < size);
	if (flag[idx] == true) {
	    for (auto i = 0u; i < sizeof(arg); ++i) {
		assert(flag[idx] == true);
		printf("%02x ", arg & 0xFF);
		arg = arg >> 8;
		++idx;
	    }
	    write_internal(idx, args...);
	} else {
	    printf("%02x ", data[idx]);
	    write_internal(idx + 1, arg, args...);
	}
    }

    static void write_internal (uint8_t idx)
    {
	for (auto i = idx; i < size; i++) {
	    printf("%02x ", data[i]);
	}
	printf("\n");
	return;
    }
};

template <>
struct Bytes<void, void> {};

template <typename Arg, uint8_t... ls, bool... lfs>
constexpr auto mark_arg (Bytes<ByteArray<ls...>, FlagArray<lfs...>>)
{
    return Bytes<ByteArray<ls...>, FlagArray<lfs...>, Arg>{};
}

template <uint8_t... ls, uint8_t... rs, bool... lis_variable, bool... ris_variable, typename... largs, typename... rargs>
constexpr auto operator+ (Bytes<ByteArray<ls...>, FlagArray<lis_variable...>, largs...>,
			  Bytes<ByteArray<rs...>, FlagArray<ris_variable...>, rargs...>)
{
    return Bytes<ByteArray<ls..., rs...>, FlagArray<lis_variable..., ris_variable...>, largs..., rargs...>{};
}

template <uint8_t... ls, bool... lfs, typename... largs>
constexpr auto operator+ (Bytes<ByteArray<ls...>, FlagArray<lfs...>, largs...>,
			  Bytes<void, void>)
{
    return Bytes<ByteArray<ls...>, FlagArray<lfs...>, largs...>{};
}

template <uint8_t... rs, bool... rfs, typename... rargs>
constexpr auto operator+ (Bytes<void, void>,
			  Bytes<ByteArray<rs...>, FlagArray<rfs...>, rargs...>)
{
    return Bytes<ByteArray<rs...>, FlagArray<rfs...>, rargs...>{};
}

template <uint8_t l, uint8_t... ls, bool lf, bool... lfs,
	  uint8_t r, uint8_t... rs, bool rf, bool... rfs>
constexpr auto operator== (Bytes<ByteArray<l, ls...>, FlagArray<lf, lfs...>>,
			   Bytes<ByteArray<r, rs...>, FlagArray<rf, rfs...>>)
{
    return (l == r) && (Bytes<ByteArray<ls...>, FlagArray<lfs...>>{} == Bytes<ByteArray<rs...>, FlagArray<rfs...>>{});
}

constexpr auto operator== (Bytes<void, void>, Bytes<void, void>)
{
    return true;
}

template <uint8_t... rs, bool... rfs>
constexpr auto operator== (Bytes<void, void>, Bytes<ByteArray<rs...>, FlagArray<rfs...>>)
{
    return false;
}

template <uint8_t... ls, bool... lfs>
constexpr auto operator== (Bytes<ByteArray<ls...>, FlagArray<lfs...>>, Bytes<void, void>)
{
    return false;
}

template <size_t size, long long x, bool flag = false>
constexpr auto to_bytes ()
{
    if constexpr (size <= 0) {
	return Bytes<void, void>{};
    }
    else {
	return Bytes<ByteArray<x & 0xFF>, FlagArray<flag>>{} + to_bytes<size - 1, (x >> 8), flag>();
    }
}

template <size_t s, typename Immediate_type<s>::type x, bool is_var>
constexpr auto to_bytes (Immediate<s, x, is_var>)
{
    if constexpr (is_var) {
	using ArgType = typename Immediate<s, x, is_var>::type;
    	return mark_arg<ArgType>(to_bytes<Immediate<s, x, is_var>::size, x, true>());
    } else {
    	return to_bytes<Immediate<s, x, is_var>::size, x, false>();
    }
}

template <size_t size, typename Displacement_type<size>::type x, bool is_var>
constexpr auto to_bytes (Displacement<size, x, is_var>)
{
    using disp = Displacement<size, x, is_var>;
    if constexpr (is_var) {
	return mark_arg<typename Displacement<size, x, is_var>::type>(to_bytes<disp::size, x, true>());
    } else {
	return to_bytes<disp::size, x, false>();
    }
}

//! aux
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


//! Prefix
template <uint8_t... code>
constexpr auto prefix () { return Bytes<ByteArray<code...>, decltype(gen_flag_array<false, sizeof...(code)>())>{}; }

template <uint8_t w, uint8_t r, uint8_t x, uint8_t b>
constexpr auto rex ()
{ 
    constexpr auto c = w | r | x | b;
    static_assert(c <= 0b1);
    if constexpr (c == 0) {
	return Bytes<void, void>{};
    } else {
	return to_bytes<1, (0b0100 << 4) + (w << 3) + (r << 2) + (x << 1) + b>();
    }
}

// Figure 2-4.
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

// Figure 2-5.
template <uint8_t w, size_t s1, size_t i1, size_t s2, size_t i2>
constexpr auto rex (Register<s1, i1>, Register<s2, i2>)
{
    using r1 = Register<s1, i1>;
    using r2 = Register<s2, i2>;
    return rex<w, get_rex_r(r2{}), 0, get_rex_b(r1{})>();
}

// Figure 2-6.
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
constexpr auto opcode () { return Bytes<ByteArray<code...>, decltype(gen_flag_array<false, sizeof...(code)>())>{}; }

template <uint8_t mode, uint8_t reg, uint8_t rm>
constexpr auto modrm ()
{
    static_assert(mode | 0b11 == 0b11);
    static_assert(reg | 0b111 == 0b111);
    static_assert(rm | 0b111 == 0b111);
    return to_bytes<1, (mode << 6) + (reg << 3) + rm>();
}

template <uint8_t scale, uint8_t index, uint8_t base>
constexpr auto sib ()
{
    static_assert(scale | 0b11 == 0b11);
    static_assert(index | 0b111 == 0b111);
    static_assert(base | 0b111 == 0b111);
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

//! operators
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

    template <T sum, char x, char... xs>
    static constexpr T fold () {
	if constexpr (x == '+') {
	    return fold<sum, xs...>();
	} else if constexpr (x == '-') {
	    return -fold<sum, xs...>();
	} else {
	    return fold<10 * sum + (x - '0'), xs...>();
	}
    }

    template <T sum>
    static constexpr T fold () {
	return sum;
    }
};

_<1, int8_t > b_{};
_<2, int16_t> w_{};
_<4, int32_t> d_{};
_<8, int64_t> q_{};

template <char... xs>
constexpr auto operator""_b ()
{
    return Imm8<b_.fold<0, xs...>()>;
}


// TODO
template <size_t s, size_t i,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto operator+ (Register<s, i>, Immediate<imms, x, is_var>)
{
    using r = Register<s, i>;
    using imm = Immediate<imms, x, is_var>;
    return Memory<s, r, NoReg, NoScale, Displacement<imms, x, is_var>>{};
}

template <size_t s, size_t i,
	  size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto operator* (Register<s, i>, Immediate<imms, x, is_var>)
{
    using reg = Register<s, i>;
    return Memory<s, NoReg, reg, Scale<x>, NoDisp>{};
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
    using mem = Memory<s2, NoReg, r2, scale, NoDisp>;
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
	// TODO: not support!
    }
    else if constexpr (r2::index == 0b101)
    {
	// TODO: not support!
    }
    else {
	return modrm<disp::mode, reg::index, 0b100>() + sib(mem{}) + to_bytes(disp{});
    }
}

//! Instructions
// ADD r8 , imm8  -- 80 /0 ib
// ADD r16, imm8  -- 83 /0 ib
// ADD r16, imm16 -- 81 /0 iw
// ADD r32, imm8  -- 83 /0 ib
// ADD r32, imm32 -- 81 /0 id
// ADD r64, imm8  -- REX.W + 83 /0 ib
// ADD r64, imm32 -- REX.W + 81 /0 id
template <size_t s, size_t i, size_t imms, typename Immediate_type<imms>::type x, bool is_var>
constexpr auto ADD (Register<s, i> reg, Immediate<imms, x, is_var> imm)
{
    if constexpr (is_r8(reg) && is_imm8(imm))
    {
	return rex<0>(reg) + opcode<'\x80'>() + modrm<0>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r16(reg) && is_imm8(imm))
    {
	return rex<0>(reg) + prefix<'\x66'>() + opcode<'\x83'>() + modrm<0>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r16(reg) && is_imm16(imm))
    {
	return prefix<'\x66'>() + rex<0>(reg) + opcode<'\x81'>() + modrm<0>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r32(reg) && is_imm8(imm))
    {
	return rex<0>(reg) + opcode<'\x83'>() + modrm<0>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r32(reg) && is_imm32(imm))
    {
	return rex<0>(reg) + opcode<'\x81'>() + modrm<0>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r64(reg) && is_imm8(imm))
    {
	return rex<1>(reg) + opcode<'\x83'>() + modrm<0>(reg, imm) + to_bytes(imm);
    }
    else if constexpr (is_r64(reg) && is_imm32(imm))
    {
	return rex<1>(reg) + opcode<'\x81'>() + modrm<0>(reg, imm) + to_bytes(imm);
    } 
    else {
	//return Bytes<void, void>{};
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
    if constexpr (is_r16(reg1) && is_r16(reg2))
    {
	return rex<0>(reg1, reg2) + prefix<'\x66'>() + opcode<'\x01'>() + modrm(reg1, reg2);
    }
    if constexpr (is_r32(reg1) && is_r32(reg2))
    {
	return rex<0>(reg1, reg2) + opcode<'\x01'>() + modrm(reg1, reg2);
    }
    else {
	//return Bytes<void, void>{};
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
    if constexpr (is_m16(mem) && is_imm8(imm))
    {
	return prefix<'\x66'>() +
	       rex<0, 0>(mem, imm) + opcode<'\x83'>() + modrm<0>(mem, imm) + to_bytes(imm);
    } 
    if constexpr (is_m16(mem) && is_imm16(imm))
    {
	return prefix<'\x66'>() +
	       rex<0, 0>(mem, imm) + opcode<'\x81'>() + modrm<0>(mem, imm) + to_bytes(imm);
    } 
    if constexpr (is_m32(mem) && is_imm8(imm))
    {
	return rex<0, 0>(mem, imm) + opcode<'\x83'>() + modrm<0>(mem, imm) + to_bytes(imm);
    } 
    if constexpr (is_m32(mem) && is_imm32(imm))
    {
	return rex<0, 0>(mem, imm) + opcode<'\x81'>() + modrm<0>(mem, imm) + to_bytes(imm);
    } 
    if constexpr (is_m64(mem) && is_imm8(imm))
    {
	return rex<1, 0>(mem, imm) + opcode<'\x83'>() + modrm<0>(mem, imm) + to_bytes(imm);
    } 
    if constexpr (is_m64(mem) && is_imm32(imm))
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
template <size_t memsize, typename r1, typename r2,  typename scale, typename disp, size_t regsize, size_t i>
constexpr auto ADD (Memory<memsize, r1, r2, scale, disp> mem, Register<regsize, i> reg)
{
    if constexpr (is_m8(mem) && is_r8(reg))
    {
	return rex<0>(mem, reg) + opcode<'\x00'>() + modrm(mem, reg);
    } 
    if constexpr (is_m16(mem) && is_r16(reg))
    {
	return prefix<'\x66'>() +
	       rex<0>(mem, reg) + opcode<'\x01'>() + modrm(mem, reg);
    } 
    if constexpr (is_m32(mem) && is_r32(reg))
    {
	return rex<0>(mem, reg) + opcode<'\x01'>() + modrm(mem, reg);
    } 
    if constexpr (is_m64(mem) && is_r64(reg))
    {
	return rex<1>(mem, reg) + opcode<'\x01'>() + modrm(mem, reg);
    } 
    else {

    }
}

int main ()
{
#if 0
    static_assert(ADD(al,   Imm8<0x12>)		== Bytes<0x80, 0xc0, 0x12>{});
    static_assert(ADD(bx,   Imm8<0x12>)	   	== Bytes<0x66, 0x83, 0xc3, 0x12>{});
    static_assert(ADD(cx,   Imm16<0x1234>)	== Bytes<0x66, 0x81, 0xc1, 0x34, 0x12>{});
    static_assert(ADD(edx,  Imm32<0x12345678>)	== Bytes<0x81, 0xc2, 0x78, 0x56, 0x34, 0x12>{});
    static_assert(ADD(rsi,  Imm8<0x12>)		== Bytes<0x48, 0x83, 0xc6, 0x12>{});
    static_assert(ADD(r11,  Imm8<0x12>) 	== Bytes<0x49, 0x83, 0xc3, 0x12>{});
    static_assert(ADD(rdi,  Imm32<0x12345678>)  == Bytes<0x48, 0x81, 0xc7, 0x78, 0x56, 0x34, 0x12>{});

    static_assert(ADD(bl,  ah)	== Bytes<0x00, 0xe3>{});
    static_assert(ADD(sp,  di)	== Bytes<0x66, 0x01, 0xfc>{});
    static_assert(ADD(esp, edi)	== Bytes<0x01, 0xfc>{});

    constexpr uint8_t a = 0x12;
    static_assert(ADD(al,  Imm8<a>) == Bytes<0x80, 0xc0, 0x12>{});

    static_assert(ADD(b_[al],  Imm8<0x12>)				    == Bytes<0x80, 0x00, 0x12>{});
    static_assert(ADD(b_[rax + Imm8<0x00>], Imm8<0x12>)			    == Bytes<0x80, 0x40, 0x00, 0x12>{});
    static_assert(ADD(b_[rax + Imm8<0x34>], Imm8<0x12>)	    		    == Bytes<0x80, 0x40, 0x34, 0x12>{});
    static_assert(ADD(b_[rax + Imm32<0x12345678>], Imm8<0x12>)  	    == Bytes<0x80, 0x80, 0x78, 0x56, 0x34, 0x12, 0x12>{});
    static_assert(ADD(b_[r8  + Imm8<0x34>], Imm8<0x12>)			    == Bytes<0x41, 0x80, 0x40, 0x34, 0x12>{});
    static_assert(ADD(w_[rax + Imm8<0x34>], Imm8<0x12>) 		    == Bytes<0x66, 0x83, 0x40, 0x34, 0x12>{});
    static_assert(ADD(w_[r9  + Imm8<0x34>], Imm8<0x12>) 		    == Bytes<0x66, 0x41, 0x83, 0x41, 0x34, 0x12>{});
    static_assert(ADD(w_[rax + Imm8<0x34>], Imm16<0x1234>)      	    == Bytes<0x66, 0x81, 0x40, 0x34, 0x34, 0x12>{});
    static_assert(ADD(w_[r14 + Imm8<0x34>], Imm16<0x1234>)      	    == Bytes<0x66, 0x41, 0x81, 0x46, 0x34, 0x34, 0x12>{});
    static_assert(ADD(d_[rax + Imm8<0x34>], Imm8<0x12>)			    == Bytes<0x83, 0x40, 0x34, 0x12>{});
    static_assert(ADD(d_[r10 + Imm8<0x34>], Imm8<0x12>)			    == Bytes<0x41, 0x83, 0x42, 0x34, 0x12>{});
    static_assert(ADD(d_[r15 + Imm8<0x34>], Imm32<0x12345678>)  	    == Bytes<0x41, 0x81, 0x47, 0x34, 0x78, 0x56, 0x34, 0x12>{});
    static_assert(ADD(q_[rax + Imm8<0x34>], Imm8<0x12>)			    == Bytes<0x48, 0x83, 0x40, 0x34, 0x12>{});
    static_assert(ADD(q_[r11 + Imm8<0x34>], Imm8<0x12>)			    == Bytes<0x49, 0x83, 0x43, 0x34, 0x12>{});
    static_assert(ADD(q_[rdx + Imm8<0x34>], Imm32<0x12345678>)  	    == Bytes<0x48, 0x81, 0x42, 0x34, 0x78, 0x56, 0x34, 0x12>{});
    static_assert(ADD(w_[rax + rcx + Imm8<0x34>], Imm8<0x12>)   	    == Bytes<0x66, 0x83, 0x44, 0x08, 0x34, 0x12>{});
    static_assert(ADD(w_[rax + rcx * Imm8<2> + Imm8<0x34>], Imm8<0x12>)	    == Bytes<0x66, 0x83, 0x44, 0x48, 0x34, 0x12>{});
    static_assert(ADD(w_[rax + rcx * Imm8<4> + Imm8<0x34>], Imm8<0x12>)     == Bytes<0x66, 0x83, 0x44, 0x88, 0x34, 0x12>{});
    static_assert(ADD(w_[rax + rcx * Imm8<8> + Imm8<0x34>], Imm8<0x12>)     == Bytes<0x66, 0x83, 0x44, 0xc8, 0x34, 0x12>{});

    static_assert(ADD(b_[rax + Imm8<0x34>], al)	    == Bytes<0x00, 0x40, 0x34>{});
    static_assert(ADD(w_[rax + Imm8<0x34>], ax)	    == Bytes<0x66, 0x01, 0x40, 0x34>{});
    static_assert(ADD(d_[rdx + Imm8<0x34>], ebx)    == Bytes<0x01, 0x5a, 0x34>{});
    static_assert(ADD(q_[rdx + Imm32<0x34>], rcx)   == Bytes<0x48, 0x01, 0x8a, 0x34, 0x00, 0x00, 0x00>{});
    static_assert(ADD(q_[r8  + Imm32<0x34>], rcx)   == Bytes<0x49, 0x01, 0x88, 0x34, 0x00, 0x00, 0x00>{});
    static_assert(ADD(q_[r8  + Imm32<0x34>], r9)    == Bytes<0x4d, 0x01, 0x88, 0x34, 0x00, 0x00, 0x00>{});
#endif

    // TODO: double check
    //static_assert(ADD(w_[Imm32<0x34>], Imm8<0x12>)  == Bytes<0x66, 0x83, 0x05, 0x34, 0x00, 0x00, 0x00, 0x12>{});

    //{
    //    auto c = ADD(b_[rax + var(Imm32<0x12345678>)], var(Imm8<0x12>));
    //    c.print();
    //    c.write(0x12, 0xcd);
    //    c.write(0x1234, 0xcd);
    //    c.write(0x123456, 0xcd);
    //}

    //{
        auto c = ADD(b_[rax + Imm32<0x12345678>], VarImm8);
        c.print();
        c.write(0xcd);



        ADD(b_[rax + Imm32<0x12345678>], Imm8<0x12>).write();
        ADD(b_[rax + Imm32<0x12345678>], VarImm8).write(0xcd);
    //}

    //{
    //    auto c = ADD(Mem8<rax, NoReg, NoScale, NoDisp>{}, Imm8<0x12>);
    //    for (auto i = 0u; i < c.size; ++i) {
    //        printf("%02x ", c.data[i]);
    //    }
    //    printf("\n");
    //}

    //{
    //    auto c = ADD(Mem64<rdx, NoReg, NoScale, Disp8<0x34>>{}, Imm32<0x12345678>);
    //    for (auto i = 0u; i < c.size; ++i) {
    //        printf("%02x ", c.data[i]);
    //    }
    //    printf("\n");
    //}

    //{
    //    auto c = ADD(Mem16<rax, rcx, NoScale, Disp8<0x34>>{}, Imm8<0x12>);
    //    for (auto i = 0u; i < c.size; ++i) {
    //        printf("%02x ", c.data[i]);
    //    }
    //    printf("\n");
    //}

    //{
    //    auto c = ADD(Mem16<NoReg, NoReg, NoScale, Disp32<0x34>>{}, Imm8<0x12>);
    //    for (auto i = 0u; i < c.size; ++i) {
    //        printf("%02x ", c.data[i]);
    //    }
    //    printf("\n");
    //}

    {
	//auto c = 34_b;
	//for (auto i = 0u; i < c.size; ++i) {
	//    printf("%02x ", c.data[i]);
	//}
	//printf("\n");
    }


    return 0;
}
