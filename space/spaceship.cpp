#include "main.h"

using namespace game;

bool spaceship::marshto() {
	auto n = getlocation();
	auto p = n->getparent();
	auto r = game::chooselocation(p, n, "� ����� ������� ������� ����?");
	// ��������� �������
	nextday();
	set(r);
	return true;
}

bool spaceship::encounter() {
	return true;
}