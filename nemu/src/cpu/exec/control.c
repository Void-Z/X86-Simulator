#include "cpu/exec.h"
#include "cpu/cc.h"

make_EHelper(jmp) {
  // the target address is calculated at the decode stage
  rtl_j(decoding.jmp_eip);
  print_asm("jmp %x", decoding.jmp_eip);
}

// make_EHelper(jle) {
//   if(id_dest->width == 1) {
//     rtl_sign_extend8to32(&id_dest->val);
//   }
//   if(cpu.ZF == 1 || cpu.SF != cpu.OF) {
//     rtl_j(decoding.seq_eip + id_dest->val);
//   }
//   print_asm("jle %x", decoding.seq_eip + id_dest->val);
// }

// make_EHelper(jge) {
//   if(id_dest->width == 1) {
//     rtl_sign_extend8to32(&id_dest->val);
//   }
//   if(cpu.SF == cpu.OF) {
//     rtl_j(decoding.seq_eip + id_dest->val);
//   }
//   print_asm("jge %x", decoding.seq_eip + id_dest->val);
// }

make_EHelper(jcc) {
  // the target address is calculated at the decode stage
  uint32_t cc = decoding.opcode & 0xf;
  rtl_setcc(&t0, cc);
  rtl_li(&t1, 0);
  rtl_jrelop(RELOP_NE, &t0, &t1, decoding.jmp_eip);

  print_asm("j%s %x", get_cc_name(cc), decoding.jmp_eip);
}

make_EHelper(jmp_rm) {
  rtl_jr(&id_dest->val);

  print_asm("jmp *%s", id_dest->str);
}

make_EHelper(call) {
  // the target address is calculated at the decode stage
  rtl_push(&decoding.seq_eip,4);
  rtl_j(decoding.seq_eip + id_dest->val);
  print_asm("call %x", decoding.seq_eip + id_dest->val);
}

make_EHelper(ret) {
  rtl_pop(&id_dest->val,4);
  rtl_j(id_dest->val);
  print_asm("ret");
}

make_EHelper(call_rm) {
  rtl_push(&decoding.seq_eip,4);
  rtl_j(id_dest->val);

  print_asm("call *%s", id_dest->str);
}

// make_EHelper(je) {
//   if(id_dest->width == 1) {
//     rtl_sign_extend8to32(&id_dest->val);
//   }
//   if(cpu.ZF) {
//     rtl_j(decoding.seq_eip + id_dest->val);
//   }
//   print_asm("je 0x%x", decoding.seq_eip + id_dest->val);
// }

// make_EHelper(jne) {
  
//   if(id_dest->width == 1) {
//     rtl_sign_extend8to32(&id_dest->val);
//   }
//   if(!cpu.ZF) {
//     rtl_j(decoding.seq_eip + id_dest->val);
//   }
//   print_asm("jne 0x%x", decoding.seq_eip + id_dest->val);
// }

make_EHelper(sete) {
  id_dest->val = cpu.ZF;
  // if(cpu.ZF) {
  //   // rtl_sr(id_dest->reg,&id_dest->val,id_dest->width);
    
  // }
  operand_write(id_dest,&id_dest->val);
  print_asm("sete *%s", id_dest->str);
}

make_EHelper(setne) {
  id_dest->val = cpu.ZF == 0 ? 1 : 0;
  // if(!cpu.ZF) {
  //   rtl_sr(id_dest->reg,&id_dest->val,id_dest->width);
  // }
  operand_write(id_dest,&id_dest->val);
  print_asm("setne *%s", id_dest->str);
}