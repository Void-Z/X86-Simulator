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
  printf("cpu idtr : 0x%08x\n",cpu.idtr);
  for(int i = 0;i <= NO;++i) {
    *(uint32_t *)&idt = vaddr_read(cpu.idtr + i * 8,4);
    *((uint32_t *)&idt + 1) = vaddr_read(cpu.idtr + i * 8 + 4,4);
    printf("int %d addr : 0x%08x\n",i,(idt.offset_31_16 << 16) + idt.offset_15_0);
  }
  // *(uint32_t *)&idt = vaddr_read(cpu.idtr + NO * 8,4);
  // *((uint32_t *)&idt + 1) = vaddr_read(cpu.idtr + NO * 8 + 4,4);
  // printf("int %d addr : 0x%08x\n",NO,(idt.offset_31_16 << 16) + idt.offset_15_0);
  rtl_j((idt.offset_31_16 << 16) + idt.offset_15_0);
}

void dev_raise_intr() {
}
