#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  union {
    GateDesc GD;
    struct {
      uint32_t low,high;
    };
  } idt;
  rtl_push(&cpu.EFLAGS,4);
  rtl_push(&cpu.cs,4);
  rtl_push(&ret_addr,4);
  NO += 4;
  vaddr_t addr = cpu.idtr + 8 * NO;
  printf("0x%x",cpu.idtr);
  idt.low = vaddr_read(addr,4);
  idt.high = vaddr_read(addr + 4,4);
  t0 = ((idt.GD.offset_31_16 & 0xffff) << 16) | (idt.GD.offset_15_0 & 0xffff);
  rtl_j(t0);
  // GateDesc idt;
  // *(uint32_t *)&idt = vaddr_read(cpu.idtr + NO * 8,4);
  // *((uint32_t *)&idt + 1) = vaddr_read(cpu.idtr + NO * 8 + 4,4);
  // rtl_j((idt.offset_31_16 << 16) + idt.offset_15_0);
}

void dev_raise_intr() {
}
