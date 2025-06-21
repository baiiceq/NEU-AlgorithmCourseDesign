#pragma once
#include "maze.h"
#include <string>
#include <functional>
#include "boss.h"

#include "skill.h"

class Player
{
private:
    Position pos;
    int resource;
    int health;
    int maxHealth;

    std::vector<Skill> skills;

public:
    Player(Position start, int hp = 100, int initResource = 0);

    void moveTo(Position pos);
    Position getPosition() const;

    int getResource() const;
    void addResource(int val);

    int getHealth() const;
    void addHealth(int val);
    void reduceHealth(int val);

    std::vector<Skill>& getSkills();
    void resetSkillCooldowns();
    void updateCooldowns();
};
