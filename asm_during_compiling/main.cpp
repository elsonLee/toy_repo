#include <array>
#include <cstdio>
#include <cassert>

#include "lxasm.h"

int main ()
{
    //ADD(b_[rsi + Imm8<0x34>], al).print();
    //ADD(b_[rdi + Imm8<0x34>], al).print();
    //ADD(b_[r8  + Imm8<0x34>], al).print();
    //ADD(b_[r12 + Imm8<0x34>], al).print();
    //ADD(b_[r12 + Imm32<0x12345678>], al).print();
    //ADD(b_[r13 + Imm8<0x34>], al).print();
    //ADD(b_[rsp + Imm8<0x34>], al).print();
    //ADD(b_[rsp + Imm32<0x12345678>], al).print();
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
