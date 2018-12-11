#include <stdio.h>
#include <stdint.h>
uint16_t mylib_regname2number(const char *args,int len) {
    uint16_t code;
    code = ((args[1] >= 'A' ? args[1] - 'A' : args[1] - 'a') << 10) + 
           ((args[2] >= 'A' ? args[2] - 'A' : args[2] - 'a') << 5);
    if(len == 4) {
        code += args[3] >= 'A' ? args[3] - 'A' : args[3] - 'a';
    }
    return code;
}
int main() {
    printf("%x\n",mylib_regname2number("$eax",4));
    printf("%x\n",mylib_regname2number("$ecx",4));
    printf("%x\n",mylib_regname2number("$edx",4));
    printf("%x\n",mylib_regname2number("$ebx",4));
    printf("%x\n",mylib_regname2number("$esp",4));
    printf("%x\n",mylib_regname2number("$ebp",4));
    printf("%x\n",mylib_regname2number("$esi",4));
    printf("%x\n",mylib_regname2number("$edi",4));
    printf("%x\n",mylib_regname2number("$eip",4));
    return 0;
}


