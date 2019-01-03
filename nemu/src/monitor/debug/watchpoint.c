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
	free_ = wp_pool; // 所有的节点都为空
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(char* args) {
	bool success = true;
	int a = expr(args, &success);
	if(!success) {
		printf("ilegal expression! Set watchpoint failed!\n");
		return NULL;
	}
	if(!free_ ) { // 所有节点都已经被分配
		printf("Sorry, this is no space left for a new watchpoint! Set watchpoint failed\n");
		return NULL;	
	}
	WP* result = free_;
	free_ = free_->next;
	result->next = head;
	head = result;
	strcpy(result->expr, args);
	result->value = a;
	return result;
}

void del_wp(int index) {
	if(index >= 0 && index<NR_WP)
		free_wp(&wp_pool[index]);
	else
		printf("Index %d out of range!(0<=index<%d)\n", index, NR_WP);
}

void free_wp(WP *wp) {
	if(!head || !wp) return;

	bool isFound = false;
	WP* present = head;
	if(head->NO == wp->NO)
	{
		head = head->next;
		present->next = free_;
		free_ = present;
		isFound = true;
	}
	else
		while(present->next) {
			if(present->next->NO == wp->NO)
			{
				present->next = present->next->next;
				wp->next = free_;
				free_ = wp;
				isFound = true;
				break;
			}
		}
	if (isFound) {
		printf("The watchpoint of No.%d ( %s ) has been deleted!\n", wp->NO, (wp->expr));
		(wp->expr)[0] = '\0';
		wp->value = 0;
	}
	else {
		printf("The watchpoint of No.%d doesn't exist, delete watchpoint failed!\n", wp->NO);
	}
}

bool check_wp() {
	bool ischanged = false;
	if(!head) return ischanged;
	
	WP* pre = head;
	while(pre) {
		bool success = true;
		int a = expr(pre->expr, &success);
		if(a != pre->value) {
			printf("EXPR: %s\n", pre->expr);
			printf("Previous\t\t\tNow\n");
			printf("%#8x  %d", pre->value, pre->value);
			printf(" <==> ");
			printf("%#8x  %d\n", a, a);
			pre->value = a;
			ischanged = true;
		}
		pre = pre->next;
	}
	return ischanged;
}

void print_wp() {
	WP* pre = head;
	if(!pre) {
		printf("No watvhpoint to print\n");
		return;
	}
	printf("NO\tEXPR\t\tValue\n");
	while(pre) {
		printf("%-d\t%-16s%#08x  %d\n",pre->NO, pre->expr, pre->value, pre->value);
		pre = pre->next;
	}
}


