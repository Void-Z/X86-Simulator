#include "common.h"
_Context* do_syscall(_Context *c);
static _Context* do_event(_Event e, _Context* c) {
  switch (e.event) {
    case _EVENT_SYSCALL: {
      printf("syscall,id = %d\n",e.event);
      do_syscall(c);
      break;
    }
    case _EVENT_YIELD: {
      printf("yield,id = %d\n",e.event);
      _yield();
      break;
    }
    case _EVENT_ERROR: {
      printf("error,id = %d\n",e.event);
      break;
    }
    default: panic("Unhandled event ID = %d", e.event);
  }

  return NULL;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
