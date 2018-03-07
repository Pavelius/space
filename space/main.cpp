#include "main.h"
#include "io.h"

int	main(int argc, char *argv[]) {
	logs::open("Test");
	game::chooselocation(game::find("sun"));
	return 0;
}

int _stdcall WinMain(void* ci, void* pi, char* cmd, int sw) {
	return main(0, 0);
}