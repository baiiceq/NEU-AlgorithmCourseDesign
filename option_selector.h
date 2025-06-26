#pragma once
#include <graphics.h>
#include <string>
#include <vector>
#include <functional>

#include "Button.h"

class OptionSelector 
{

public:
    OptionSelector();
    ~OptionSelector();

    void on_render();

    void on_input(const ExMessage& msg);

    void set_position(Vector2 pos)
    {
        position = pos;

        update_button_layout();
    }

    void set_size(Vector2 s)
    {
        size = s;

        update_button_layout();
    }


    // 设置回调函数
    void set_callback(std::function<void()> callback)
    {
        on_select_callback = callback;
    }

    // 设置当前选中项
    void SetSelected(int index)
    {
        if (index >= 0 && index < options.size()) {
            selected = index;
        }
    }

    // 获取当前选中项索引
    int GetSelected() const
    {
        return selected;
    }

    // 获取当前选项文本
    std::wstring GetSelectedText() const
    {
        return options[selected];
    }

    void set_options(const std::vector<std::wstring>& opts)
    {
        options = opts;
    }

private:
    // 更新按钮大小和位置
    void update_button_layout();

private:
    Vector2 position;
    Vector2 size;
    std::vector<std::wstring> options;
    int selected = 0;

    std::function<void()> on_select_callback = nullptr;

    Button left;
    Button right;
};
