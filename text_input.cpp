#include "text_input.h"
#include "util.h"

#pragma comment(lib, "imm32.lib")

void TextInput::on_render()
{
	setlinecolor(LIGHTGRAY);
	setbkcolor(0xeeeeee);
	setfillcolor(0xeeeeee);
	fillrectangle(x, y, x + w, y + h);

	int font_height = h - 10;
	if (font_height < 10) font_height = 10;
	settextstyle(font_height, 0, L"ºÚÌå");

	settextcolor(BLACK);
	outtextxy(x + 10, y + 5, text.c_str());

	if (!isFocused && text.empty())
	{
		settextcolor(0x050505);
		outtextxy(x + 10, y + 5, text_when_blank.c_str());
	}

	if (isFocused && showCursor)
	{
		if (cursor_pos > text.length()) cursor_pos = (int)text.length();
		int tw = textwidth(text.substr(0, cursor_pos).c_str());
		int cx = x + 10 + tw;
		int cy1 = y + 5;
		int cy2 = y + h - 5;
		setlinecolor(BLACK);
		line(cx, cy1, cx, cy2);
	}
}

void TextInput::on_update(int delta)
{
	time += delta;
	if (time >= blink_time)
	{
		time -= blink_time;
		showCursor = !showCursor;
	}
}

void TextInput::on_input(const ExMessage& msg)
{
	switch (msg.message)
	{
	case WM_LBUTTONDOWN:
		if (msg.x >= x && msg.x <= x + w &&
			msg.y >= y && msg.y <= y + h)
		{
			HWND hwnd = GetHWnd();
			ImmAssociateContext(hwnd, NULL);
			isFocused = true;
		}
		else
		{
			isFocused = false;
		}
		break;

	case WM_KEYDOWN:
		if (!isFocused) break;
		if (msg.vkcode == VK_LEFT && cursor_pos > 0)
			cursor_pos--;
		else if (msg.vkcode == VK_RIGHT && cursor_pos < text.length())
			cursor_pos++;
		break;

	case WM_CHAR:
		if (!isFocused) break;

		if (msg.ch == 8) // Backspace
		{
			if (cursor_pos > 0)
			{
				text.erase(cursor_pos - 1, 1);
				cursor_pos--;
			}
		}
		else if (msg.ch == 13) // Enter
		{
			isFocused = false;
		}
		else if ((msg.ch >= '0' && msg.ch <= '9') || msg.ch == '.')
		{
			if (text.length() < maxlen)
			{
				text.insert(cursor_pos, 1, msg.ch);
				cursor_pos++;
			}
		}
		if (cursor_pos > text.length()) cursor_pos = (int)text.length();
		break;
	}
}


