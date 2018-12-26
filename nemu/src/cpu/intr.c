#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_push(&cpu.EFLAGS,4);
  rtl_push(&cpu.cs,4);
  rtl_push(&ret_addr,4);
  GateDesc idt;
  idt = (GateDesc)vaddr_read(cpu.idtr + NO * 4,4);
  printf("%d 0x%08x 0x%08x 0x%08x\n",NO,idt.offset_15_0,idt.offset_31_16,cpu.idtr);
  rtl_j((idt.offset_31_16 << 16) + idt.offset_15_0);
}

void dev_raise_intr() {
}
