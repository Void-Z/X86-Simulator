#include "cpu/exec.h"

make_EHelper(mov);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);

// Not sure!!
// div.c
make_EHelper(cltd);
make_EHelper(sete);

// Implement
// dummy.c fine
make_EHelper(call);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(sub);
make_EHelper(xor);
make_EHelper(ret);


// div.c fine
make_EHelper(and);
make_EHelper(lea);
make_EHelper(pushl);
make_EHelper(inc);
make_EHelper(cmp);
make_EHelper(jne);
make_EHelper(nop);
make_EHelper(imul2);// CF not implement
make_EHelper(add);
make_EHelper(idiv);
make_EHelper(test);
make_EHelper(je);
make_EHelper(leave);

// add.c fine
make_EHelper(movzx);

//add-longlong.c
make_EHelper(adc);
make_EHelper(or);

//bit.c
make_EHelper(sar);
make_EHelper(shl);
make_EHelper(setne);
make_EHelper(dec);
make_EHelper(sbb);
make_EHelper(not);

//if-else.c
make_EHelper(jmp);
make_EHelper(jle);

//mul-longlong.c
make_EHelper(imul1);

//max.c
make_EHelper(jge);

//load-store.c
make_EHelper(movsx);

//improve
make_EHelper(jcc);

//shift.c
make_EHelper(shr);

//matrix-mul.c
make_EHelper(neg);

//shuixianhua.c
make_EHelper(mul);

//recursion.c
make_EHelper(call_rm);
make_EHelper(jmp_rm);

make_EHelper(in);
make_EHelper(out);

make_EHelper(div);

make_EHelper(cwtl);
make_EHelper(setcc);

make_EHelper(rol);

make_EHelper(imul3);

make_EHelper(lidt);
make_EHelper(int);
make_EHelper(pusha);
make_EHelper(popa);
make_EHelper(stos);
make_EHelper(iret);