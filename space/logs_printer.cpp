#include "crt.h"
#include "logs.h"

static void msg(gender_s gender, char* result, const char* text_male, const char* text_female, const char* text_pluar) {
	if(gender == Female) {
		if(text_female)
			zcpy(result, text_female);
	} else if(gender == Male) {
		if(text_male)
			zcpy(result, text_male);
	} else if(gender == Pluar) {
		if(text_pluar)
			zcpy(result, text_pluar);
	}
}

void logs::printer::parseidentifier(char* result, const char* result_max, const char* identifier) {
	if(strcmp(identifier, "�����") == 0)
		zcpy(result, name);
	else if(strcmp(identifier, "��") == 0)
		msg(gender, result, "��", identifier, "��");
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
	else if(strcmp(identifier, "��") == 0)
		msg(gender, result, identifier, identifier, "��");
	else if(strcmp(identifier, "��") == 0)
		msg(gender, result, identifier, identifier, "��");
	else {
		zcat(result, "[-");
		zcat(result, identifier);
		zcat(result, "]");
	}
}