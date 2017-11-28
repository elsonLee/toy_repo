#include <array>
#include <cstdio>
#include <cassert>

#include "lxasm.h"

int main ()
{
    static_assert(ADD(al,   Imm8<0x12>)		== make_bytes<0x80, 0xc0, 0x12>());
    static_assert(ADD(bx,   Imm8<0x12>)	   	== make_bytes<0x66, 0x83, 0xc3, 0x12>());
    static_assert(ADD(cx,   Imm16<0x1234>)	== make_bytes<0x66, 0x81, 0xc1, 0x34, 0x12>());
    static_assert(ADD(edx,  Imm32<0x12345678>)	== make_bytes<0x81, 0xc2, 0x78, 0x56, 0x34, 0x12>());

    static_assert(ADD(rsi,  Imm8<0x12>)		== make_bytes<0x48, 0x83, 0xc6, 0x12>());
    static_assert(ADD(r11,  Imm8<0x12>) 	== make_bytes<0x49, 0x83, 0xc3, 0x12>());
    static_assert(ADD(rdi,  Imm32<0x12345678>)  == make_bytes<0x48, 0x81, 0xc7, 0x78, 0x56, 0x34, 0x12>());

    static_assert(ADD(bl,  ah)	== make_bytes<0x00, 0xe3>());
    static_assert(ADD(sp,  di)	== make_bytes<0x66, 0x01, 0xfc>());
    static_assert(ADD(esp, edi)	== make_bytes<0x01, 0xfc>());

    static_assert(ADD(b_[al],  Imm8<0x12>)				    == make_bytes<0x80, 0x00, 0x12>());
    static_assert(ADD(b_[rax + Imm8<0x00>], Imm8<0x12>)			    == make_bytes<0x80, 0x40, 0x00, 0x12>());
    static_assert(ADD(b_[rax + Imm8<0x34>], Imm8<0x12>)	    		    == make_bytes<0x80, 0x40, 0x34, 0x12>());
    static_assert(ADD(b_[rax + Imm32<0x12345678>], Imm8<0x12>)  	    == make_bytes<0x80, 0x80, 0x78, 0x56, 0x34, 0x12, 0x12>());
    static_assert(ADD(b_[r8  + Imm8<0x34>], Imm8<0x12>)			    == make_bytes<0x41, 0x80, 0x40, 0x34, 0x12>());
    static_assert(ADD(w_[rax + Imm8<0x34>], Imm8<0x12>) 		    == make_bytes<0x66, 0x83, 0x40, 0x34, 0x12>());
    static_assert(ADD(w_[r9  + Imm8<0x34>], Imm8<0x12>) 		    == make_bytes<0x66, 0x41, 0x83, 0x41, 0x34, 0x12>());
    static_assert(ADD(w_[rax + Imm8<0x34>], Imm16<0x1234>)      	    == make_bytes<0x66, 0x81, 0x40, 0x34, 0x34, 0x12>());
    static_assert(ADD(w_[r14 + Imm8<0x34>], Imm16<0x1234>)      	    == make_bytes<0x66, 0x41, 0x81, 0x46, 0x34, 0x34, 0x12>());
    static_assert(ADD(d_[rax + Imm8<0x34>], Imm8<0x12>)			    == make_bytes<0x83, 0x40, 0x34, 0x12>());
    static_assert(ADD(d_[r10 + Imm8<0x34>], Imm8<0x12>)			    == make_bytes<0x41, 0x83, 0x42, 0x34, 0x12>());
    static_assert(ADD(d_[r15 + Imm8<0x34>], Imm32<0x12345678>)  	    == make_bytes<0x41, 0x81, 0x47, 0x34, 0x78, 0x56, 0x34, 0x12>());
    static_assert(ADD(q_[rax + Imm8<0x34>], Imm8<0x12>)			    == make_bytes<0x48, 0x83, 0x40, 0x34, 0x12>());
    static_assert(ADD(q_[r11 + Imm8<0x34>], Imm8<0x12>)			    == make_bytes<0x49, 0x83, 0x43, 0x34, 0x12>());
    static_assert(ADD(q_[rdx + Imm8<0x34>], Imm32<0x12345678>)  	    == make_bytes<0x48, 0x81, 0x42, 0x34, 0x78, 0x56, 0x34, 0x12>());
    static_assert(ADD(w_[rax + rcx + Imm8<0x34>], Imm8<0x12>)   	    == make_bytes<0x66, 0x83, 0x44, 0x08, 0x34, 0x12>());
    static_assert(ADD(w_[rax + rcx * 2_x + Imm8<0x34>], Imm8<0x12>)	    == make_bytes<0x66, 0x83, 0x44, 0x48, 0x34, 0x12>());
    static_assert(ADD(w_[rax + rcx * 4_x + Imm8<0x34>], Imm8<0x12>)     == make_bytes<0x66, 0x83, 0x44, 0x88, 0x34, 0x12>());
    static_assert(ADD(w_[rax + rcx * 8_x + Imm8<0x34>], Imm8<0x12>)     == make_bytes<0x66, 0x83, 0x44, 0xc8, 0x34, 0x12>());
    static_assert(ADD(w_[rax + rcx * 8_x + Imm8<0x34>], Imm8<0x12>)     == make_bytes<0x66, 0x83, 0x44, 0xc8, 0x34, 0x12>());
    static_assert(ADD(w_[rax + 8_x * rcx + Imm8<0x34>], Imm8<0x12>)     == make_bytes<0x66, 0x83, 0x44, 0xc8, 0x34, 0x12>());

    static_assert(ADD(b_[rax + Imm8<0x34>], al)	    == make_bytes<0x00, 0x40, 0x34>());
    static_assert(ADD(w_[rax + Imm8<0x34>], ax)	    == make_bytes<0x66, 0x01, 0x40, 0x34>());
    static_assert(ADD(d_[rdx + Imm8<0x34>], ebx)    == make_bytes<0x01, 0x5a, 0x34>());
    static_assert(ADD(q_[rdx + Imm32<0x34>], rcx)   == make_bytes<0x48, 0x01, 0x8a, 0x34, 0x00, 0x00, 0x00>());
    static_assert(ADD(q_[r8  + Imm32<0x34>], rcx)   == make_bytes<0x49, 0x01, 0x88, 0x34, 0x00, 0x00, 0x00>());
    static_assert(ADD(q_[r8  + Imm32<0x34>], r9)    == make_bytes<0x4d, 0x01, 0x88, 0x34, 0x00, 0x00, 0x00>());

    ADD(b_[rsi + Imm8<0x34>], al).print();
    ADD(b_[rdi + Imm8<0x34>], al).print();
    ADD(b_[r8  + Imm8<0x34>], al).print();
    ADD(b_[r12 + Imm8<0x34>], al).print();
    ADD(b_[r12 + Imm32<0x12345678>], al).print();
    ADD(b_[r13 + Imm8<0x34>], al).print();
    ADD(b_[rsp + Imm8<0x34>], al).print();
    ADD(b_[rsp + Imm32<0x12345678>], al).print();
    // error
#if 0
    ADD r64, imm8
    ADD r64, imm32
    ADD r64, r64
    ADD mem8, r8
    {error
	ADD(b_[rsi + Imm8<0x34>], al).print();
	ADD(b_[rdi + Imm8<0x34>], al).print();
	ADD(b_[r8  + Imm8<0x34>], al).print();
	ADD(b_[r12 + Imm8<0x34>], al).print();
	ADD(b_[r13 + Imm8<0x34>], al).print();
    }
    {
	ADD(rax, Imm8<0x12>).print();
	ADD(rcx, Imm8<0x12>).print();
	ADD(rdx, Imm8<0x12>).print();
	ADD(rbx, Imm8<0x12>).print();
	ADD(rsp, Imm8<0x12>).print();
	ADD(rbp, Imm8<0x12>).print();
	ADD(rsi, Imm8<0x12>).print();
	ADD(rdi, Imm8<0x12>).print();
	ADD(r8,  Imm8<0x12>).print();
	ADD(r9,  Imm8<0x12>).print();
	ADD(r10, Imm8<0x12>).print();
	ADD(r11, Imm8<0x12>).print();
	ADD(r12, Imm8<0x12>).print();
	ADD(r13, Imm8<0x12>).print();
	ADD(r14, Imm8<0x12>).print();
	ADD(r15, Imm8<0x12>).print();
    }
#endif
    {
	INC(al).print();
	INC(ax).print();
	INC(eax).print();
	INC(rax).print();
	INC(r8).print();
	INC(r9).print();
	INC(r10).print();
	INC(r11).print();
	INC(r12).print();
	INC(r13).print();
	INC(r14).print();
	INC(r15).print();
	INC(b_[rax + Imm32<0x12345678>]).print();
	INC(w_[rax + Imm32<0x12345678>]).print();
	INC(d_[rax + Imm32<0x12345678>]).print();
	INC(q_[rax + Imm32<0x12345678>]).print();
    }

    //{
    //    auto c = ADD(b_[rax + Imm32<0x12345678>], VarImm8);
    //    c.print();
    //    c.emit(0xcd);
    //}

    //ADD(b_[rax + Imm32<0x12345678>], Imm8<0x12>).emit();
    //ADD(b_[rax + Imm32<0x12345678>], VarImm8).emit(0xcd);



    return 0;
}
