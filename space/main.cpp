#include "main.h"
#include "io.h"

int	main(int argc, char *argv[]) {
	logs::open("Test");
	auto p1 = game::find("mars");
	ship e;
	e.set(p1);
	e.marshto();
	return 0;
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}