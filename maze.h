#pragma once

#include "util.h"
#include "random.h"
#include <vector>
#include "tile.h"

class MazeLayer 
{
private:
    int rows, cols;
    std::vector<std::vector<Tile>> grid;
    bool isFogLayer;  // 是否为迷雾层

    void divide(int left, int top, int right, int bottom);

public:
    MazeLayer(int rows, int cols, bool isFog = false);

    Tile get_tile(int x, int y) const
    {
        return grid[x][y];
    }

    void set_tile(int x, int y, Tile type)
    {
		grid[x][y] = type;
    }

    int getRows() const;
    int getCols() const;
    bool isFog() const;

    void generate();  // 分治生成本层
    void on_render();

    const std::vector<std::vector<Tile>>& getGrid() const { return grid; }

    bool isWalkable(int x, int y) const
    {
		return grid[x][y].is_walkable();
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

