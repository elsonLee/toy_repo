#ifndef BYTES_H
#define BYTES_H

template <uint8_t... bytes>
struct ByteArray;

template <bool... bytes>
struct FlagArray {};

template <bool... lbytes, bool... rbytes>
constexpr auto operator+ (FlagArray<lbytes...>, FlagArray<rbytes...>) {
    return FlagArray<lbytes..., rbytes...>{};
}

template <bool flag, size_t size>
constexpr auto make_flag_array ()
{
    if constexpr (size == 1) {
	return FlagArray<flag>{};
    } else {
	return FlagArray<flag>{} + make_flag_array<flag, size-1>();
    }
};

//! Bytes
template <typename T, typename U, typename... Args>
struct Bytes;

template <uint8_t... bytes, bool... is_vars, typename... Args>
struct Bytes<ByteArray<bytes...>, FlagArray<is_vars...>, Args...> {
    static_assert(sizeof...(bytes) == sizeof...(is_vars));
    constexpr static size_t size = sizeof...(bytes);
    constexpr static std::array<uint8_t, size> data = { bytes... };
    constexpr static std::array<bool, size> flag = { is_vars... };

    static void print () {
	//printf("code: ");
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

    static void emit (Args... args)
    {
	printf("code: ");
	emit_internal(0, args...);
    }

    private:
    template <typename Arg_1st, typename... Arg_rem>
	static void emit_internal (uint8_t idx, Arg_1st arg, Arg_rem... args)
	{
	    assert(idx < size);
	    if (flag[idx] == true) {
		for (auto i = 0u; i < sizeof(arg); ++i) {
		    assert(flag[idx] == true);
		    printf("%02x ", arg & 0xFF);
		    arg = arg >> 8;
		    ++idx;
		}
		emit_internal(idx, args...);
	    } else {
		printf("%02x ", data[idx]);
		emit_internal(idx + 1, arg, args...);
	    }
	}

    static void emit_internal (uint8_t idx)
    {
	for (auto i = idx; i < size; i++) {
	    printf("%02x ", data[i]);
	}
	printf("\n");
	return;
    }
};

template <uint8_t... bytes>
constexpr auto make_bytes () {
    return Bytes<ByteArray<bytes...>, decltype(make_flag_array<false, sizeof...(bytes)>())>{};
};

template <typename Arg, uint8_t... ls, bool... lfs>
constexpr auto add_args (Bytes<ByteArray<ls...>, FlagArray<lfs...>>)
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
			  Bytes<ByteArray<>, FlagArray<>>)
{
    return Bytes<ByteArray<ls...>, FlagArray<lfs...>, largs...>{};
}

template <uint8_t... rs, bool... rfs, typename... rargs>
constexpr auto operator+ (Bytes<ByteArray<>, FlagArray<>>,
			  Bytes<ByteArray<rs...>, FlagArray<rfs...>, rargs...>)
{
    return Bytes<ByteArray<rs...>, FlagArray<rfs...>, rargs...>{};
}

constexpr auto operator+ (Bytes<ByteArray<>, FlagArray<>>,
			  Bytes<ByteArray<>, FlagArray<>>)
{
    return Bytes<ByteArray<>, FlagArray<>>{};
}

template <uint8_t l, uint8_t... ls, bool lf, bool... lfs,
	  uint8_t r, uint8_t... rs, bool rf, bool... rfs>
constexpr auto operator== (Bytes<ByteArray<l, ls...>, FlagArray<lf, lfs...>>,
			   Bytes<ByteArray<r, rs...>, FlagArray<rf, rfs...>>)
{
    return (l == r) && (Bytes<ByteArray<ls...>, FlagArray<lfs...>>{} == Bytes<ByteArray<rs...>, FlagArray<rfs...>>{});
}

constexpr auto operator== (Bytes<ByteArray<>, FlagArray<>>,
			   Bytes<ByteArray<>, FlagArray<>>)
{
    return true;
}

template <uint8_t... rs, bool... rfs>
constexpr auto operator== (Bytes<ByteArray<>, FlagArray<>>,
			   Bytes<ByteArray<rs...>, FlagArray<rfs...>>)
{
    return false;
}

template <uint8_t... ls, bool... lfs>
constexpr auto operator== (Bytes<ByteArray<ls...>, FlagArray<lfs...>>,
			   Bytes<ByteArray<>, FlagArray<>>)
{
    return false;
}

template <size_t s, long long x, bool flag = false>
constexpr auto to_bytes ()
{
    if constexpr (s <= 0) {
	return Bytes<ByteArray<>, FlagArray<>>{};
    }
    else {
	return Bytes<ByteArray<x & 0xFF>, FlagArray<flag>>{} + to_bytes<s - 1, (x >> 8), flag>();
    }
}

#endif // BYTES_H
