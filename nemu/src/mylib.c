#include "mylib.h"

My_lib _ = {
    mylib_log,
    mylib_atox,
    mylib_getreg
};

int mylib_log(const char * format, ...) {
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

uint32_t mylib_atox(char *args) {
    _.log("%s",args);
    uint32_t val = 0;
    args += 2;
    for(;*args;++args) {
        val *= 16;
        val += *args >= 'a' ? *args - 'a' + 10 : *args - '0';
    }
    return val;
}

uint32_t mylib_getreg(char *args,bool *success) {
    // if(!strcmp(args,"$eax")) {
    //     return cpu.eax;
    // } else if(!strcmp(args,"$ecx")) {
    //     return cpu.ecx;
    // } else if(!strcmp(args,"$edx")) {

    // } else if(!strcmp(args,"$ebx")) {

    // } else if(!strcmp(args,"$esp")) {

    // } else if(!strcmp(args,"$ebp")) {

    // } else if(!strcmp(args,"$esi")) {

    // } else if(!strcmp(args,"$edi")) {

    // } else if(!strcmp(args,"$eip")) {

    // } else {
    //     return 0;
    // }
    // 00 a,b,c,e
    // 0000 a,b,c,d,s,i,x,h,l
    // 00 x,p,i
    assert(args != NULL && success != NULL);
    // uint8_t op;
    // int len = strlen(args);
    // if(len == 4) {
    //     // 1100 0000
    //     op = 0xc0;
    // } else if(len == 3) {
        
    // } else {
    //     *success = false;
    //     return 0;
    // }
    return 0;
}

