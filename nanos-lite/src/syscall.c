#include "common.h"
#include "syscall.h"

int sys_yield() {
  printf("Sys_yield().\n");
  _yield();
  return 1;
}

void sys_exit(int code) {
  printf("Sys_exit().\n");
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
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  c->GPRx = result;
  return NULL;
}

