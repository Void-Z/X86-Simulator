#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
  cpu.CF = 0;
  cpu.OF = 0;
  id_dest->val &= id_src->val;
  if(id_dest->val) {
    cpu.ZF = 0;
  } else {
    cpu.ZF = 1;
  }
  rtl_sr(id_dest->reg,&id_dest->val,id_dest->width);
  print_asm_template2(test);
}

make_EHelper(and) {
  // imm8 sign-extended
  if(id_src->width == 1) {
    id_src->val = (id_src->val & 0x80) == 0x80 ? id_src->val | 0xffffff00 : id_src->val;
  }
  rtl_and(&id_dest->val,&id_dest->val,&id_src->val);
  rtl_sr(id_dest->reg,&id_dest->val,id_dest->width);

  print_asm_template2(and);
}

make_EHelper(xor) {
  id_dest->val = id_dest->val ^ id_src->val;
  rtl_sr(id_dest->reg,&id_dest->val,id_dest->width);
  cpu.CF = 0;
  cpu.OF = 0;
  print_asm_template2(xor);
}

make_EHelper(or) {
  id_dest->val |= id_src->val;
  if(id_dest->type == OP_TYPE_REG) {
    rtl_sr(id_dest->reg,&id_dest->val,id_dest->width);
  } else {
    rtl_sm(&id_dest->addr,&id_dest->val,id_dest->width);
  }

  print_asm_template2(or);
}

make_EHelper(sar) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decoding.opcode & 0xf;

  rtl_setcc(&t2, cc);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  TODO();

  print_asm_template1(not);
}
