#pragma once
#include "animation.h"

enum class TileType
{
    Wall,
    Path,
    Start,
    Exit,
    Gold,
    Trap,
    Locker,
    Boss,
    Empty = -1,
    DOOR = -2
};

class Tile
{
public:
    Tile(TileType type = TileType::Path) : type(type) {}

    TileType get_type() const { return type; }
    void set_type(TileType new_type) { type = new_type; }
    
    void set_pos(Vector2 pos)
    {
        this->pos = pos;
    }

    virtual void on_render();

    virtual void on_update(int delta);

    bool is_walkable() const
    {
        return type != TileType::Wall;
    }

protected:
    TileType type;
    Vector2 pos;
};


class Gold : public Tile
{
public:
    Gold(int value = 5);

	int get_value() const { return value; }
	void set_value(int new_value) { value = new_value; }

	void set_triggered(bool triggered) { is_triggered = triggered; }

    void on_render() override;
    void on_update(int delta) override;

private:
	int value;  // 金币的价值
	bool is_triggered = false;  // 是否被触发过

    Animation anim_gold;

    const int OFFSET_X = 20;
    const int OFFSET_Y = 30;
};

class Trap : public Tile
{
public:
	Trap(int damage = 10) : Tile(TileType::Trap), damage(damage) {}

	int get_damage() const { return damage; }
	void set_damage(int new_damage) { damage = new_damage; }

    void set_triggered(bool triggered) { is_triggered = triggered; }

private:
	int damage;  // 陷阱造成的伤害
    bool is_triggered = false;  // 是否被触发过
};