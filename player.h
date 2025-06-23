#pragma once
#include "maze.h"
#include <string>
#include <functional>
#include "boss.h"

#include "skill.h"
#include "vector2.h"
#include "animation.h"
#include <graphics.h>

class Player
{
public:
    Player(Vector2 start = {10, 10}, int hp = 100, int initResource = 0);

    void on_input(const ExMessage& msg, const MazeLayer& ml);

	void on_update(int delta);

	void on_render();

    void move_to(Vector2 pos, const MazeLayer& ml);
    Vector2 getPosition() const;

    int getResource() const;
    void addResource(int val);

    int getHealth() const;
    void addHealth(int val);
    void reduceHealth(int val);

    std::vector<Skill>& getSkills();
    void resetSkillCooldowns();
    void updateCooldowns();

private:
    Vector2 pos;
    Vector2 image_pos;
    int resource;
    int health;
    int maxHealth;

    std::vector<Skill> skills;
      
    bool is_facing_right = true;
    bool is_idle = true;

    Animation anim_idle_left;
    Animation anim_idle_right;
    Animation anim_run_left;
    Animation anim_run_right;

	const int RUN_TIME = 500; // 移动一格的时间 
    Timer timer_move;
};
