#pragma once
#include <string>
#include <functional>
#include "boss.h"

#include "skill.h"
#include "vector2.h"
#include "animation.h"
#include <graphics.h>

class MazeLayer;

class Player
{
public:
    Player(Vector2 start = {0, 0}, int hp = 100, int initResource = 0);

    void on_input(const ExMessage& msg);


    void on_update(int delta, MazeLayer& ml);

    void on_render();

    bool move_to(Vector2 pos, MazeLayer& ml);

	void set_position(Vector2 pos);
    Vector2 getPosition() const;

    int get_resource() const
    {
		return resource;
    }
    void add_resource(int val)
    {
        resource += val;
        resource = max(0, resource);
    }

    int getHealth() const;
    void addHealth(int val);
    void reduceHealth(int val);

    std::vector<Skill>& getSkills();
    void resetSkillCooldowns();
    void updateCooldowns();

private:
    Vector2 pos;
    Vector2 image_pos;
    Vector2 last_image_pos;
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

    bool is_running_right = false;
    bool is_running_left = false;
    bool is_running_up = false;
    bool is_running_down = false;

	const int RUN_TIME = 300; // 移动一格的时间 
    Timer timer_move;

    const int OFFESET_X = 21;
    const int OFFESET_Y = 39;
};
