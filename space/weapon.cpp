#include "main.h"

static struct weapon_info {
	const char*	name;
	weapon_type_s type;
	damageinfo	damage;
	damageinfo	upgrade;
} weapon_data[] = {{""},
{"лазер", Energy, {{10, 15}, 1}, {1, 2}},
{"пушка", Shrapnel, {{15, 30}, 1}, {2, 4}},
{"ракеты", Rocket, {{5, 10}, 3}, {1, 2}},
{"торпеда", Rocket, {{30, 40}, 1}, {3, 5}},
};
assert_enum(weapon, Torpedo);
getstr_enum(weapon);

void damageinfo::set(weapon_s id, int level) {
	*this = weapon_data[id].damage;
	damage[0] += weapon_data[id].upgrade.damage[0] * level;
	damage[1] += weapon_data[id].upgrade.damage[1] * level;
	distance += weapon_data[id].upgrade.distance * level;
}

void weapon::get(damageinfo& e) const {
	e.set(type, level);
}

weapon_type_s weapon::gettype() const {
	return weapon_data[type].type;
}