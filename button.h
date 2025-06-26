#pragma once
#include <graphics.h>
#include <string>
#include <functional>

#include "util.h"
#include "resources_manager.h"

// ��ť��

class Button 
{
public:
    // ��ť��ǰ״̬����״̬�������ڰ�ť�ϡ������ť��
    enum class State  
    {
        IDLE,
        HOVER,
        PRESS
    };

public:
    Button();
    ~Button();

    void on_render();

    void on_input(const ExMessage& msg);
    
    void on_update(int delta);

    // ���õ����Ļص�����
    void set_on_click(std::function<void()> on_click)
    {
        this->on_click = on_click;
    }

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

    void set_text(const std::wstring& text)
    {
        this->text = text;
    }

private:
    int x, y;
    int w, h;
    std::wstring text;
    State state;

    // �����Ļص�����
    std::function<void()> on_click;

    // ��ǰ���������Ƿ��ڰ�ť��
    bool is_inside(int px, int py) const 
    {
        return (px >= x && px <= x + w && py >= y && py <= y + h);
    }
};