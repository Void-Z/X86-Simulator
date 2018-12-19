#include "cpu/exec.h"

make_EHelper(mov);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);

//dummy.c
make_EHelper(call);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(sub);//eflags set wait implement
make_EHelper(xor);
make_EHelper(ret);


//div.c
make_EHelper(and);//eflags set wait implement
make_EHelper(lea);
make_EHelper(pushl);
make_EHelper(inc);
make_EHelper(cmp);//eflags set wait implement
make_EHelper(jne);
make_EHelper(nop);
make_EHelper(imul2);