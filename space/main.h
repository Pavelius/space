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
	OnOrbit, InAmbush, InCosmoport, OnLand,
};
enum duration_s : unsigned char {
	Instant,
	DurationMinute, Duration10Minute,
	DurationHalfHour, DurationHour,
	DurationDay, DurationMonth, DurationYear,
};
enum bay_s : unsigned char {
	NoBay,
	EngeneeringBay, FotifiedStorage, Hangar, Laboratory, LivingCells, LifeSupport, MedicalBay, Manipulator,
	Prison, Radar, Stock, Weaponry,
};
struct location {
	location*		parent;
	const char*		id;
	const char*		name;
	population_s	population;
	landscape_s		landscape;
	size_s			size;
	location*		neighboards[8];
	const char* getname() const { return name; }
};
struct bay {
	bay_s			type;
	unsigned char	level;
	short unsigned	hits;
	operator bool() const { return type != NoBay; }
};
struct bays : adat<bay, 16> {
};
class ship : bays {
	const char*		name;
	short unsigned	hits;
	disposition_s	disposition;
	location*		parent;
public:
	ship() : name() {}
	disposition_s	getdisposition() const { return disposition; }
	short unsigned	gethits() const { return hits; }
	location*		getlocation() const { return parent; }
	location*		marshto();
	void			set(disposition_s value) { disposition = value; }
	void			set(location* value) { parent = value; }
};
namespace game {
void				add(duration_s value);
location*			find(const char* id);
location*			chooselocation(location* parent, const char* format, ...);
}