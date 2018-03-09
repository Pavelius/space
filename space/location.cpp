#include "main.h"

static location* gp(const char* name) {
	return game::find(name);
}

static location locations[] = {{0, "sun", "Солнце", 0, NoPopulated, Terrain, Gigantic},
{gp("sun"), "earth", "Земля", 1500, Overpopulated, Terrain, Medium},
{gp("earth"), "moon", "Луна", 3, Outposts, RockyRealm, Small},
{gp("sun"), "mars", "Марс", 2270, Cities, RockyRealm, Medium},
{gp("sun"), "venera", "Венера", 1080, Colonies, CorrosiveHell, Medium},
};

location* game::find(const char* id) {
	for(auto& e : locations) {
		if(strcmp(e.id, id) == 0)
			return &e;
	}
	exit(0);
	return 0;
}

location* spaceship::chooselocation() {
	auto loc = getlocation();
	auto parent = loc->getparent();
	auto speed = march_speed*50;
	for(auto& e : locations) {
		if(e.parent != parent)
			continue;
		if(&e == loc)
			continue;
		logs::add((int)&e, "%1 (%2i дней)", e.name, e.distance/speed);
	}
	return (location*)logs::inputv(true, false, true, "К какой планете держать курс?", 0, "\n$(answers)");
}