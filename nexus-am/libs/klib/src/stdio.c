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

  // int f = 0,d,n,count = 0;
  // char buf[32];
  // const char * s;
  
  // va_list arg_ptr;
  // va_start(arg_ptr,0);
  // while(*fmt) {
  //   if(f) {
  //     if(*fmt == 'd') {
  //       d = va_arg(arg_ptr,int);
  //       n = 0;
  //       while(d) {
  //         *(buf + n++) = d % 10 + '0';
  //         d /= 10;
  //       }
  //       while(n--) {
  //         _putc(*(buf + n));
  //         ++count;
  //       }
  //     } else if(*fmt == 's') {
  //       s = va_arg(arg_ptr,const char *);
  //       while(*s) {
  //         _putc(*(s++));
  //         ++count;
  //       }
  //     }
  //     f = 0;
  //   } else {
  //     if(*fmt == '%') {
  //       f = 1;
  //     } else {
  //       _putc(*fmt);
  //       ++count;
  //     }
  //   }
  //   ++fmt;
  // }
  // va_end(arg_ptr);
  // return count;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int f = 0,d,n;
  char buf[32];
  const char * s;
  char *p = out;

  while(*fmt) {
    if(f) {
      if(*fmt == 'd') {
        d = va_arg(ap,int);
        n = 0;
        while(d) {
          *(buf + n++) = d % 10 + '0';
          d /= 10;
        }
        while(n--) {
          *(out++) = *(buf + n);
        }
      } else if(*fmt == 's') {
        s = va_arg(ap,const char *);
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
  *out = 0;
  return out - p;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list arg_ptr;
  va_start(arg_ptr,fmt);
  int n = vsprintf(out,fmt,arg_ptr);
  va_end(arg_ptr);
  return n;
  // int f = 0,d,n;
  // char buf[32];
  // const char * s;
  // char *p = out;
  
  // va_list arg_ptr;
  // va_start(arg_ptr,0);
  // while(*fmt) {
  //   if(f) {
  //     if(*fmt == 'd') {
  //       d = va_arg(arg_ptr,int);
  //       n = 0;
  //       while(d) {
  //         *(buf + n++) = d % 10 + '0';
  //         d /= 10;
  //       }
  //       while(n--) {
  //         *(out++) = *(buf + n);
  //       }
  //     } else if(*fmt == 's') {
  //       s = va_arg(arg_ptr,const char *);
  //       while(*s) {
  //         *(out++) = *(s++);
  //       }
  //     }
  //     f = 0;
  //   } else {
  //     if(*fmt == '%') {
  //       f = 1;
  //     } else {
  //       *(out++) = *fmt;
  //     }
  //   }
  //   ++fmt;
  // }
  // *out = 0;
  // va_end(arg_ptr);
  // return out - p;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
