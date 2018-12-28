#include <am.h>
#include <x86.h>

int printf(const char *fmt, ...);
static _Context* (*user_handler)(_Event, _Context*) = NULL;

void vectrap();
void vecnull();

_Context* irq_handle(_Context *tf) {
  _Context *next = tf;
  printf("cs:%d\n",tf->cs);
  printf("eax:%d\n",tf->eax);
  printf("ebp:%d\n",tf->ebp);
  printf("ebx:%d\n",tf->ebx);
  printf("ecx:%d\n",tf->ecx);
  printf("edi:%d\n",tf->edi);
  printf("edx:%d\n",tf->edx);
  printf("eflags:%d\n",tf->eflags);
  printf("eip:%d\n",tf->eip);
  printf("esi:%d\n",tf->esi);
  printf("esp:%d\n",tf->esp);
  printf("irq:%d\n",tf->irq);
  printf("prot:%d\n",tf->prot);
  if (user_handler) {
    _Event ev = {0};
    switch (tf->irq) {
      default: ev.event = _EVENT_ERROR; break;
    }

    next = user_handler(ev, tf);
    if (next == NULL) {
      next = tf;
    }
  }

  return next;
}

static GateDesc idt[NR_IRQ];

int _cte_init(_Context*(*handler)(_Event, _Context*)) {
  // initialize IDT
  for (unsigned int i = 0; i < NR_IRQ; i ++) {
    idt[i] = GATE(STS_TG32, KSEL(SEG_KCODE), vecnull, DPL_KERN);
  }

  // -------------------- system call --------------------------
  idt[0x81] = GATE(STS_TG32, KSEL(SEG_KCODE), vectrap, DPL_KERN);
// idt[0x81] = GATE(STS_TG32, KSEL(SEG_KCODE), 0x12345678, DPL_KERN);
  set_idt(idt, sizeof(idt));

  // register event handler
  user_handler = handler;

  return 0;
}

_Context *_kcontext(_Area stack, void (*entry)(void *), void *arg) {
  return NULL;
}

void _yield() {
  asm volatile("int $0x81");
}

int _intr_read() {
  return 0;
}

void _intr_write(int enable) {
}
