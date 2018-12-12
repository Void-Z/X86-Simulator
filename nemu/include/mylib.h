#ifndef __MYLIB_H__
#define __MYLIB_H__

#include "cpu/reg.h"
#include "monitor/monitor.h"

#include <stdint.h>
#include <stdarg.h>

#define __MYLIB_DEBUG__

int mylib_log(const char *, ...);
uint32_t mylib_atox(char *);
uint32_t mylib_getreg(char *,bool *);

typedef struct My_lib {
    int (*log)(const char *, ...);
    uint32_t (*atox)(char *);
    uint32_t (*getreg)(char *,bool *);
} My_lib;
extern My_lib _;


#endif