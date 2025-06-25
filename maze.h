#pragma once

#include "util.h"
#include "random.h"
#include <vector>
#include "tile.h"

class MazeLayer 
{
private:
    int rows, cols;
    std::vector<std::vector<Tile*> > grid;

    Vector2 end_pos;
    Vector2 start_pos;
    std::vector<Vector2> gold_pos;
    std::vector<Vector2> trap_pos;

    void divide(int left, int top, int right, int bottom);

    void generate_entry_and_exit();

	void generate_gold_and_trap();

public:
    MazeLayer(int rows, int cols);

    ~MazeLayer();

    Tile* get_tile(int x, int y) const
    {
        return grid[x][y];
    }

    int getRows() const;
    int getCols() const;

    void generate();  // 分治生成本层

    void on_render(bool is_show_resource);
    void on_update(int delta);

    const std::vector<std::vector<Tile*>>& getGrid() const { return grid; }

    bool isWalkable(int x, int y) const
    {
		return grid[x][y]->is_walkable();
    }

	Vector2 get_start_pos() { return start_pos; }

	Vector2 get_end_pos() { return end_pos; }

	const std::vector<Vector2>& get_gold_pos() const { return gold_pos; }

	const std::vector<Vector2>& get_trap_pos() const { return trap_pos; }

    std::vector<std::vector<TileType> > get_simple_grid() const;

    std::vector<std::vector<int> > get_resource_grid() const;

    std::vector<Vector2> get_coins_pos() const;
};


class Maze  
{
public:
    Maze(int l, int r, int c);

    void generate(int layer);             // 多层分治生成

    void on_render(int layer, bool is_show_resource);

    void on_update(int delta, int layer);

	MazeLayer& get_layer(int layer)
	{
		return maze[layer];
	}

    Vector2 get_start_pos(int layer) { return maze[layer].get_start_pos(); }

    Vector2 get_end_pos(int layer) { return maze[layer].get_end_pos(); }

	std::vector<std::vector<TileType> > get_simple_grid(int layer) const
	{
		return maze[layer].get_simple_grid();
	}

	std::vector<std::vector<int> > get_resource_grid(int layer) const
	{
		return maze[layer].get_resource_grid();
	}

	std::vector<Vector2> get_coins_pos(int layer) const
	{
		return maze[layer].get_coins_pos();
	}

private:
    int layers, rows, cols;
    std::vector<MazeLayer> maze;
};

