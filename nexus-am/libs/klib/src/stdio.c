#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char buf[256],*p = buf;
  va_list arg_ptr;
  va_start(arg_ptr,fmt);
  int n = vsprintf(buf,fmt,arg_ptr);
  va_end(arg_ptr);
  while(*p) {
    _putc(*(p++));
  }
  return n;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int f = 0,d,n;
  uint32_t u;
  char buf[32];
  const char * s;
  char *p = out;

  while(*fmt) {
    if(f) {
      if(*fmt == 'd') {
        d = va_arg(ap,int);
        n = 0;
        if(d == 0) {
          *(buf + n++) = '0';
        } else if(d < 0) {
          *(out++) = '-';
          d = -d;
        }
        while(d) {
          *(buf + n++) = (d % 10) + '0';
          d /= 10;
        }
        while(n--) {
          *(out++) = *(buf + n);
        }
        f = 0;
      } else if(*fmt == 'u') {
        u = va_arg(ap,uint32_t);
        n = 0;
        if(u == 0) {
          *(buf + n++) = '0';
        }
        while(u) {
          *(buf + n++) = (u % 10) + '0';
          u /= 10;
        }
        while(n--) {
          *(out++) = *(buf + n);
        }
        f = 0;
      } else if(*fmt == 's') {
        s = va_arg(ap,const char *);
        while(*s) {
          *(out++) = *(s++);
        }
        f = 0;
      } else if(*fmt == 'x') {
        u = va_arg(ap,int);
        n = 0;
        if(u == 0) {
          *(buf + n++) = '0'; 
        }
        
        while(d) {
          *(buf + n++) = (u % 16) < 10 ? (u % 16) + '0' : ((u % 16) - 10) + 'a';
          u /= 16;
        }
        while(n--) {
          *(out++) = *(buf + n);
        }
        f = 0;
      } else if(*fmt >= '0' && *fmt <= '9') {

      } 
      
    } else {
      if(*fmt == '%') {
        f = 1;
      } else {
        *(out++) = *fmt;
      }
    }
    ++fmt;
  }
  *out = 0;
  return out - p;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list arg_ptr;
  va_start(arg_ptr,fmt);
  int n = vsprintf(out,fmt,arg_ptr);
  va_end(arg_ptr);
  return n;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
