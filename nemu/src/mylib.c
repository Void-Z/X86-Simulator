#include "mylib.h"

My_lib $ = {
    0xff,
    __mylib_log__,
    __mylib_atox__,
    __mylib_getreg__
};

int __mylib_log__(const char * format, ...) {
#ifdef __MYLIB_DEBUG__
    va_list arg_ptr;
    va_start(arg_ptr, format);
    int n_Bytes = vprintf(format, arg_ptr);
    va_end(arg_ptr);
    return n_Bytes;
#else
    return 0;
#endif
}

uint32_t __mylib_atox__(char *args) {
  uint32_t val = 0;
  args += 2;
  for(;*args;++args) {
    val *= 16;
    val += *args - '0';
  }
  return val;
}

uint32_t __mylib_getreg__(char *args,bool *success) {
    return 0;
}

