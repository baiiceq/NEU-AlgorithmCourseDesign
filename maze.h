#pragma once

#include "util.h"
#include "random.h"
#include <vector>

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


class MazeLayer 
{
private:
    int rows, cols;
    std::vector<std::vector<TileType>> grid;
    bool isFogLayer;  // 是否为迷雾层

    void divide(int left, int top, int right, int bottom);

public:
    MazeLayer(int rows, int cols, bool isFog = false);

    TileType getTile(int x, int y) const;
    void setTile(int x, int y, TileType type);

    int getRows() const;
    int getCols() const;
    bool isFog() const;

    void generate();  // 分治生成本层
    void on_render();

    const std::vector<std::vector<TileType>>& getGrid() const { return grid; }

    bool isWalkable(int x, int y) const
    {
		if (grid[x][y] != TileType::Wall)
			return true;

        return false;
    }

};


class Maze  
{
public:
    Maze(int l, int r, int c);

    void generate(int layer);             // 多层分治生成

	void on_render(int layer);     

	const MazeLayer& get_layer(int layer) const
	{
		return maze[layer];
	}
private:
    int layers, rows, cols;
    std::vector<MazeLayer> maze;
};

