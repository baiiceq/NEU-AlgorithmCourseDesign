#include "button.h"

#include <iostream>

Button::Button() : x(0), y(0), w(0), h(0),state(State::IDLE), text(L"按钮")
{
}

Button::~Button()
{
}

void Button::on_render()
{
	COLORREF bg_color;
	switch (state)
	{
	case State::IDLE:
		bg_color = RGB(220, 220, 220); 
		break;
	case State::HOVER:
		bg_color = RGB(180, 180, 250);
		break;
	case State::PRESS:
		bg_color = RGB(150, 150, 200);
		break;
	}

	// 绘制按钮背景
	setfillcolor(bg_color);
	solidrectangle(x, y, x + w, y + h);

	// 设置文字颜色
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);

	// 计算文字居中位置
	int text_width = textwidth(text.c_str());
	int text_height = textheight(text.c_str());
	int text_x = x + (w - text_width) / 2;
	int text_y = y + (h - text_height) / 2;

	// 绘制文字
	outtextxy(text_x, text_y, text.c_str());
}

void Button::on_input(const ExMessage& msg)
{
	switch (msg.message)
	{
	case WM_MOUSEMOVE:
		state = is_inside(msg.x, msg.y) ? State::HOVER : State::IDLE;
		break;
	case WM_LBUTTONDOWN:
		if (is_inside(msg.x, msg.y))
			state = State::PRESS;
		break;
	case WM_LBUTTONUP:
		if (state == State::PRESS && is_inside(msg.x, msg.y))
		{
			state = State::HOVER;
			if (on_click)
				on_click();
		}
		else
		{
			state = State::IDLE;
		}
		break;
	}
}

void Button::on_update(int delta)
{
}
