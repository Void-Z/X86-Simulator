#include <unistd.h>
#include <sys/stat.h>
#include <setjmp.h>
#include <sys/time.h>
#include <assert.h>
#include <time.h>
#include "syscall.h"


extern char _end;
intptr_t end = (intptr_t)&_end;

#if defined(__ISA_X86__)
intptr_t _syscall_(int type, intptr_t a0, intptr_t a1, intptr_t a2){
  int ret = -1;
  asm volatile("int $0x80": "=a"(ret): "a"(type), "b"(a0), "c"(a1), "d"(a2));
  return ret;
}
#elif defined(__ISA_AM_NATIVE__)
intptr_t _syscall_(int type, intptr_t a0, intptr_t a1, intptr_t a2){
  intptr_t ret = 0;
  asm volatile("call *0x100000": "=a"(ret): "a"(type), "S"(a0), "d"(a1), "c"(a2));
  return ret;
}
#else
#error _syscall_ is not implemented
#endif

void _exit(int status) {
  _syscall_(SYS_exit, status, 0, 0);
  while (1);
}

int _open(const char *path, int flags, mode_t mode) {
  // _exit(SYS_open);
  int n = _syscall_(SYS_open,path,flags,mode);
  return n;
}

int _write(int fd, void *buf, size_t count){
  // _exit(SYS_write);
  int n = _syscall_(SYS_write,fd,buf,count);
  return n;
}

void *_sbrk(intptr_t increment){
  intptr_t tem = end;
  int n = _syscall_(SYS_brk,(tem + increment),0,0);
  if(n == 0) {
    end += increment;
    return (void *)tem;
  } else {
    return (void *)-1;
  }
  
}

int _read(int fd, void *buf, size_t count) {
  // _exit(SYS_read);
  // printf("begin syscall_read\n");
  int n = _syscall_(SYS_read,fd,buf,count);
  // printf("syscall_read return %d\n",n);
  return n;
}

int _close(int fd) {
  // _exit(SYS_close);
  int n = _syscall_(SYS_close,fd,0,0);
  return n;
}

off_t _lseek(int fd, off_t offset, int whence) {
  // _exit(SYS_lseek);
  off_t n = _syscall_(SYS_lseek,fd,offset,whence);
  return n;
}

int _execve(const char *fname, char * const argv[], char *const envp[]) {
  return _syscall_(SYS_execve,fname,argv,envp);
}

// The code below is not used by Nanos-lite.
// But to pass linking, they are defined as dummy functions

int _fstat(int fd, struct stat *buf) {
  return 0;
}

int _kill(int pid, int sig) {
  _exit(-SYS_kill);
  return -1;
}

pid_t _getpid() {
  _exit(-SYS_getpid);
  return 1;
}

void _wait(int status) {

}
void _fork(int status) {

}