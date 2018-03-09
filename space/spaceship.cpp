#include "main.h"

using namespace game;

bool spaceship::marshto() {
	auto n = getlocation();
	auto p = n->getparent();
	auto r = chooselocation();
	// ��������� �������
	nextday();
	set(r);
	return true;
}

bool spaceship::encounter() {
	return true;
}

void spaceship::damage(bool interactive, int value, weapon_type_s type) {
	value -= armor;
	stat.hits -= value;
	if(stat.hits>0) {
		if(interactive) {
			act("������� ������� %1i �����������.", value);
		}
	} else {
		if(interactive) {
			act("������� ��� ���������.");
		}
	}
}

void spaceship::shoot(bool interactive, weapon& e, spaceship& enemy) {
	damageinfo d; e.get(d);
	enemy.damage(interactive, d.roll(), e.gettype());
}