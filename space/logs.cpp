#include "adat.h"
#include "command.h"
#include "crt.h"
#include "draw.h"
#include "logs.h"
#include "stringcreator.h"

namespace logs {
	struct answer {
		int				id;
		const char*		text;
		static int		compare(const void* v1, const void* v2);
	};
};
namespace metrics {
const int		padding = 4;
}

static adat<logs::answer, 128> messages;
static draw::surface picture;
static char	text_buffer[256 * 32];
static char* text_ptr = text_buffer;
extern rect	sys_static_area;
extern bool	sys_optimize_mouse_move;
command* command_logs_clear;
static char content[256 * 8];

enum answer_tokens {
	FirstAnswer = InputUser,
	LastAnswer = FirstAnswer + sizeof(messages.data) / sizeof(messages.data[0])
};

int logs::answer::compare(const void* v1, const void* v2) {
	return strcmp(((answer*)v1)->text, ((answer*)v2)->text);
}

void logs::clear(bool clear_text) {
	if(clear_text)
		content[0] = 0;
	text_ptr = text_buffer;
	messages.clear();
	command_logs_clear->execute();
}

static char* ending(char* p, const char* string) {
	char* p1 = (char*)zright(p, 1);
	if(p1[0] != '.' && p1[0] != '?' && p1[0] != '!' && p1[0] != ':' && p1[0] != 0)
		zcat(p1, string);
	return p;
}

void logs::sort() {
	qsort(messages.data, messages.count, sizeof(messages.data[0]), answer::compare);
}

void logs::addv(int id, stringcreator& sc, const char* format, const char* param) {
	logs::answer* e = messages.add();
	if(!e)
		return;
	memset(e, 0, sizeof(logs::answer));
	e->id = id;
	szprintv(text_ptr, endofs(text_buffer), format, param);
	sc.printv(text_ptr, endofs(text_buffer) - 1, format, param);
	szupper(text_ptr, 1);
	e->text = ending(text_ptr, ".");
	text_ptr = zend(text_ptr) + 1;
}

void logs::addv(int id, const char* format, const char* param) {
	stringcreator sc;
	addv(id, sc, format, param);
}

void logs::add(int id, const char* format ...) {
	addv(id, format, xva_start(format));
}

char* logs::getptr() {
	return zend(content);
}

void logs::addv(stringcreator& sc, const char* format, const char* param) {
	if(!format || format[0] == 0)
		return;
	char* p = getptr();
	// First string may not be emphty or white spaced
	if(p == content)
		format = zskipspcr(format);
	if(format[0] == 0)
		return;
	if(p != content) {
		if(p[-1] != ' ' && p[-1] != '\n' && *format != '\n' && *format != '.' && *format != ',') {
			*p++ = ' ';
			*p = 0;
		}
		if(p[-1] == '\n' && format[0] == '\n')
			format++;
		if(p[-1] == ' ' && format[0] == ' ')
			format++;
	}
	sc.printv(p, content + sizeof(content) - 1, format, param);
}

void logs::addv(const char* format, const char* param) {
	stringcreator sc;
	addv(sc, format, param);
}

void logs::add(const char* format, ...) {
	addv(format, xva_start(format));
}

void logs::open(const char* title, bool resize) {
	sys_optimize_mouse_move = true;
	draw::create(-1, -1, 800, 600, WFResize|WFMinmax, 32);
	draw::font = metrics::font;
	draw::fore = colors::text;
	draw::setcaption(title);
	clear();
}

static char* letter(char* result, const char* result_max, int n) {
	if(n < 9)
		return szprints(result, result_max, "%1i)", n + 1);
	result[0] = 'A' + (n - 9);
	result[1] = ')';
	result[2] = 0;
	return result;
}

static int wdt_answer_proc(int x, int y, int width, int id, int* max_width) {
	draw::state push;
	char result[32];
	auto y0 = y;
	auto x0 = x;
	auto x2 = x + width;
	x += metrics::padding;
	auto i = id - FirstAnswer;
	letter(result, endofs(result), i);
	draw::fore = colors::text.mix(colors::special, 164);
	draw::text(x, y, result);
	draw::fore = colors::text;
	auto x1 = x + draw::textw("AZ)");
	rect rc = {x1, y, x2, y};
	int dy = draw::textf(rc, messages[i].text);
	areas a = draw::area(rc);
	if(a == AreaHilited || a == AreaHilitedPressed) {
		if(a == AreaHilitedPressed) {
			hot::pressed = false;
			draw::execute(i + FirstAnswer);
		}
		draw::rectf({rc.x1 - 2, rc.y1 - 2, rc.x2 + 2, rc.y2 + 2}, colors::edit, 16);
		draw::rectb({rc.x1 - 2, rc.y1 - 2, rc.x2 + 2, rc.y2 + 2}, colors::border.mix(colors::window, 128));
	}
	draw::textf(x1, y, x2 - x1, messages[i].text);
	y += dy + metrics::padding;
	if(max_width)
		*max_width = rc.x2 - x0;
	return y - y0;
}

TEXTPLUGIN(answers) {
	auto column_count = 1 + messages.getcount() / 13;
	auto medium_width = width / column_count;
	auto column_width = medium_width - metrics::padding;
	auto rows_count = messages.getcount() / column_count;
	auto index = 0;
	auto y0 = y;
	for(auto column = 0; column < column_count; column++) {
		y = y0;
		auto max_width = 0;
		for(auto row = 0; row < rows_count; row++) {
			int answer_width;
			y += wdt_answer_proc(x, y, column_width, index + FirstAnswer, &answer_width);
			if(max_width < answer_width)
				max_width = answer_width;
			index++;
		}
		if(column != column_count - 1)
			x += max_width + metrics::padding;
	}
	while(index < messages.getcount()) {
		y += wdt_answer_proc(x, y, column_width, index + FirstAnswer, 0);
		index++;
	}
	return 0;
}

static int render_input() {
	char temp[4096];
	while(true) {
		int x = 0;
		int y = 0;
		int y2 = draw::getheight();
		int x2 = draw::getwidth();
		draw::rectf({x, y, x2, y2}, colors::window);
		x += metrics::padding;
		y += metrics::padding;
		x2 -= metrics::padding;
		y2 -= metrics::padding;
		int left_width = logs::getwidth(0);
		auto panel_information = logs::getpanel(0);
		if(picture)
			left_width = picture.width;
		else if(panel_information) {
			if(!left_width)
				left_width = 300;
		}
		if(left_width) {
			int y1 = metrics::padding;
			int x1 = x2 - left_width;
			if(picture) {
				draw::blit(*draw::canvas, x1, metrics::padding, picture.width, picture.height, 0, picture, 0, 0);
				draw::rectb({x1, y1, x2, y1 + picture.height}, colors::border);
				y1 += picture.height + metrics::padding;
			}
			// Left panel
			if(panel_information) {
				szprints(temp, endofs(temp), panel_information);
				y1 += draw::textf(x1, y1, x2 - x1, temp);
			}
			x2 = x1 - metrics::padding;
		}
		y += draw::textf(x, y, x2 - x, content);
		int id = draw::input();
		if(!id)
			exit(0);
		else if(id >= FirstAnswer && id <= LastAnswer) {
			if((id - FirstAnswer) < messages.getcount())
				return messages[id - FirstAnswer].id;
		} else if(id == InputSymbol) {
			auto result = -1;
			int sym = szupper(hot::param);
			if(sym >= '1' && sym <= '9')
				draw::execute(FirstAnswer + (sym - '1'));
			else if(sym >= 'A' && sym <= 'A' + (messages.getcount() - 10))
				draw::execute(FirstAnswer + 9 + sym - 'A');
		} else if(id < FirstInput)
			return id; // События от прочих элементов упавления
	}
}

static void correct(char* p) {
	bool need_uppercase = true;
	for(; *p; p++) {
		if(*p == '.' || *p == '?' || *p == '!' || *p == '\n') {
			p = (char*)zskipspcr(p + 1);
			if(*p != '-')
				need_uppercase = true;
		}
		if(*p == ' ' || *p == '-' || *p == '\t')
			continue;
		if(need_uppercase) {
			szupper(p, 1);
			need_uppercase = false;
		}
	}
}

int	logs::getcount() {
	return messages.getcount();
}

int logs::inputv(bool interactive, bool clear_text, bool return_single, const char* format, const char* param, const char* element) {
	int r = 0;
	if(return_single && messages.getcount() == 1) {
		// Fast return single element
		r = messages[0].id;
		clear(clear_text);
		return r;
	}
	char* p = zend(content);
	while(p > content && p[-1] == '\n')
		*--p = 0;
	if(format && format[0] && interactive) {
		if(p != content)
			zcat(p, "\n");
		logs::addv(format, param);
	}
	correct(content);
	if(element)
		zcat(content, element);
	if(interactive)
		r = render_input();
	else if(messages.getcount())
		r = messages[rand() % (messages.getcount())].id;
	p[0] = 0;
	clear(clear_text);
	return r;
}

int logs::input(bool inveractive, bool clear_text, const char* format, ...) {
	return inputv(inveractive, clear_text, false, format, xva_start(format), "\n$(answers)");
}

int logs::inputsg(bool inveractive, bool clear_text, const char* format, ...) {
	return inputv(inveractive, clear_text, true, format, xva_start(format), "\n$(answers)");
}

bool logs::loadart(const char* url) {
	char temp[260];
	if(zchr(url, '.'))
		zcpy(temp, url);
	else
		szprints(temp, endofs(temp), "art/%1.png", url);
	picture.read(temp);
	if(!picture)
		return false;
	picture.convert(-32, 0);
	return true;
}

void logs::next(bool interactive) {
	logs::add(1, "Далее");
	logs::input(interactive);
}

bool logs::yesno(bool interactive, const char* format, ...) {
	add(1, "Да");
	add(2, "Нет");
	return inputv(interactive, true, false, format, xva_start(format), "\n$(answers)") == 1;
}