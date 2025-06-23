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