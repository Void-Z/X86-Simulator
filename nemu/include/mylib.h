#ifndef __MYLIB_H__
#define __MYLIB_H__

#include "cpu/reg.h"
#include "monitor/monitor.h"

#include <stdint.h>
#include <stdarg.h>

#define __MYLIB_DEBUG__

int __mylib_log__(const char *, ...);
uint32_t __mylib_atox__(char *);
uint32_t __mylib_getreg__(char *,bool *);


struct {
    
    int (*log)(const char *, ...);
    uint32_t (*atox)(char *);
    uint32_t (*getreg)(char *,bool *);

} $ = {
    __mylib_log__,
    __mylib_atox__,
    __mylib_getreg__
};



#endif