#pragma once
#include "player.h"
class Trap
{
public:
    static const int penalty = -3;
    static void applyEffect(Player& player);
	static void applyEffect(Player& player, Boss& boss);
};
