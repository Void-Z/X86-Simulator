#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define true 1
#define false 0
typedef uint8_t bool;
const int GEN_NUM_MAX = 10;


// this should be enough
static char buf[65536];
uint32_t pos = 0;
int choose(int range) {
  return rand() % range;
}

static inline void gen(char c) {
  // printf("gen:%c\n",c);
  buf[pos++] = c;
  buf[pos] = '\0';
}

static inline void gen_num() {
  int num = choose(GEN_NUM_MAX);
  if(!num) gen('0');
  // printf("gen_num:%d\n",num);
  bool zero = true;
  while(num) {
    if(zero) {
      if(num % 10) {
        zero = false;
        gen(num % 10 + '0');
      }
    } else {
      gen(num % 10 + '0');
    }
    num /= 10;
  }
}

static inline void gen_rand_op() {
  switch(choose(4)) {
    case 0: {
      gen('+');
      break;
    }
    case 1: {
      gen('-');
      break;
    }
    case 2: {
      gen('*');
      break;
    }
    case 3: {
      gen('/');
      break;
    }
  }
}

static inline void _gen_rand_expr() {
  switch(choose(3)) {
    case 0: {
      gen_num();
      break;
    }
    case 1: {
      gen('(');
      _gen_rand_expr();
      gen(')');
      break;
    }
    case 2: {
      _gen_rand_expr();
      gen_rand_op();
      _gen_rand_expr();
      break;
    }
  }
}

static inline void gen_rand_expr() {
  pos = 0;
  _gen_rand_expr();
}

static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();
    // printf("%s\n",buf);
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen(".code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc .code.c -o .expr");
    if (ret != 0) continue;

    fp = popen("./.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
