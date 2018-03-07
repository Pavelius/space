#include "main.h"

int damageinfo::roll() {
	auto n1 = damage[0];
	auto n2 = damage[1];
	return n1 + (rand() % (n2 - n1));
}