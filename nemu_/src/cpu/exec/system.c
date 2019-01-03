#include "cpu/exec.h"

void difftest_skip_ref();
void difftest_skip_dut();
void raise_intr(uint8_t NO, vaddr_t ret_add);

uint32_t pio_read_l(ioaddr_t addr);
uint32_t pio_read_w(ioaddr_t addr);
uint32_t pio_read_b(ioaddr_t addr);
void pio_write_l(ioaddr_t addr, uint32_t data);
void pio_write_w(ioaddr_t addr, uint32_t data);
void pio_write_b(ioaddr_t addr, uint32_t data);

make_EHelper(lidt) {
  // cpu.idtr = id_dest->addr;
  if(decoding.is_operand_size_16) {
    cpu.idtr = vaddr_read(id_dest->addr + 2,4) & 0x00ffffff;
  } else {
    cpu.idtr = vaddr_read(id_dest->addr + 2,4);
  }
  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(int) {
  
  raise_intr(id_dest->val,decoding.seq_eip);
  print_asm("int %s", id_dest->str);

#if defined(DIFF_TEST) && defined(DIFF_TEST_QEMU)
  difftest_skip_dut();
#endif
}

make_EHelper(iret) {
  rtl_pop(&t0,4);
  rtl_pop(&cpu.cs,4);
  rtl_pop(&cpu.EFLAGS,4);
  rtl_j(t0);
  print_asm("iret");
}

make_EHelper(in) {
  switch(id_src->width) {
    case 1: {
      id_src->val = pio_read_b(id_src->val);
      break;
    }
    case 2: {
      id_src->val = pio_read_w(id_src->val);
      break;
    }
    case 4: {
      id_src->val = pio_read_l(id_src->val);
      break;
    }
  }
  operand_write(id_dest,&id_src->val);

  print_asm_template2(in);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}

make_EHelper(out) {
  switch(id_dest->width) {
    case 1: {
      pio_write_b(id_dest->val,id_src->val);
      break;
    }
    case 2: {
      pio_write_w(id_dest->val,id_src->val);
      break;
    }
    case 4: {
      pio_write_l(id_dest->val,id_src->val);
      break;
    }
  }

  print_asm_template2(out);

#if defined(DIFF_TEST)
  difftest_skip_ref();
#endif
}
