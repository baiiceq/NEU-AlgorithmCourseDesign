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
    Vector2 locker_pos;
    Vector2 boss_pos;

    void divide(int left, int top, int right, int bottom, bool is_show_process);

    void generate_entry_and_exit();

    void generate_gold_and_trap();

    void generate_lock_and_boss();

public:
    MazeLayer(int rows, int cols);

    MazeLayer(const MazeLayer& ml);

    ~MazeLayer();

    Tile* get_tile(int x, int y) const
    {
        return grid[x][y];
    }

    void reset(int row, int col);

    void load_maze_from_json(std::wstring filename);

    int getRows() const;
    int getCols() const;

    void generate(bool is_show_process = true);  // 分治生成本层

    void on_render(bool is_show_resource);
    void on_update(int delta);

    const std::vector<std::vector<Tile*>>& getGrid() const { return grid; }

    bool isWalkable(int x, int y) const
    {
		return grid[x][y]->is_walkable();
    }

	Vector2 get_start_pos() { return start_pos; }

    Vector2 get_end_pos() { return end_pos; }

    Vector2 get_locker_pos() { return locker_pos; }

	const std::vector<Vector2>& get_gold_pos() const { return gold_pos; }

	const std::vector<Vector2>& get_trap_pos() const { return trap_pos; }

    int get_password()
    {
        Locker* locker = static_cast<Locker*>(grid[locker_pos.x][locker_pos.y]);
        return locker->get_password();
    }

    std::vector<std::vector<int> > get_clue()
    {
        Locker* locker = static_cast<Locker*>(grid[locker_pos.x][locker_pos.y]);
        return locker->get_clue();
    }

    std::vector<std::vector<TileType> > get_simple_grid() const;

    std::vector<std::vector<int> > get_resource_grid() const;

    std::vector<Vector2> get_coins_pos() const;

public:
    static void save_maze_to_json(const std::vector<std::vector<TileType> >& maze, const std::string& filename, unsigned int seed, MazeLayer& ml);

    static void generate_multiple_mazes(int count, int rows, int cols);
};


class Maze  
{
public:
    Maze(int l = 2, int r = 7, int c = 7);

    void generate(int layer);             // 多层分治生成

    void on_render(int layer, bool is_show_resource);

    void on_update(int delta, int layer);

    void reset(int layer, int row, int col);

    void load_maze_from_json(std::wstring filename);

    int get_row() const
    {
        return rows;
    }

    int get_col() const
    {
        return cols;
    }

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

    Vector2 get_locker_pos(int layer)
    {
        return maze[layer].get_locker_pos();
    }

    int get_password(int layer)
    {
        return maze[layer].get_password();
    }

    std::vector<std::vector<int> > get_clue(int layer)
    {
        return maze[layer].get_clue();
    }
private:
    int layers, rows, cols;
    std::vector<MazeLayer> maze;
};

