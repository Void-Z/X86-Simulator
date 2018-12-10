#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
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
  printf("si %d step(s).\n",n_steps);
  cpu_exec(n_steps);
  return 0;
}

uint32_t atox(char *args) {
  uint32_t val = 0;
  args += 2;
  for(;*args;++args) {
    val *= 16;
    val += *args - '0';
  }
  return val;
}

static int cmd_x(char *args) {
  if(!args) {
    printf("Invalid Commands!\n");
  } else {
    args = strtok(args," ");
    uint32_t counts = atoi(args);
    args = strtok(NULL," ");
    printf("%s\n",args);
    uint32_t entry = atox(args);
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
    printf("Watchpoint mods has no implemention.\n");
  } else {
    printf("Invalid Commands!\n");
  }
  return 0;
}

static int cmd_p(char *args) {
  bool *success = (bool *)malloc(sizeof(bool));
  uint32_t val = expr(/*args*/"1",success);
  if(*success) {
    printflog("Expression Value : %u\n",val);
  } else {
    printflog("Invalid Expression!\n");
  }
  free(success);
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
  { "si", "Format : si [N], Description : Let the program suspend execution after stepping through N instructions. When N is not given, the default is 1", cmd_si},
  { "info", "Format : info SUBCMD, Description : r-Print registers status,w-Print watchpoint infomation", cmd_info},
  { "x", "Format : x N EXPR, Description : Find the value of the expression EXPR, and use the result as the starting memory address to output consecutive N 4 bytes in hexadecimal format.", cmd_x},
  { "p", "Format : p EXPR, Description : Expression evaluation", cmd_p},
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
