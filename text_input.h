#pragma once

#include <string>

#include <graphics.h>

class TextInput
{
public:
	void on_render();
	
	void on_update(int delta);

	void on_input(const ExMessage& msg);

	void set_pos(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void set_size(int w, int h)
	{
		this->w = w;
		this->h = h;
	}

	void set_maxlen(int l)
	{
		maxlen = l;
	}
	
	void set_text_when_blank(const std::wstring text)
	{
		text_when_blank = text;
	}

	std::wstring get_wtext() const
	{
		return text;
	}

	std::string get_text() const
	{
		std::string str(text.begin(), text.end());

		return str;
	}
private:
	 int x, y, w, h;
	std::wstring text;
	std::wstring text_when_blank;
	int cursor_pos = 0;
	bool isFocused = false;
	bool showCursor = true;
	int time = 0;
	int maxlen = 32;
	int blink_time = 500; // π‚±Í…¡À∏º‰∏Ù
};

