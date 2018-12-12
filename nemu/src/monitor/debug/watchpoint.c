#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

WP * new_wp(char *str) {
  assert(free_ != NULL);
  WP *ptr = free_->next;
  ptr->next = head;
  head = free_;
  free_ = ptr;
  strncpy(head->wp_str,str,sizeof(head->wp_str) - 1);
  return head;
}
void free_wp(WP *wp) {
  WP *ptr = head;
  while(ptr->next != wp) {
    ptr = ptr->next;
  }
  ptr->next = wp->next;
  wp->next = free_;
  free_ = wp;
}
WP * found_wp(int id) {
  WP *ptr = head;
  while(ptr) {
    if(ptr->NO == id) {
      return ptr;
    }
    ptr = ptr->next;
  }
  return NULL;
}

/* TODO: Implement the functionality of watchpoint */


