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
	if(strcmp(identifier, "�����") == 0)
		zcpy(result, name);
	else if(strcmp(identifier, "�����") == 0)
		grammar::of(result, name);
	else if(strcmp(identifier, "���") == 0)
		msg(gender, result, "��", identifier, "���");
	else if(strcmp(identifier, "�") == 0)
		msg(gender, result, "", identifier, "�");
	else if(strcmp(identifier, "��") == 0)
		msg(gender, result, "", identifier, "��");
	else if(strcmp(identifier, "���") == 0)
		msg(gender, result, "��", identifier, "���");
	else if(strcmp(identifier, "��") == 0)
		msg(gender, result, "���", identifier, "��");
	else if(strcmp(identifier, "���") == 0)
		msg(gender, result, "����", identifier, "���");
	else {
		zcat(result, "[-");
		zcat(result, identifier);
		zcat(result, "]");
	}
}

PRINTPLG(monster) {
	if(logc.monster) {
		szprint(result, "����� ��� %1i %2.", logc.monster->count, logc.monster->getname());
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
		sc.print(p, "%�����");
		p = zend(p);
		if(!e.isalive())
			sc.print(p, " �����%��");
		else {
			if(e.hp < e.getmaxhits())
				sc.print(p, " ([-%1i]/%2i)", e.hp, e.getmaxhits());
		}
		auto p1 = zend(p); p = p1;
		if(e.armor) {
			sc.print(p, " ����� %1", e.armor.getname(temp, false, true));
			p = zend(p);
		}
		if(e.weapon) {
			sc.print(p, " ������ %1", e.weapon.getname(temp, false, true));
			p = zend(p);
		}
		auto spell_count = e.select(spell_active, spell_active + lenghtof(spell_active));
		for(unsigned i = 0; i < spell_count; i++) {
			if(i == 0) {
				if(p1!=p)
					zcat(p, ", ");
				zcat(p, " ������������");
			}
			else if(i>0 && i == spell_count - 1)
				zcat(p, " � ");
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
		szprint(result, "� ��� ���� [%1i] �����.", hero::getcoins());
		zcat(result, "\n");
	}
	return result;
}