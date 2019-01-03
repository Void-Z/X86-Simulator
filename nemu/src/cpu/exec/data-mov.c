#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  rtl_push(&id_dest->val,id_dest->width);
  print_asm_template1(push);
}

make_EHelper(pushl) {
  // switch(id_dest->width) {
  //   case 1: {
  //     rtl_sign_extend8to32(&id_dest->val);
  //   }
  // }
  if(id_dest->width == 1) {
    rtl_sext(&id_dest->val,&id_dest->val,id_dest->width);
  }
  rtl_push(&id_dest->val,4);
  // printf("0x%08x 0x%08x\n\n",id_dest->addr,id_dest->val);
  print_asm_template1(pushl);
}

make_EHelper(pop) {
  rtl_pop(&id_dest->val,id_dest->width);
  // rtl_sr(id_dest->reg,&id_dest->val,id_dest->width);
  operand_write(id_dest,&id_dest->val);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  t0 = cpu.esp;
  rtl_push(&cpu.eax,4);
  rtl_push(&cpu.ecx,4);
  rtl_push(&cpu.edx,4);
  rtl_push(&cpu.ebx,4);
  rtl_push(&t0,4);
  rtl_push(&cpu.ebp,4);
  rtl_push(&cpu.esi,4);
  rtl_push(&cpu.edi,4);

  print_asm("pusha");
}

make_EHelper(popa) {
  rtl_pop(&cpu.edi,4);
  rtl_pop(&cpu.esi,4);
  rtl_pop(&cpu.ebp,4);
  rtl_pop(&t0,4);
  rtl_pop(&cpu.ebx,4);
  rtl_pop(&cpu.edx,4);
  rtl_pop(&cpu.ecx,4);
  rtl_pop(&cpu.eax,4);

  print_asm("popa");
}

make_EHelper(leave) {
  rtl_lr(&id_dest->val,5,4);
  rtl_sr(4,&id_dest->val,4);
  rtl_pop(&id_dest->val,4);
  if(decoding.is_operand_size_16) {
    rtl_sr(5,&id_dest->val,2);
  } else {
    rtl_sr(5,&id_dest->val,4);
  }

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decoding.is_operand_size_16) {
    rtl_lr(&id_dest->val,0,2);
    id_dest->val = (id_dest->val & 0x8000) ? 0xffffffff : 0;
  }
  else {
    rtl_lr(&id_dest->val,0,4);
    id_dest->val = (id_dest->val & 0x80000000) ? 0xffffffff : 0;
  }
  rtl_sr(2,&id_dest->val,4);
  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}
make_EHelper(stos) {
  rtl_sm(&cpu.edi,&cpu.eax,4);
  --cpu.edi;
  print_asm("stos");
}
make_EHelper(cwtl) {
  if (decoding.is_operand_size_16) {
    printf("1to2\n");
    TODO();
  } else {
    // printf("2to4\n");
    rtl_lr(&id_dest->val,R_AX,2);
    rtl_sext(&id_dest->val,&id_dest->val,2);
    rtl_sr(R_EAX,&id_dest->val,4);
  }

  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t0, &id_src->val, id_src->width);
  // printf("\n\n movsx src width:%d,dest width:%d,sext-value:0x%08x\n",id_src->width,id_dest->width,t0);
  operand_write(id_dest, &t0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}

