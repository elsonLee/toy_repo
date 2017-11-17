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

    auto c = ADD(b_[rax + Imm32<0x12345678>], VarImm8);
    c.print();
    c.emit(0xcd);

    ADD(b_[rax + Imm32<0x12345678>], Imm8<0x12>).emit();
    ADD(b_[rax + Imm32<0x12345678>], VarImm8).emit(0xcd);
    return 0;
}
