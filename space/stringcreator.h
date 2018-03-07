#pragma once

// Macro for correct declaration string indentifier.
#define PRINTPLG(name) static void get_##name(char* result, const char* result_maximum);\
static stringcreator::plugin print_##name = {#name, get_##name};\
static void get_##name(char* result, const char* result_maximum)

struct stringcreator {
	// Custom print tokens set
	struct plugin {
		const char*		name;
		void			(*proc)(char* result, const char* result_maximum);
		plugin*			next;
		static plugin*	first;
		plugin(const char* name, void (*proc)(char* result, const char* result_maximum));
		static plugin*	find(const char* name);
	};
	const char*			parseformat(char* result, const char* result_max, const char* format, const char* format_param);
	virtual void		parseidentifier(char* result, const char* result_max, const char* identifier);
	static char*		parseint(char* dst, const char* result_max, int value, int precision, const int radix);
	static char*		parsenumber(char* dst, const char* result_max, unsigned value, int precision, const int radix);
	virtual void		parsevariable(char* result, const char* result_max, const char** format);
	void				printv(char* result, const char* result_max, const char* format, const char* format_param);
	void				print(char* result, const char* format, ...);
	void				printn(char* result, const char* format, ...);
	void				prints(char* result, const char* result_end, const char* src, ...);
	void				println(char* result, const char* format, ...);
};