#pragma once
#include <string>
#include <functional>

#include "skill.h"
#include "vector2.h"
#include "animation.h"
#include <graphics.h>

class MazeLayer;

class Player
{
public:
    Player(Vector2 start = {0, 0}, int hp = 100, int init_resource = 1000);

    void on_input(const ExMessage& msg);


    void on_update(int delta, MazeLayer& ml);

    void on_render();

    bool move_to(Vector2 pos, MazeLayer& ml);

	void set_position(Vector2 pos);
    Vector2 get_pos() const
    {
        return pos;
    }

    int get_resource() const
    {
		return resource;
    }
    void add_resource(int val)
    {
        resource += val;
        resource = max(0, resource);
    }

    bool get_is_exit() const
    {
		return is_exit;
    }

	void set_is_exit(bool exit)
	{
		is_exit = exit;
	}

    void set_is_locker(bool b)
    {
        is_locker = b;
    }

	bool get_is_locker() const
	{
		return is_locker;
	}

    void run_reset()
    {
        is_running_down = false;
        is_running_up = false;
        is_running_right = false;
        is_running_left = false;
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

    bool is_exit = false;
    bool is_locker = false;

	const int RUN_TIME = 150; // 移动一格的时间 
    Timer timer_move;

    const int OFFESET_X = 21;
    const int OFFESET_Y = 39;

};
