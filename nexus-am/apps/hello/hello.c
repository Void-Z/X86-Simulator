#include <am.h>
int printf(const char *fmt, ...);
void print(const char *s) {
  for (; *s; s ++) {
    _putc(*s);
  }
}
int main() {
  for (int i = 0; i < 10; i ++) {
    print("Hello World!\n");
  }
  char *p = "asdsad";
  printf("asd%s,,%d",p,1);
  return 0;
}
