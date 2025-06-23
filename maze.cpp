#include "maze.h"
#include <iostream>



MazeLayer::MazeLayer(int rows, int cols, bool isFog) : rows(rows), cols(cols), isFogLayer(isFog)
{
	grid.resize(rows, std::vector<Tile>(cols, Tile(TileType::Empty)));
}

void MazeLayer::divide(int x, int y, int width, int height)
{
	on_render();
	Sleep(80);
	if (width <= 1 || height <= 1) return;

	// 优先从较大的一边分割
	bool prefer_vertical = (width > height);
	bool can_vertical = (width > 1);
	bool can_horizontal = (height > 1);
	bool do_vertical;

	if (width == height) 
	{
		do_vertical = Random::randint(0, 1);
	}
	else 
	{
		do_vertical = (prefer_vertical && can_vertical) || (!can_horizontal && can_vertical);
	}

	const int MAX_RETRY = 10;

	if (!can_vertical && !can_horizontal) return;

	if (do_vertical) 
	{
		if (width < 3) return;

		int wall_x;
		bool valid = false;

		// 尝试多次，找到不含门的横墙位置
		for (int attempt = 0; attempt < MAX_RETRY; ++attempt)
		{
			wall_x = x + Random::randint(1, width - 2);

			bool has_door = false;
			for (int i = max(0, y - 1); i < min(y + height + 1, cols) ; i++)
			{
				if (grid[i][wall_x].get_type() == TileType::DOOR)
				{
					has_door = true;
					break;
				}
			}

			if (!has_door)
			{
				valid = true;
				break;
			}
		}

		if (!valid) return;
		int door_y = y + Random::randint(0, height - 1);   // 门可以在整条墙上

		for (int i = y; i < y + height; i++) 
		{
			if (i == door_y)
			{
				grid[i][wall_x] = TileType::DOOR;
				continue;
			}
			grid[i][wall_x] = TileType::Wall;
		}
		 
		// 递归左右区域
		divide(x, y, wall_x - x, height);                      // 左区域
		divide(wall_x + 1, y, x + width - wall_x - 1, height); // 右区域


	}
	else 
	{
		if (height < 3) return;

		int wall_y;
		bool valid = false;

		// 尝试多次，找到不含门的横墙位置
		for (int attempt = 0; attempt < MAX_RETRY; ++attempt)
		{
			wall_y = y + Random::randint(1, height - 2);

			bool has_door = false;
			for (int i = max(0, x - 1); i < min(x + width + 1, rows); i++) 
			{
				if (grid[wall_y][i].get_type() == TileType::DOOR)
				{
					has_door = true;
					break;
				}
			}

			if (!has_door)
			{
				valid = true;
				break;
			}
		}

		if (!valid) return;

		int door_x = x + Random::randint(0, width - 1);     // 门可以在整条墙上

		for (int i = x; i < x + width; i++)
		{
			if (i == door_x)
			{
				grid[wall_y][i] = TileType::DOOR;
				continue;
			}
			grid[wall_y][i] = TileType::Wall;
		}

		// 递归上下区域
		divide(x, y, width, wall_y - y);                       // 上区域
		divide(x, wall_y + 1, width, y + height - wall_y - 1); // 下区域
	}
}

int MazeLayer::getRows() const
{
	return rows;
}

int MazeLayer::getCols() const
{
	return cols;
}


bool MazeLayer::isFog() const
{
	return isFogLayer;
}

void MazeLayer::generate()
{
	divide(0, 0, cols, rows);
}

void MazeLayer::on_render()
{
	int TILE_SIZE = Grid::getTileSize();
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j].get_type() == TileType::Wall)
			{
				Rect rect = { Grid::toPixelX(i), Grid::toPixelY(j), TILE_SIZE, TILE_SIZE};
				putimage_alpha(ResourcesManager::instance()->find_image("wall"), &rect);
			}
			else
			{
				Rect rect = { Grid::toPixelX(i), Grid::toPixelY(j), TILE_SIZE, TILE_SIZE };
				putimage_alpha(ResourcesManager::instance()->find_image("floor"), &rect);
			}
		}
	}
}

Maze::Maze(int l, int r, int c) :layers(l), rows(r), cols(c)
{
	maze.resize(layers, MazeLayer(rows, cols));
}

void Maze::generate(int layer)
{
	maze[layer].generate();
}

void Maze::on_render(int layer)
{
	maze[layer].on_render();
}

