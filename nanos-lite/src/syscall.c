#include "common.h"
#include "syscall.h"

int sys_yield() {
  _yield();
  return 1;
}

void sys_exit(int code) {
  _halt(code);
}

_Context* do_syscall(_Context *c) {
  uintptr_t a[4],result = -1;
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;  
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_exit: {
      sys_exit(0);
      break;
    }
    case SYS_yield: {
      result = sys_yield();
      break;
    }
    case SYS_kill: {
      sys_exit(0);
      break;
    }
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  c->GPRx = result;
  return NULL;
}

