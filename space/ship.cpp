#include "main.h"

location* ship::marshto() {
	auto p = game::chooselocation(getlocation(), "� ����� ������� ������� ����?");
	return p;
}