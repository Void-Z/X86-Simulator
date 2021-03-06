#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

WP * new_wp(char *);
void free_wp(WP *);
WP * found_wp(int);
void print_wp();

uint32_t expr(char *, bool *);
/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_si(char *args) {
  int n_steps;
  if(!args) n_steps = 1;
  else {
    n_steps = atoi(args);
    if(n_steps < 1) n_steps = 1;
  }
  cpu_exec(n_steps);
  return 0;
}


static int cmd_x(char *args) {
  if(!args) {
    printf("Invalid Commands!\n");
  } else {
    args = strtok(args," ");
    uint32_t counts = atoi(args);
    args = strtok(NULL," ");
    printf("%s\n",args);
    uint32_t entry = _.atox(args);
    printf("%u %x\n",entry,entry);
    for(uint32_t i = 0;i < counts;++i) {
      printflog("0x%08x\n",*(unsigned *)guest_to_host(entry + i * 4));
    }
  }
  
  return 0;
}

static int cmd_info(char *args) {
  if(!strcmp("r",args)) {
    printflog("eax : 0x%08x ,esp : 0x%08x\n",cpu.eax,cpu.esp);
    printflog("ecx : 0x%08x ,ebp : 0x%08x\n",cpu.ecx,cpu.ebp);
    printflog("edx : 0x%08x ,esi : 0x%08x\n",cpu.edx,cpu.esi);
    printflog("ebx : 0x%08x ,edi : 0x%08x\n",cpu.ebx,cpu.edi);
    printflog("eip : 0x%08x\n",cpu.eip);
  } else if(!strcmp("w",args)) {
    print_wp();
  } else {
    printf("Invalid Commands!\n");
  }
  return 0;
}

static int cmd_p(char *args) {
  bool *success = (bool *)malloc(sizeof(bool));
  uint32_t val = expr(args,success);
  if(*success) {
    printflog("Expression Value is dec:%u, hex:0x%08x\n",val,val);
  } else {
    printflog("Invalid Expression! % d\n",val);
  }
  free(success);
  return 0; 
}

static int cmd_w(char *args) {
  WP *wp = new_wp(args);
  Log("Watchpoint id:%d,now value:0x%08x\n",wp->NO,wp->wp_value);
  return 0;
}

static int cmd_d(char *args) {
  int no = atoi(args);
  WP *wp = found_wp(no);
  Log("Watchpoint delete ! id:%d, expr:%s",wp->NO,wp->wp_str);
  free_wp(wp);
  return 0;
}

static int cmd_help(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },

  /* TODO: Add more commands */
  { "si", "Execution n step(s)", cmd_si},
  { "info", "Show register and watchpoint infomation", cmd_info},
  { "x", "Print memory", cmd_x},
  { "p", "Expression evaluation", cmd_p},
  { "w", "Set watchpoint", cmd_w},
  { "d", "Delete watchpoint", cmd_d}
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
