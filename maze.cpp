#include "maze.h"
#include <iostream>



MazeLayer::MazeLayer(int rows, int cols) : rows(rows), cols(cols)
{
	grid.resize(rows, std::vector<Tile*>(cols, new Tile()));

	Tile* shared = grid[0][0]; 
	delete shared;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grid[i][j] = new Tile();
			grid[i][j]->set_pos({ (float)i, (float)j });
		}
	}

}

MazeLayer::~MazeLayer()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			delete grid[i][j];
		}
	}
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
				if (grid[i][wall_x]->get_type() == TileType::DOOR)
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
				grid[i][wall_x]->set_type(TileType::DOOR);
				continue;
			}
			grid[i][wall_x]->set_type(TileType::Wall);
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
				if (grid[wall_y][i]->get_type() == TileType::DOOR)
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
				grid[wall_y][i]->set_type(TileType::DOOR);
				continue;
			}
			grid[wall_y][i]->set_type(TileType::Wall);
		}

		// 递归上下区域
		divide(x, y, width, wall_y - y);                       // 上区域
		divide(x, wall_y + 1, width, y + height - wall_y - 1); // 下区域
	}
}

void MazeLayer::generate_entry_and_exit()
{
	std::vector<Vector2> valid_tiles;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j]->get_type() == TileType::Path || grid[i][j]->get_type() == TileType::DOOR)
			{
				valid_tiles.push_back({ (float)i, (float)j });
			}
		}
	}

	std::shuffle(valid_tiles.begin(), valid_tiles.end(), Random::get_rng());

	start_pos = valid_tiles[0];
	delete grid[start_pos.x][start_pos.y];
	grid[start_pos.x][start_pos.y] = new Start();
	grid[start_pos.x][start_pos.y]->set_pos(start_pos);

	std::cout << "开始位置: (" << start_pos.x << ", " << start_pos.y << ")" << std::endl;

	// 当出口与入口距离大于最佳距离时，选择该出口
	// 若所有出口都不满足条件，则选择距离最远的出口
	int best_dist = (rows + cols) / 2;
	int now_dist = -1;
	Vector2 best_end = valid_tiles[1];

	for (int i = 1; i < valid_tiles.size(); ++i)
	{
		auto pos = valid_tiles[i];
		int dist = std::abs((int)(pos.x - start_pos.x)) + std::abs((int)(pos.y - start_pos.y)); // 曼哈顿距离
		if (dist > best_dist) 
		{
			best_end = pos;
			break;
		}
		
		if (dist > now_dist)
		{
			now_dist = dist;
			best_end = pos;
		}
	}

	end_pos = best_end;
	delete grid[end_pos.x][end_pos.y];
	grid[end_pos.x][end_pos.y] = new End();
	grid[end_pos.x][end_pos.y]->set_pos(end_pos);

	std::cout << "结束位置: (" << end_pos.x << ", " << end_pos.y << ")" << std::endl;

}

void MazeLayer::generate_gold_and_trap()
{
	int block_size = 3;  
	for (int i = 0; i < rows; i += block_size)
	{
		for (int j = 0; j < cols; j += block_size)
		{
			std::vector<Vector2> empty_cells;
			for (int di = 0; di < block_size && i + di < rows; ++di)
			{
				for (int dj = 0; dj < block_size && j + dj < cols; ++dj)
				{
					if (grid[i + di][j + dj]->get_type() == TileType::Empty || 
						grid[i + di][j + dj]->get_type() == TileType::DOOR  ||
						grid[i + di][j + dj]->get_type() == TileType::Path)
						empty_cells.push_back(Vector2((float)(i + di), (float)(j + dj)));
				}
			}

			// 从这一小块中随机挑一个位置
			if (!empty_cells.empty() && Random::chance(0.3)) // 30% 的块可能生成资源
			{
				Vector2 pos = Random::choice(empty_cells);

				if (Random::chance(0.6))  // 60% 概率金币 / 40% 陷阱
				{
					delete grid[pos.x][pos.y];
					grid[pos.x][pos.y] = new Gold(Random::randint(5, 10));
					grid[pos.x][pos.y]->set_pos(pos);
				}
				else
				{
					delete grid[pos.x][pos.y];
					grid[pos.x][pos.y] = new Trap(Random::randint(5, 10));
					grid[pos.x][pos.y]->set_pos(pos);
				}
			}
		}
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



void MazeLayer::generate()
{
	divide(0, 0, cols, rows);
	generate_entry_and_exit();

	on_render();
	Sleep(150);

	generate_gold_and_trap();

	on_render();
	Sleep(150);
}

void MazeLayer::on_render()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grid[i][j]->on_render();
		}
	}
}

void MazeLayer::on_update(int delta)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grid[i][j]->on_update(delta);
		}
	}
}

Maze::Maze(int l, int r, int c) :layers(l), rows(r), cols(c)
{
	maze.reserve(layers);
	for (int i = 0; i < layers; ++i)
		maze.emplace_back(rows, cols);
}

void Maze::generate(int layer)
{
	maze[layer].generate();
}

void Maze::on_render(int layer)
{
	maze[layer].on_render();
}

void Maze::on_update(int delta, int layer)
{
	maze[layer].on_update(delta);
}

