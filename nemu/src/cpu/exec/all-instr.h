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