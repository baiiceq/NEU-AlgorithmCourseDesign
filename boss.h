#pragma once
#include "util.h"
class Boss 
{
public:
    Boss(Position pos, int hp = 30);
    int getHP() const;

private:
    int hp;
    Position pos;
};