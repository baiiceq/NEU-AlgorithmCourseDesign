#pragma once

#include "util.h"
#include "random.h"
#include <vector>
#include "tile.h"

class MazeLayer 
{
private:
    int rows, cols;
    std::vector<std::vector<Tile*>> grid;
    bool isFogLayer;  // �Ƿ�Ϊ�����

    void divide(int left, int top, int right, int bottom);

public:
    MazeLayer(int rows, int cols, bool isFog = false);

    ~MazeLayer();

    Tile* get_tile(int x, int y) const
    {
        return grid[x][y];
    }

    int getRows() const;
    int getCols() const;
    bool isFog() const;

    void generate();  // �������ɱ���

    void on_render();
    void on_update(int delta);

    const std::vector<std::vector<Tile*>>& getGrid() const { return grid; }

    bool isWalkable(int x, int y) const
    {
		return grid[x][y]->is_walkable();
    }

};


class Maze  
{
public:
    Maze(int l, int r, int c);

    void generate(int layer);             // ����������

    void on_render(int layer);

    void on_update(int delta, int layer);

	const MazeLayer& get_layer(int layer) const
	{
		return maze[layer];
	}
private:
    int layers, rows, cols;
    std::vector<MazeLayer> maze;
};

