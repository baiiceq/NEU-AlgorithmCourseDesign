#pragma once
#include "util.h"
class Boss 
{
public:
    Boss(Vector2 pos, int hp = 30);
    int getHP() const;

private:
    int hp;
    Vector2 pos;
};