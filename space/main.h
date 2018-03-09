#include "acol.h"
#include "adat.h"
#include "aref.h"
#include "crt.h"
#include "logs.h"

#pragma once

enum population_s : unsigned char {
	NoPopulated, Outposts, Colonies, Cities, Overpopulated,
};
enum landscape_s : unsigned char {
	Terrain, ColdRealm, SandRealm, WaterRealm, FoggyJungles, RockyRealm, CorrosiveHell,
};
enum size_s : unsigned char {
	Diminutive, Small, Medium, Large, Huge, Gigantic,
};
enum disposition_s : unsigned char {
	OnOrbit, NearTo, InCosmoportOf, OnSurfaceOf,
};
enum bay_s : unsigned char {
	NoBay,
	EngeneeringBay, FotifiedStorage, Hangar, Laboratory, LivingCells, LifeSupport, MedicalBay, Manipulator,
	Prison, Radar, Stock, Weaponry,
};
enum weapon_type_s : unsigned char {
	Energy, Shrapnel, Rocket,
};
enum weapon_s : unsigned char {
	NoWeapon,
	Laser, ShrapnelCannon, RocketLauncher, Torpedo
};
struct location {
	location*		parent;
	const char*		id;
	const char*		name;
	unsigned		distance;
	population_s	population;
	landscape_s		landscape;
	size_s			size;
	location*		neighboards[8];
	location*		getparent() const { return parent; }
	const char*		getname() const { return name; }
};
struct status {
	short unsigned	hits;
	unsigned char	level;
	short unsigned	getmaximum() const;
};
struct bay : status {
	bay_s			type;
	constexpr bay(bay_s type = NoBay) : status{100, 0}, type(type) {}
	operator bool() const { return type != NoBay; }
};
typedef adat<bay, 11> baya;
struct damageinfo {
	unsigned char	damage[2];
	unsigned char	count;
	unsigned char	distance;
	void			set(weapon_s id, int level);
	int				roll();
};
struct weapon : status {
	weapon_s		type;
	unsigned char	level;
	constexpr weapon(weapon_s type = NoWeapon) : type(type), level(0) {}
	void			get(damageinfo& e) const;
};
typedef weapon		weapona[6];
struct ship {
	const char*		id;
	const char*		name;
	size_s			size;
	status			stat;
	char			armor;
	char			march_speed; // Скорость маршевых двигателей (4 - 10)
	char			hyper_speed; // Скорость в гипер пространстве (0 - 3)
	char			weapon_slots; // Количество слотов оружия
	char			bay_slots; // Количество отсеков
	weapona			weapons;
	baya			bays;
};
class spaceship : ship {
	short unsigned	hits_maximum;
	disposition_s	disposition;
	location*		parent;
public:
	spaceship(const char* id);
	location*		chooselocation();
	bool			encounter();
	disposition_s	getdisposition() const { return disposition; }
	short unsigned	gethits() const { return stat.hits; }
	short unsigned	gethitsmax() const { return hits_maximum; }
	location*		getlocation() const { return parent; }
	bool			marshto();
	void			set(disposition_s value) { disposition = value; }
	void			set(location* value) { parent = value; }
};
namespace game {
location*			chooselocation(location* parent, location* exclude, const char* format, ...);
location*			find(const char* id);
unsigned			getday();
unsigned			getyear();
void				nextday();
}