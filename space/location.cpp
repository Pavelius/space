#include "main.h"

static location* gp(const char* name) {
	return game::find(name);
}

static location locations[] = {{0, "sun", "������", NoPopulated, Terrain, Gigantic},
{gp("sun"), "earth", "�����", Overpopulated, Terrain, Medium},
{gp("earth"), "moon", "����", Outposts, RockyRealm, Small},
{gp("sun"), "mars", "����", Cities, RockyRealm, Medium},
{gp("sun"), "venera", "������", Colonies, CorrosiveHell, Medium},
};

location* game::find(const char* id) {
	for(auto& e : locations) {
		if(strcmp(e.id, id) == 0)
			return &e;
	}
	exit(0);
	return 0;
}

location* game::chooselocation(location* parent, const char* format, ...) {
	for(auto& e : locations) {
		if(e.parent != parent)
			continue;
		logs::add((int)&e, e.name);
	}
	return (location*)logs::inputv(true, false, true, format, xva_start(format), "\n$(answers)");
}