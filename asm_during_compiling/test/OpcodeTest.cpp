#include "catch.hpp"
#include "lxasm.h"

namespace Catch {
    template <uint8_t... bytes, bool... is_vars, typename... Args>
    struct StringMaker< Bytes<ByteArray<bytes...>, FlagArray<is_vars...>, Args...> > {
        static std::string convert( Bytes<ByteArray<bytes...>, FlagArray<is_vars...>, Args...> const& v ) {
	return ::Catch::Detail::arrayRangeToString<v.size>(v.data.begin(), v.data.end());
        }
    };
}

TEST_CASE("various opcodes", "[opcode]") {

    SECTION("ADD") {
	REQUIRE(ADD(bl, Imm8<0x12>)					 == BYTES(0x80, 0xc3, 0x12));
	REQUIRE(ADD(b_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)   	 == BYTES(0x80, 0x44, 0x6c, 0x7f, 0x12));
	REQUIRE(ADD(bx, Imm8<0x12>)				    	 == BYTES(0x66, 0x83, 0xc3, 0x12));
	REQUIRE(ADD(w_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)   	 == BYTES(0x66, 0x83, 0x44, 0x6c, 0x7f, 0x12));
	REQUIRE(ADD(bx, Imm16<0x1234>)				    	 == BYTES(0x66, 0x81, 0xc3, 0x34, 0x12));
	REQUIRE(ADD(w_[rsp + rbp * 2_x + Imm8<0x7f>], Imm16<0x1234>)	 == BYTES(0x66, 0x81, 0x44, 0x6c, 0x7f, 0x34, 0x12));
	REQUIRE(ADD(ebx, Imm8<0x12>)					 == BYTES(0x83, 0xc3, 0x12));
	REQUIRE(ADD(d_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)	 == BYTES(0x83, 0x44, 0x6c, 0x7f, 0x12));
	REQUIRE(ADD(ebx, Imm32<0x12345678>)				 == BYTES(0x81, 0xc3, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(d_[rsp + rbp * 2_x + Imm8<0x7f>], Imm32<0x12345678>) == BYTES(0x81, 0x44, 0x6c, 0x7f, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(rbx, Imm8<0x12>)					 == BYTES(0x48, 0x83, 0xc3, 0x12));
	REQUIRE(ADD(q_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)	 == BYTES(0x48, 0x83, 0x44, 0x6c, 0x7f, 0x12));
	REQUIRE(ADD(rbx, Imm32<0x12345678>)				 == BYTES(0x48, 0x81, 0xc3, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(q_[rsp + rbp * 2_x + Imm8<0x7f>], Imm32<0x12345678>) == BYTES(0x48, 0x81, 0x44, 0x6c, 0x7f, 0x78, 0x56, 0x34, 0x12));
    }

    SECTION("CMP") {
	REQUIRE(CMP(bl, Imm8<0x12>)					 == BYTES(0x80, 0xfb, 0x12));
	REQUIRE(CMP(b_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)   	 == BYTES(0x80, 0x7c, 0x6c, 0x7f, 0x12));
	REQUIRE(CMP(bx, Imm8<0x12>)				    	 == BYTES(0x66, 0x83, 0xfb, 0x12));
	REQUIRE(CMP(w_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)   	 == BYTES(0x66, 0x83, 0x7c, 0x6c, 0x7f, 0x12));
	REQUIRE(CMP(bx, Imm16<0x1234>)				    	 == BYTES(0x66, 0x81, 0xfb, 0x34, 0x12));
	REQUIRE(CMP(w_[rsp + rbp * 2_x + Imm8<0x7f>], Imm16<0x1234>)	 == BYTES(0x66, 0x81, 0x7c, 0x6c, 0x7f, 0x34, 0x12));
	REQUIRE(CMP(ebx, Imm8<0x12>)					 == BYTES(0x83, 0xfb, 0x12));
	REQUIRE(CMP(d_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)	 == BYTES(0x83, 0x7c, 0x6c, 0x7f, 0x12));
	REQUIRE(CMP(ebx, Imm32<0x12345678>)				 == BYTES(0x81, 0xfb, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(CMP(d_[rsp + rbp * 2_x + Imm8<0x7f>], Imm32<0x12345678>) == BYTES(0x81, 0x7c, 0x6c, 0x7f, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(CMP(rbx, Imm8<0x12>)					 == BYTES(0x48, 0x83, 0xfb, 0x12));
	REQUIRE(CMP(q_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)	 == BYTES(0x48, 0x83, 0x7c, 0x6c, 0x7f, 0x12));
	REQUIRE(CMP(rbx, Imm32<0x12345678>)				 == BYTES(0x48, 0x81, 0xfb, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(CMP(q_[rsp + rbp * 2_x + Imm8<0x7f>], Imm32<0x12345678>) == BYTES(0x48, 0x81, 0x7c, 0x6c, 0x7f, 0x78, 0x56, 0x34, 0x12));
    }

    SECTION("DEC") {
	REQUIRE(DEC(bl)  == BYTES(0xfe, 0xcb));
	REQUIRE(DEC(bx)  == BYTES(0x66, 0xff, 0xcb));
	REQUIRE(DEC(ebx) == BYTES(0xff, 0xcb));	
	REQUIRE(DEC(rbx) == BYTES(0x48, 0xff, 0xcb));
	REQUIRE(DEC(r12) == BYTES(0x49, 0xff, 0xcc));
	REQUIRE(DEC(b_[rsp + rbp * 2_x + Imm8<0x7f>]) == BYTES(0xfe, 0x4c, 0x6c, 0x7f));
	REQUIRE(DEC(w_[rsp + rbp * 2_x + Imm8<0x7f>]) == BYTES(0x66, 0xff, 0x4c, 0x6c, 0x7f));
	REQUIRE(DEC(d_[rsp + rbp * 2_x + Imm8<0x7f>]) == BYTES(0xff, 0x4c, 0x6c, 0x7f));
	REQUIRE(DEC(q_[rsp + rbp * 2_x + Imm8<0x7f>]) == BYTES(0x48, 0xff, 0x4c, 0x6c, 0x7f));
    }

    SECTION("INC") {
	REQUIRE(INC(bl)  == BYTES(0xfe, 0xc3));
	REQUIRE(INC(bx)  == BYTES(0x66, 0xff, 0xc3));
	REQUIRE(INC(ebx) == BYTES(0xff, 0xc3));	
	REQUIRE(INC(rbx) == BYTES(0x48, 0xff, 0xc3));
	REQUIRE(INC(r12) == BYTES(0x49, 0xff, 0xc4));
	REQUIRE(INC(b_[rsp + rbp * 2_x + Imm8<0x7f>]) == BYTES(0xfe, 0x44, 0x6c, 0x7f));
	REQUIRE(INC(w_[rsp + rbp * 2_x + Imm8<0x7f>]) == BYTES(0x66, 0xff, 0x44, 0x6c, 0x7f));
	REQUIRE(INC(d_[rsp + rbp * 2_x + Imm8<0x7f>]) == BYTES(0xff, 0x44, 0x6c, 0x7f));
	REQUIRE(INC(q_[rsp + rbp * 2_x + Imm8<0x7f>]) == BYTES(0x48, 0xff, 0x44, 0x6c, 0x7f));
    }

    SECTION("JE") {
    }

    SECTION("JNE") {
    }

    SECTION("MOV") {
    }

    SECTION("SUB") {
	REQUIRE(SUB(bl, Imm8<0x12>)					 == BYTES(0x80, 0xeb, 0x12));
	REQUIRE(SUB(b_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)   	 == BYTES(0x80, 0x6c, 0x6c, 0x7f, 0x12));
	REQUIRE(SUB(bx, Imm8<0x12>)				    	 == BYTES(0x66, 0x83, 0xeb, 0x12));
	REQUIRE(SUB(w_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)   	 == BYTES(0x66, 0x83, 0x6c, 0x6c, 0x7f, 0x12));
	REQUIRE(SUB(bx, Imm16<0x1234>)				    	 == BYTES(0x66, 0x81, 0xeb, 0x34, 0x12));
	REQUIRE(SUB(w_[rsp + rbp * 2_x + Imm8<0x7f>], Imm16<0x1234>)	 == BYTES(0x66, 0x81, 0x6c, 0x6c, 0x7f, 0x34, 0x12));
	REQUIRE(SUB(ebx, Imm8<0x12>)					 == BYTES(0x83, 0xeb, 0x12));
	REQUIRE(SUB(d_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)	 == BYTES(0x83, 0x6c, 0x6c, 0x7f, 0x12));
	REQUIRE(SUB(ebx, Imm32<0x12345678>)				 == BYTES(0x81, 0xeb, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(SUB(d_[rsp + rbp * 2_x + Imm8<0x7f>], Imm32<0x12345678>) == BYTES(0x81, 0x6c, 0x6c, 0x7f, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(SUB(rbx, Imm8<0x12>)					 == BYTES(0x48, 0x83, 0xeb, 0x12));
	REQUIRE(SUB(q_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)	 == BYTES(0x48, 0x83, 0x6c, 0x6c, 0x7f, 0x12));
	REQUIRE(SUB(rbx, Imm32<0x12345678>)				 == BYTES(0x48, 0x81, 0xeb, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(SUB(q_[rsp + rbp * 2_x + Imm8<0x7f>], Imm32<0x12345678>) == BYTES(0x48, 0x81, 0x6c, 0x6c, 0x7f, 0x78, 0x56, 0x34, 0x12));
    }

    SECTION("SYSCALL") {
	REQUIRE(SYSCALL() == BYTES(0x0f, 0x05));
    }

    SECTION("XOR") {
	REQUIRE(XOR(bl, Imm8<0x12>)					 == BYTES(0x80, 0xf3, 0x12));
	REQUIRE(XOR(b_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)   	 == BYTES(0x80, 0x74, 0x6c, 0x7f, 0x12));
	REQUIRE(XOR(bx, Imm8<0x12>)				    	 == BYTES(0x66, 0x83, 0xf3, 0x12));
	REQUIRE(XOR(w_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)   	 == BYTES(0x66, 0x83, 0x74, 0x6c, 0x7f, 0x12));
	REQUIRE(XOR(bx, Imm16<0x1234>)				    	 == BYTES(0x66, 0x81, 0xf3, 0x34, 0x12));
	REQUIRE(XOR(w_[rsp + rbp * 2_x + Imm8<0x7f>], Imm16<0x1234>)	 == BYTES(0x66, 0x81, 0x74, 0x6c, 0x7f, 0x34, 0x12));
	REQUIRE(XOR(ebx, Imm8<0x12>)					 == BYTES(0x83, 0xf3, 0x12));
	REQUIRE(XOR(d_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)	 == BYTES(0x83, 0x74, 0x6c, 0x7f, 0x12));
	REQUIRE(XOR(ebx, Imm32<0x12345678>)				 == BYTES(0x81, 0xf3, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(XOR(d_[rsp + rbp * 2_x + Imm8<0x7f>], Imm32<0x12345678>) == BYTES(0x81, 0x74, 0x6c, 0x7f, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(XOR(rbx, Imm8<0x12>)					 == BYTES(0x48, 0x83, 0xf3, 0x12));
	REQUIRE(XOR(q_[rsp + rbp * 2_x + Imm8<0x7f>], Imm8<0x12>)	 == BYTES(0x48, 0x83, 0x74, 0x6c, 0x7f, 0x12));
	REQUIRE(XOR(rbx, Imm32<0x12345678>)				 == BYTES(0x48, 0x81, 0xf3, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(XOR(q_[rsp + rbp * 2_x + Imm8<0x7f>], Imm32<0x12345678>) == BYTES(0x48, 0x81, 0x74, 0x6c, 0x7f, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(XOR(bl, cl)						 == BYTES(0x30, 0xcb));
	REQUIRE(XOR(b_[rsp + rbp * 2_x + Imm8<0x7f>], bl)  		 == BYTES(0x30, 0x5c, 0x6c, 0x7f));
	REQUIRE(XOR(bx, cx)				   		 == BYTES(0x66, 0x31, 0xcb));
	REQUIRE(XOR(w_[rsp + rbp * 2_x + Imm8<0x7f>], bx)  		 == BYTES(0x66, 0x31, 0x5c, 0x6c, 0x7f));
	REQUIRE(XOR(ebx, ecx)				   		 == BYTES(0x31, 0xcb));
	REQUIRE(XOR(d_[rsp + rbp * 2_x + Imm8<0x7f>], ebx) 		 == BYTES(0x31, 0x5c, 0x6c, 0x7f));
	REQUIRE(XOR(rbx, rcx)				   		 == BYTES(0x48, 0x31, 0xcb));
	REQUIRE(XOR(q_[rsp + rbp * 2_x + Imm8<0x7f>], rbx) 		 == BYTES(0x48, 0x31, 0x5c, 0x6c, 0x7f));
	REQUIRE(XOR(bl, b_[rsp + rbp * 2_x + Imm8<0x7f>])  		 == BYTES(0x32, 0x5c, 0x6c, 0x7f));
	REQUIRE(XOR(bx, w_[rsp + rbp * 2_x + Imm8<0x7f>])  		 == BYTES(0x66, 0x33, 0x5c, 0x6c, 0x7f));
	REQUIRE(XOR(ebx, d_[rsp + rbp * 2_x + Imm8<0x7f>]) 		 == BYTES(0x33, 0x5c, 0x6c, 0x7f));
	REQUIRE(XOR(rbx, q_[rsp + rbp * 2_x + Imm8<0x7f>]) 		 == BYTES(0x48, 0x33, 0x5c, 0x6c, 0x7f));
    }
}
