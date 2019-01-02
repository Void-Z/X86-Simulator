#include "common.h"
#include "syscall.h"

void sys_yield() {
  _yield();
}

void sys_exit(int code) {
  _halt(code);
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;  
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_exit: {
      sys_exit(a[1]);
      break;
    }
    case SYS_yield: {
      sys_yield();
      break;
    }
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}

