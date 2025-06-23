#pragma once

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

    bool is_walkable() const
    {
        return type != TileType::Wall;
    }

private:
    TileType type;
};

class Gold : public Tile
{
public:
	Gold(int value = 5) : Tile(TileType::Gold), value(value) {}

	int get_value() const { return value; }
	void set_value(int new_value) { value = new_value; }

private:
	int value;  // 金币的价值
};

class Trap : public Tile
{
public:
	Trap(int damage = 10) : Tile(TileType::Trap), damage(damage) {}

	int get_damage() const { return damage; }
	void set_damage(int new_damage) { damage = new_damage; }

private:
	int damage;  // 陷阱造成的伤害
};