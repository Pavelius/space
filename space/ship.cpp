#include "main.h"

location* ship::marshto() {
	auto p = game::chooselocation(getlocation(), "К какой планете держать курс?");
	return p;
}