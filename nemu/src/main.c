#include <stdio.h>
#include <stdlib.h>
#include "monitor/expr.h"
int init_monitor(int, char *[]);
void ui_mainloop(int);

struct {
  unsigned int val;
  char expr_buf[65536];
  unsigned int error;
  unsigned int total;
} expr_test_unit;

void expr_test() {
  printflog("Open test file ...\n");
  FILE *fp = fopen("test.txt", "rt");
  assert(fp != NULL);
  printflog("Open success!\n");
  while(fscanf(fp, "%d %s\n", &expr_test_unit.val, expr_test_unit.expr_buf) != EOF) {
    printf("%d %s\n",expr_test_unit.val,expr_test_unit.expr_buf);
    ++expr_test_unit.total;
    bool * success = (bool *)malloc(sizeof(bool));
    if(expr_test_unit.val != expr(expr_test_unit.expr_buf,success)) {
      ++expr_test_unit.error;
    }
    free(success);
  }
  fclose(fp);
  printflog("Expr() test results : total %d test(s), %d error(s)\n",expr_test_unit.total,expr_test_unit.error);
}

int main(int argc, char *argv[]) {
  /* Test expr(). */
  bool * success = (bool *)malloc(sizeof(bool));
  expr("1",success);
  printf("%d",*success);
  free(success);
  expr_test();

  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
