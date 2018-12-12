#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#include <stdlib.h>

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
  free_->next = head;
  head = free_;
  free_ = ptr;
  strncpy(head->wp_str,str,sizeof(head->wp_str) - 1);
  bool *success = (bool *)malloc(sizeof(bool));
  head->wp_value = expr(str,success);
  free(success);
  return head;
}
void free_wp(WP *wp) {
  WP *ptr = head;
  if(wp == head) {
    head = head->next;
  } else {
    while(ptr->next != wp) {
      ptr = ptr->next;
    }
    ptr->next = wp->next;
  }
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
bool hit_wp_check() {
  WP *ptr = head;
  bool result = false;
  bool *success = (bool *)malloc(sizeof(bool));
  while(ptr) {
    uint32_t val = expr(ptr->wp_str,success);
    if(val != ptr->wp_value) {
      Log("Hit watchpoint %d:value from 0x%08x to 0x%08x.\n",ptr->NO,ptr->wp_value,val);
      result = true;
    }
    ptr = ptr->next;
  }
  free(success);
  return result;
}
void print_wp() {
  WP *ptr = head;
  while(ptr) {
    Log("Watchpoint NO.%d %s 0x%08x\n",ptr->NO,ptr->wp_str,ptr->wp_value);
    ptr = ptr->next;
  }
}

/* TODO: Implement the functionality of watchpoint */


