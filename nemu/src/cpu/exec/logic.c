#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(test) {
  cpu.CF = 0;
  cpu.OF = 0;
  id_dest->val &= id_src->val;
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  // if(id_dest->type == OP_TYPE_REG) {
  //   rtl_sr(id_dest->reg,&id_dest->val,id_dest->width);
  // } else {
  //   rtl_sm(&id_dest->addr,&id_dest->val,id_dest->width);
  // }
  print_asm_template2(test);
}

make_EHelper(and) {
  // imm8 sign-extended
  cpu.CF = 0;
  cpu.OF = 0;
  if(id_src->width == 1) {
    // id_src->val = (id_src->val & 0x80) == 0x80 ? id_src->val | 0xffffff00 : id_src->val;
    rtl_sign_extend8to32(&id_src->val);
  }
  rtl_and(&id_dest->val,&id_dest->val,&id_src->val);
  operand_write(id_dest,&id_dest->val);
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  print_asm_template2(and);
}

make_EHelper(xor) {
  if(id_src->width == 1) {
    rtl_sign_extend8to32(&id_src->val);
  }
  id_dest->val = id_dest->val ^ id_src->val;
  operand_write(id_dest,&id_dest->val);
  cpu.CF = 0;
  cpu.OF = 0;
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  print_asm_template2(xor);
}

make_EHelper(or) {
  cpu.CF = 0;
  cpu.OF = 0;
  if(id_src->width == 1) {
    rtl_sign_extend8to32(&id_src->val);
  }
  id_dest->val |= id_src->val;
  operand_write(id_dest,&id_dest->val);
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  print_asm_template2(or);
}

make_EHelper(sar) {
  for(t0 = 0;t0 < id_src->val;++t0) {
    // if(id_dest->val == 0) break;
    id_dest->val = (uint32_t)(((int)id_dest->val) >> 1);
  }
  if(id_dest->type == OP_TYPE_REG) {
    rtl_sr(id_dest->reg,&id_dest->val,id_dest->width);
  } else {
    rtl_sm(&id_dest->addr,&id_dest->val,id_dest->width);
  }
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  for(t0 = 0;t0 < id_src->val;++t0) {
    id_dest->val = id_dest->val * 2;
  }
  if(id_dest->type == OP_TYPE_REG) {
    rtl_sr(id_dest->reg,&id_dest->val,id_dest->width);
  } else {
    rtl_sm(&id_dest->addr,&id_dest->val,id_dest->width);
  }
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  for(t0 = 0;t0 < id_src->val;++t0) {
    id_dest->val = id_dest->val / 2;
  }
  operand_write(id_dest,&id_dest->val);
  rtl_update_ZFSF(&id_dest->val,id_dest->width);
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
  id_dest->val = ~id_dest->val;
  operand_write(id_dest,&id_dest->val);
  print_asm_template1(not);
}

make_EHelper(rol) {
  t0 = id_src->val;
  while(t0) {
    switch(id_dest->width) {
      case 1:t1 = (id_dest->val & 0x80) >> 7;break;
      case 2:t1 = (id_dest->val & 0x8000) >> 15;break;
      case 4:t1 = (id_dest->val & 0x80000000) >> 31;break;
    }
    id_dest->val = id_dest->val * 2 + t1;
    --t0;
  }
  if(id_src->val == 1) {
    switch(id_dest->width) {
      case 1:t1 = (id_dest->val & 0x80) >> 7;break;
      case 2:t1 = (id_dest->val & 0x8000) >> 15;break;
      case 4:t1 = (id_dest->val & 0x80000000) >> 31;break;
    }
    if(t1 != cpu.CF) {
      cpu.OF = 1;
    } else {
      cpu.OF = 0;
    }
  }
  operand_write(id_dest,&id_dest->val);

  print_asm("rol %s,%s",id_src->str,id_dest->str);
}