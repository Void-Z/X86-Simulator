#include <stdio.h>
#include <stdlib.h>
#include "monitor/expr.h"

#include <regex.h>


int init_monitor(int, char *[]);
void ui_mainloop(int);



int main(int argc, char *argv[]) {
  /* Test expr(). */
  

  /* Initialize the monitor. */
  int is_batch_mode = init_monitor(argc, argv);

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
