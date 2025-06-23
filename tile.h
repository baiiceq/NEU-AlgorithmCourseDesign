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

    virtual void on_render() = 0;

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

private:
	int value;  // ��ҵļ�ֵ
	bool is_triggered = false;  // �Ƿ񱻴�����

    Animation anim_gold;
};

class Trap : public Tile
{
public:
	Trap(int damage = 10) : Tile(TileType::Trap), damage(damage) {}

	int get_damage() const { return damage; }
	void set_damage(int new_damage) { damage = new_damage; }

    void set_triggered(bool triggered) { is_triggered = triggered; }

private:
	int damage;  // ������ɵ��˺�
    bool is_triggered = false;  // �Ƿ񱻴�����
};