#include "main.h"

int logs::getwidth(int panel) {
	return 300;
}

const char* logs::getpanel(int panel) {
	return "%party%monster%party_money";
}

static void msg(gender_s gender, char* result, const char* text_male, const char* text_female, const char* text_pluar) {
	if(gender == Female) {
		if(text_female)
			zcpy(result, text_female);
	} else {
		if(text_male)
			zcpy(result, text_male);
	}
}

void logs::printer::parseidentifier(char* result, const char* result_max, const char* identifier) {
	if(strcmp(identifier, "герой") == 0)
		zcpy(result, name);
	else if(strcmp(identifier, "геро€") == 0)
		grammar::of(result, name);
	else if(strcmp(identifier, "ась") == 0)
		msg(gender, result, "с€", identifier, "ись");
	else if(strcmp(identifier, "а") == 0)
		msg(gender, result, "", identifier, "и");
	else if(strcmp(identifier, "ла") == 0)
		msg(gender, result, "", identifier, "ли");
	else if(strcmp(identifier, "она") == 0)
		msg(gender, result, "он", identifier, "они");
	else if(strcmp(identifier, "ее") == 0)
		msg(gender, result, "его", identifier, "их");
	else if(strcmp(identifier, "нее") == 0)
		msg(gender, result, "него", identifier, "них");
	else {
		zcat(result, "[-");
		zcat(result, identifier);
		zcat(result, "]");
	}
}

PRINTPLG(monster) {
	if(logc.monster) {
		szprint(result, "ќколо вас %1i %2.", logc.monster->count, logc.monster->getname());
		zcat(result, "\n");
	}
	return result;
}

PRINTPLG(party) {
	char temp[260];
	result[0] = 0;
	auto p = result;
	for(auto& e : players) {
		if(!e)
			continue;
		spell_state* spell_active[32];
		logs::printer sc(e.getname(), e.gender);
		sc.print(p, "%герой");
		p = zend(p);
		if(!e.isalive())
			sc.print(p, " погиб%ла");
		else {
			if(e.hp < e.getmaxhits())
				sc.print(p, " ([-%1i]/%2i)", e.hp, e.getmaxhits());
		}
		auto p1 = zend(p); p = p1;
		if(e.armor) {
			sc.print(p, " носит %1", e.armor.getname(temp, false, true));
			p = zend(p);
		}
		if(e.weapon) {
			sc.print(p, " держит %1", e.weapon.getname(temp, false, true));
			p = zend(p);
		}
		auto spell_count = e.select(spell_active, spell_active + lenghtof(spell_active));
		for(unsigned i = 0; i < spell_count; i++) {
			if(i == 0) {
				if(p1!=p)
					zcat(p, ", ");
				zcat(p, " поддерживает");
			}
			else if(i>0 && i == spell_count - 1)
				zcat(p, " и ");
			else
				zcat(p, ", ");
			p = zend(p);
			zcpy(p, getstr(spell_active[i]->spell));
		}
		zcat(p, ".\n");
		p = zend(p);
	}
	return result;
}

PRINTPLG(party_money) {
	auto value = hero::getcoins();
	if(value) {
		szprint(result, "” вас есть [%1i] монет.", hero::getcoins());
		zcat(result, "\n");
	}
	return result;
}