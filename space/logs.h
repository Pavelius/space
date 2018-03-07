#include "stringcreator.h"

#pragma once

enum gender_s : unsigned char {
	NoGender, Male, Female, Pluar,
};

namespace logs {
void		add(const char* format, ...);
void		add(int id, const char* format, ...);
void		addv(const char* format, const char* param);
void		addv(stringcreator& sc, const char* format, const char* param);
void		addv(int id, const char* format, const char* param);
void		addv(int id, stringcreator& sc, const char* format, const char* param);
void		clear(bool clear_text = false);
int			getcount();
const char*	getpanel(int index); // Defined in main program
char*		getptr();
int			getwidth(int index); // Defined in main program
int			input(bool interactive = true, bool clear_text = true, const char* format = 0, ...);
int			inputsg(bool interactive = true, bool clear_text = true, const char* format = 0, ...);
int			inputv(bool interactive, bool clear_text, bool return_single, const char* format, const char* param, const char* element);
bool		loadart(const char* url);
void		next(bool interactive = true);
void		open(const char* title, bool resize = false);
void		sort();
bool		yesno(bool interactive = true, const char* format = 0, ...);
struct printer : stringcreator {
	gender_s	gender;
	const char* name;
	printer(const char* name, gender_s gender) : name(name), gender(gender) {}
	void parseidentifier(char* result, const char* result_max, const char* identifier) override;
};
}