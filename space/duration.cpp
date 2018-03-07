#include "main.h"

static unsigned start_year = 2200;
static unsigned current_day;

void game::nextday() {
	current_day++;
}

unsigned game::getday() {
	return current_day % 365;
}

unsigned game::getyear() {
	return start_year + (current_day / 365);
}

PRINTPLG(datetime) {
	szprints(result, result_maximum, "ƒень %1i, год %2i", game::getday(), game::getyear());
}