#include "common.h"
#include "syscall.h"
#include "fs.h"
#include "proc.h"

int sys_yield() {
  printf("Sys_yield.\n");
  _yield();
  return 1;
}

void sys_exit(int code) {
  printf("Sys_exit.\n");
  _halt(code);
}

size_t sys_write(int fd,const void *buf,size_t count) {
  _putc('a');
  if(fd == 1 || fd == 2) {
    for(size_t n = 0;n < count;++n) {
      _putc(*(const char *)(buf + n));
    }
    return count;
  } else {
    return -1;
  }
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
    case SYS_write: {
      result = sys_write(a[1],(void *)a[2],a[3]);
      break;
    }
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  c->GPRx = result;
  return NULL;
}

