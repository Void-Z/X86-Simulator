#include <common.h>

void main_loop();
void hal_init();

int
main(void) {
	Log("game start!");

  hal_init();
	Log("hal init!");
	main_loop();

	return 0;
}
