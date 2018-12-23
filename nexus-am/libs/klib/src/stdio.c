#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  int f = 0,d;
  const char * s;
  va_list arg_ptr;
  va_start(arg_ptr,0);
  while(*fmt) {
    if(f) {
      if(*fmt == 'd') {
        d = va_arg(arg_ptr,int);
        while(d) {
          *(out++) = d % 10 + '0';
          d /= 10;
        }
      } else if(*fmt == 's') {
        s = va_arg(arg_ptr,const char *);
        while(*s) {
          *(out++) = *(s++);
        }
      }
      f = 0;
    } else {
      if(*fmt == '%') {
        f = 1;
      } else {
        *(out++) = *fmt;
      }
    }
    ++fmt;
  }
  va_end(arg_ptr);
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
