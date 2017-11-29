#include <stdio.h>
#include <cassert>
#include "lxasm.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define BYTES(args...) (make_bytes<args>())

TEST_CASE("various memory addressing", "[mem addressing]") {

    SECTION("r8, imm8") {
	REQUIRE(ADD(al, Imm8<0x12>) == BYTES(0x80, 0xc0, 0x12));
	REQUIRE(ADD(cl, Imm8<0x12>) == BYTES(0x80, 0xc1, 0x12));
	REQUIRE(ADD(dl, Imm8<0x12>) == BYTES(0x80, 0xc2, 0x12));
	REQUIRE(ADD(bl, Imm8<0x12>) == BYTES(0x80, 0xc3, 0x12));
	REQUIRE(ADD(ah, Imm8<0x12>) == BYTES(0x80, 0xc4, 0x12));
	REQUIRE(ADD(ch, Imm8<0x12>) == BYTES(0x80, 0xc5, 0x12));
	REQUIRE(ADD(dh, Imm8<0x12>) == BYTES(0x80, 0xc6, 0x12));
	REQUIRE(ADD(bh, Imm8<0x12>) == BYTES(0x80, 0xc7, 0x12));
    }

    SECTION("r16, imm8") {
	REQUIRE(ADD(ax, Imm8<0x12>) == BYTES(0x66, 0x83, 0xc0, 0x12));
	REQUIRE(ADD(cx, Imm8<0x12>) == BYTES(0x66, 0x83, 0xc1, 0x12));
	REQUIRE(ADD(dx, Imm8<0x12>) == BYTES(0x66, 0x83, 0xc2, 0x12));
	REQUIRE(ADD(bx, Imm8<0x12>) == BYTES(0x66, 0x83, 0xc3, 0x12));
	REQUIRE(ADD(sp, Imm8<0x12>) == BYTES(0x66, 0x83, 0xc4, 0x12));
	REQUIRE(ADD(bp, Imm8<0x12>) == BYTES(0x66, 0x83, 0xc5, 0x12));
	REQUIRE(ADD(si, Imm8<0x12>) == BYTES(0x66, 0x83, 0xc6, 0x12));
	REQUIRE(ADD(di, Imm8<0x12>) == BYTES(0x66, 0x83, 0xc7, 0x12));
    }

    SECTION("r16, imm16") {
	REQUIRE(ADD(ax, Imm16<0x1234>) == BYTES(0x66, 0x81, 0xc0, 0x34, 0x12));
	REQUIRE(ADD(cx, Imm16<0x1234>) == BYTES(0x66, 0x81, 0xc1, 0x34, 0x12));
	REQUIRE(ADD(dx, Imm16<0x1234>) == BYTES(0x66, 0x81, 0xc2, 0x34, 0x12));
	REQUIRE(ADD(bx, Imm16<0x1234>) == BYTES(0x66, 0x81, 0xc3, 0x34, 0x12));
	REQUIRE(ADD(sp, Imm16<0x1234>) == BYTES(0x66, 0x81, 0xc4, 0x34, 0x12));
	REQUIRE(ADD(bp, Imm16<0x1234>) == BYTES(0x66, 0x81, 0xc5, 0x34, 0x12));
	REQUIRE(ADD(si, Imm16<0x1234>) == BYTES(0x66, 0x81, 0xc6, 0x34, 0x12));
	REQUIRE(ADD(di, Imm16<0x1234>) == BYTES(0x66, 0x81, 0xc7, 0x34, 0x12));
    }

    SECTION("r32, imm8") {
	REQUIRE(ADD(eax, Imm8<0x12>) == BYTES(0x83, 0xc0, 0x12));
	REQUIRE(ADD(ecx, Imm8<0x12>) == BYTES(0x83, 0xc1, 0x12));
	REQUIRE(ADD(edx, Imm8<0x12>) == BYTES(0x83, 0xc2, 0x12));
	REQUIRE(ADD(ebx, Imm8<0x12>) == BYTES(0x83, 0xc3, 0x12));
	REQUIRE(ADD(esp, Imm8<0x12>) == BYTES(0x83, 0xc4, 0x12));
	REQUIRE(ADD(ebp, Imm8<0x12>) == BYTES(0x83, 0xc5, 0x12));
	REQUIRE(ADD(esi, Imm8<0x12>) == BYTES(0x83, 0xc6, 0x12));
	REQUIRE(ADD(edi, Imm8<0x12>) == BYTES(0x83, 0xc7, 0x12));
    }

    SECTION("r32, imm32") {
	REQUIRE(ADD(eax, Imm32<0x12345678>) == BYTES(0x81, 0xc0, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(ecx, Imm32<0x12345678>) == BYTES(0x81, 0xc1, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(edx, Imm32<0x12345678>) == BYTES(0x81, 0xc2, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(ebx, Imm32<0x12345678>) == BYTES(0x81, 0xc3, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(esp, Imm32<0x12345678>) == BYTES(0x81, 0xc4, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(ebp, Imm32<0x12345678>) == BYTES(0x81, 0xc5, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(esi, Imm32<0x12345678>) == BYTES(0x81, 0xc6, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(edi, Imm32<0x12345678>) == BYTES(0x81, 0xc7, 0x78, 0x56, 0x34, 0x12));
    }

    SECTION("r64, imm8") {
	REQUIRE(ADD(rax, Imm8<0x12>) == BYTES(0x48, 0x83, 0xc0, 0x12));
	REQUIRE(ADD(rcx, Imm8<0x12>) == BYTES(0x48, 0x83, 0xc1, 0x12));
	REQUIRE(ADD(rdx, Imm8<0x12>) == BYTES(0x48, 0x83, 0xc2, 0x12));
	REQUIRE(ADD(rbx, Imm8<0x12>) == BYTES(0x48, 0x83, 0xc3, 0x12));
	REQUIRE(ADD(rsp, Imm8<0x12>) == BYTES(0x48, 0x83, 0xc4, 0x12));
	REQUIRE(ADD(rbp, Imm8<0x12>) == BYTES(0x48, 0x83, 0xc5, 0x12));
	REQUIRE(ADD(rsi, Imm8<0x12>) == BYTES(0x48, 0x83, 0xc6, 0x12));
	REQUIRE(ADD(rdi, Imm8<0x12>) == BYTES(0x48, 0x83, 0xc7, 0x12));
	REQUIRE(ADD(r8 , Imm8<0x12>) == BYTES(0x49, 0x83, 0xc0, 0x12));
	REQUIRE(ADD(r9 , Imm8<0x12>) == BYTES(0x49, 0x83, 0xc1, 0x12));
	REQUIRE(ADD(r10, Imm8<0x12>) == BYTES(0x49, 0x83, 0xc2, 0x12));
	REQUIRE(ADD(r11, Imm8<0x12>) == BYTES(0x49, 0x83, 0xc3, 0x12));
	REQUIRE(ADD(r12, Imm8<0x12>) == BYTES(0x49, 0x83, 0xc4, 0x12));
	REQUIRE(ADD(r13, Imm8<0x12>) == BYTES(0x49, 0x83, 0xc5, 0x12));
	REQUIRE(ADD(r14, Imm8<0x12>) == BYTES(0x49, 0x83, 0xc6, 0x12));
	REQUIRE(ADD(r15, Imm8<0x12>) == BYTES(0x49, 0x83, 0xc7, 0x12));
    }

    SECTION("r64, imm32") {
	REQUIRE(ADD(rax, Imm32<0x12345678>) == BYTES(0x48, 0x81, 0xc0, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(rcx, Imm32<0x12345678>) == BYTES(0x48, 0x81, 0xc1, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(rdx, Imm32<0x12345678>) == BYTES(0x48, 0x81, 0xc2, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(rbx, Imm32<0x12345678>) == BYTES(0x48, 0x81, 0xc3, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(rsp, Imm32<0x12345678>) == BYTES(0x48, 0x81, 0xc4, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(rbp, Imm32<0x12345678>) == BYTES(0x48, 0x81, 0xc5, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(rsi, Imm32<0x12345678>) == BYTES(0x48, 0x81, 0xc6, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(rdi, Imm32<0x12345678>) == BYTES(0x48, 0x81, 0xc7, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(r8 , Imm32<0x12345678>) == BYTES(0x49, 0x81, 0xc0, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(r9 , Imm32<0x12345678>) == BYTES(0x49, 0x81, 0xc1, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(r10, Imm32<0x12345678>) == BYTES(0x49, 0x81, 0xc2, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(r11, Imm32<0x12345678>) == BYTES(0x49, 0x81, 0xc3, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(r12, Imm32<0x12345678>) == BYTES(0x49, 0x81, 0xc4, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(r13, Imm32<0x12345678>) == BYTES(0x49, 0x81, 0xc5, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(r14, Imm32<0x12345678>) == BYTES(0x49, 0x81, 0xc6, 0x78, 0x56, 0x34, 0x12));
	REQUIRE(ADD(r15, Imm32<0x12345678>) == BYTES(0x49, 0x81, 0xc7, 0x78, 0x56, 0x34, 0x12));
    }

    SECTION("r8, r8") {
	REQUIRE(ADD(al, bh) == BYTES(0x00, 0xf8));
	REQUIRE(ADD(cl, dh) == BYTES(0x00, 0xf1));
	REQUIRE(ADD(dl, ch) == BYTES(0x00, 0xea));
	REQUIRE(ADD(bl, ah) == BYTES(0x00, 0xe3));
	REQUIRE(ADD(ah, bl) == BYTES(0x00, 0xdc));
	REQUIRE(ADD(ch, dl) == BYTES(0x00, 0xd5));
	REQUIRE(ADD(dh, cl) == BYTES(0x00, 0xce));
	REQUIRE(ADD(bh, al) == BYTES(0x00, 0xc7));
    }

    SECTION("r16, r16") {
	REQUIRE(ADD(ax, di) == BYTES(0x66, 0x01, 0xf8));
	REQUIRE(ADD(cx, si) == BYTES(0x66, 0x01, 0xf1));
	REQUIRE(ADD(dx, bp) == BYTES(0x66, 0x01, 0xea));
	REQUIRE(ADD(bx, sp) == BYTES(0x66, 0x01, 0xe3));
	REQUIRE(ADD(sp, bx) == BYTES(0x66, 0x01, 0xdc));
	REQUIRE(ADD(bp, dx) == BYTES(0x66, 0x01, 0xd5));
	REQUIRE(ADD(si, cx) == BYTES(0x66, 0x01, 0xce));
	REQUIRE(ADD(di, ax) == BYTES(0x66, 0x01, 0xc7));
    }

    SECTION("r32, r32") {
	//ADD(eax, edi).print();
	REQUIRE(ADD(eax, edi) == BYTES(0x01, 0xf8));
	REQUIRE(ADD(ecx, esi) == BYTES(0x01, 0xf1));
	REQUIRE(ADD(edx, ebp) == BYTES(0x01, 0xea));
	REQUIRE(ADD(ebx, esp) == BYTES(0x01, 0xe3));
	REQUIRE(ADD(esp, ebx) == BYTES(0x01, 0xdc));
	REQUIRE(ADD(ebp, edx) == BYTES(0x01, 0xd5));
	REQUIRE(ADD(esi, ecx) == BYTES(0x01, 0xce));
	REQUIRE(ADD(edi, eax) == BYTES(0x01, 0xc7));
    }

    SECTION("r64, r64") {
	REQUIRE(ADD(rax, r15) == BYTES(0x4c, 0x01, 0xf8));
	REQUIRE(ADD(rcx, r14) == BYTES(0x4c, 0x01, 0xf1));
	REQUIRE(ADD(rdx, r13) == BYTES(0x4c, 0x01, 0xea));
	REQUIRE(ADD(rbx, r12) == BYTES(0x4c, 0x01, 0xe3));
	REQUIRE(ADD(rsp, r11) == BYTES(0x4c, 0x01, 0xdc));
	REQUIRE(ADD(rbp, r10) == BYTES(0x4c, 0x01, 0xd5));
	REQUIRE(ADD(rsi, r9 ) == BYTES(0x4c, 0x01, 0xce));
	REQUIRE(ADD(rdi, r8 ) == BYTES(0x4c, 0x01, 0xc7));
	REQUIRE(ADD(r8 , rdi) == BYTES(0x49, 0x01, 0xf8));
	REQUIRE(ADD(r9 , rsi) == BYTES(0x49, 0x01, 0xf1));
	REQUIRE(ADD(r10, rbp) == BYTES(0x49, 0x01, 0xea));
	REQUIRE(ADD(r11, rsp) == BYTES(0x49, 0x01, 0xe3));
	REQUIRE(ADD(r12, rbx) == BYTES(0x49, 0x01, 0xdc));
	REQUIRE(ADD(r13, rdx) == BYTES(0x49, 0x01, 0xd5));
	REQUIRE(ADD(r14, rcx) == BYTES(0x49, 0x01, 0xce));
	REQUIRE(ADD(r15, rax) == BYTES(0x49, 0x01, 0xc7));
    }

    SECTION("m8, imm8") {
	REQUIRE(ADD(b_[eax], Imm8<0x12>) == BYTES(0x67, 0x80, 0x00, 0x12));
	REQUIRE(ADD(b_[ecx], Imm8<0x12>) == BYTES(0x67, 0x80, 0x01, 0x12));
	REQUIRE(ADD(b_[edx], Imm8<0x12>) == BYTES(0x67, 0x80, 0x02, 0x12));
	REQUIRE(ADD(b_[ebx], Imm8<0x12>) == BYTES(0x67, 0x80, 0x03, 0x12));
	REQUIRE(ADD(b_[esp], Imm8<0x12>) == BYTES(0x67, 0x80, 0x04, 0x24, 0x12));
	REQUIRE(ADD(b_[ebp], Imm8<0x12>) == BYTES(0x67, 0x80, 0x45, 0x00, 0x12));
	REQUIRE(ADD(b_[esi], Imm8<0x12>) == BYTES(0x67, 0x80, 0x06, 0x12));
	REQUIRE(ADD(b_[edi], Imm8<0x12>) == BYTES(0x67, 0x80, 0x07, 0x12));
	REQUIRE(ADD(b_[rax], Imm8<0x12>) == BYTES(0x80, 0x00, 0x12));
	REQUIRE(ADD(b_[rcx], Imm8<0x12>) == BYTES(0x80, 0x01, 0x12));
	REQUIRE(ADD(b_[rdx], Imm8<0x12>) == BYTES(0x80, 0x02, 0x12));
	REQUIRE(ADD(b_[rbx], Imm8<0x12>) == BYTES(0x80, 0x03, 0x12));
	REQUIRE(ADD(b_[rsp], Imm8<0x12>) == BYTES(0x80, 0x04, 0x24, 0x12));
	REQUIRE(ADD(b_[rbp], Imm8<0x12>) == BYTES(0x80, 0x45, 0x00, 0x12));
	REQUIRE(ADD(b_[rsi], Imm8<0x12>) == BYTES(0x80, 0x06, 0x12));
	REQUIRE(ADD(b_[rdi], Imm8<0x12>) == BYTES(0x80, 0x07, 0x12));
	REQUIRE(ADD(b_[r8 ], Imm8<0x12>) == BYTES(0x41, 0x80, 0x00, 0x12));
        REQUIRE(ADD(b_[r9 ], Imm8<0x12>) == BYTES(0x41, 0x80, 0x01, 0x12));
        REQUIRE(ADD(b_[r10], Imm8<0x12>) == BYTES(0x41, 0x80, 0x02, 0x12));
        REQUIRE(ADD(b_[r11], Imm8<0x12>) == BYTES(0x41, 0x80, 0x03, 0x12));
        REQUIRE(ADD(b_[r12], Imm8<0x12>) == BYTES(0x41, 0x80, 0x04, 0x24, 0x12));
        REQUIRE(ADD(b_[r13], Imm8<0x12>) == BYTES(0x41, 0x80, 0x45, 0x00, 0x12));
        REQUIRE(ADD(b_[r14], Imm8<0x12>) == BYTES(0x41, 0x80, 0x06, 0x12));
        REQUIRE(ADD(b_[r15], Imm8<0x12>) == BYTES(0x41, 0x80, 0x07, 0x12));

	REQUIRE(ADD(b_[eax + Imm8<0x7f>], Imm8<0x12>) == BYTES(0x67, 0x80, 0x40, 0x7f, 0x12));  
        REQUIRE(ADD(b_[ecx + Imm8<0x7f>], Imm8<0x12>) == BYTES(0x67, 0x80, 0x41, 0x7f, 0x12));
        REQUIRE(ADD(b_[edx + Imm8<0x7f>], Imm8<0x12>) == BYTES(0x67, 0x80, 0x42, 0x7f, 0x12));
        REQUIRE(ADD(b_[ebx + Imm8<0x7f>], Imm8<0x12>) == BYTES(0x67, 0x80, 0x43, 0x7f, 0x12));
        REQUIRE(ADD(b_[esp + Imm8<0x7f>], Imm8<0x12>) == BYTES(0x67, 0x80, 0x44, 0x24, 0x7f, 0x12));
        REQUIRE(ADD(b_[ebp + Imm8<0x7f>], Imm8<0x12>) == BYTES(0x67, 0x80, 0x45, 0x7f, 0x12));
        REQUIRE(ADD(b_[esi + Imm8<0x7f>], Imm8<0x12>) == BYTES(0x67, 0x80, 0x46, 0x7f, 0x12));
        REQUIRE(ADD(b_[edi + Imm8<0x7f>], Imm8<0x12>) == BYTES(0x67, 0x80, 0x47, 0x7f, 0x12));


    }

}



