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
	if(strcmp(identifier, "герой") == 0)
		zcpy(result, name);
	else if(strcmp(identifier, "ая") == 0)
		msg(gender, result, "ый", identifier, "ые");
	else if(strcmp(identifier, "ась") == 0)
		msg(gender, result, "ся", identifier, "ись");
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
	else if(strcmp(identifier, "ет") == 0)
		msg(gender, result, identifier, identifier, "ут");
	else if(strcmp(identifier, "ит") == 0)
		msg(gender, result, identifier, identifier, "ят");
	else {
		zcat(result, "[-");
		zcat(result, identifier);
		zcat(result, "]");
	}
}