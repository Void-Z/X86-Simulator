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
    uint32_t val = 0;
    args += 2;
    for(;*args;++args) {
        val *= 16;
        val += *args >= 'a' ? *args - 'a' + 10 : *args - '0';
    }
    return val;
}

uint16_t mylib_regname2number(const char *args,int len) {
    uint16_t code;
    code = ((args[1] >= 'A' ? args[1] - 'A' : args[1] - 'a') << 10) + 
           ((args[2] >= 'A' ? args[2] - 'A' : args[2] - 'a') << 5);
    if(len == 4) {
        code += args[3] >= 'A' ? args[3] - 'A' : args[3] - 'a';
    }
    printf("%s %d",args,code);
    return code;
}

uint32_t mylib_getreg(char *args,bool *success) {
    assert(args != NULL && success != NULL);
    uint16_t op;
    int len = strlen(args);
    if(len == 4) {
        op = mylib_regname2number(args,4);
    } else if(len == 3) {
        op = mylib_regname2number(args,3);
    } else {
        *success = false;
        return 0;
    }
    switch(op) {
        case 0x9437: {
            return cpu.eax;
        }
        case 0x9477: {
            return cpu.ecx;
        }
        case 0x9497: {
            return cpu.edx;
        }
        case 0x9457: {
            return cpu.ebx;
        }
        case 0x966f: {
            return cpu.esp;
        }
        case 0x944f: {
            return cpu.ebp;
        }
        case 0x9668: {
            return cpu.esi;
        }
        case 0x9488: {
            return cpu.edi;
        } 
        case 0x952f: {
            return cpu.eip;
        }
        default: {
            *success = false;
            Log("Error register name:%s.\n",args);
            return 0;
        }
    }
}

