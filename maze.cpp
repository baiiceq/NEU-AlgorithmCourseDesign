#include "maze.h"
#include <iostream>
#include "optimal_path.h"

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
	BeginBatchDraw();
	cleardevice();
	on_render(false);
	FlushBatchDraw();
	Sleep(30);
	if (width <= 1 || height <= 1) return;

	// ���ȴӽϴ��һ�߷ָ�
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

		// ���Զ�Σ��ҵ������ŵĺ�ǽλ��
		for (int attempt = 0; attempt < MAX_RETRY; ++attempt)
		{
			wall_x = x + Random::randint(1, width - 2);

			bool has_door = false;
			for (int i = max(0, y - 1); i < min(y + height + 1, rows) ; i++)
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
		int door_y = y + Random::randint(0, height - 1);   // �ſ���������ǽ��

		for (int i = y; i < y + height; i++) 
		{
			if (i == door_y)
			{
				grid[i][wall_x]->set_type(TileType::DOOR);
				continue;
			}
			grid[i][wall_x]->set_type(TileType::Wall);
		}
		 
		// �ݹ���������
		divide(x, y, wall_x - x, height);                      // ������
		divide(wall_x + 1, y, x + width - wall_x - 1, height); // ������


	}
	else 
	{
		if (height < 3) return;

		int wall_y;
		bool valid = false;

		// ���Զ�Σ��ҵ������ŵĺ�ǽλ��
		for (int attempt = 0; attempt < MAX_RETRY; ++attempt)
		{
			wall_y = y + Random::randint(1, height - 2);

			bool has_door = false;
			for (int i = max(0, x - 1); i < min(x + width + 1, cols); i++) 
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

		int door_x = x + Random::randint(0, width - 1);     // �ſ���������ǽ��

		for (int i = x; i < x + width; i++)
		{
			if (i == door_x)
			{
				grid[wall_y][i]->set_type(TileType::DOOR);
				continue;
			}
			grid[wall_y][i]->set_type(TileType::Wall);
		}

		// �ݹ���������
		divide(x, y, width, wall_y - y);                       // ������
		divide(x, wall_y + 1, width, y + height - wall_y - 1); // ������
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

	std::cout << "��ʼλ��: (" << start_pos.x << ", " << start_pos.y << ")" << std::endl;

	// ����������ھ��������Ѿ���ʱ��ѡ��ó���
	// �����г��ڶ���������������ѡ�������Զ�ĳ���
	int best_dist = (rows + cols) / 2;
	int now_dist = -1;
	Vector2 best_end = valid_tiles[1];

	for (int i = 1; i < valid_tiles.size(); ++i)
	{
		auto pos = valid_tiles[i];
		int dist = std::abs((int)(pos.x - start_pos.x)) + std::abs((int)(pos.y - start_pos.y)); // �����پ���
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

	std::cout << "����λ��: (" << end_pos.x << ", " << end_pos.y << ")" << std::endl;

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

			// ����һС���������һ��λ��
			if (!empty_cells.empty() && Random::chance(0.25)) // 25% �Ŀ����������Դ
			{
				Vector2 pos = Random::choice(empty_cells);

				if (Random::chance(0.6))  // 60% ���ʽ�� / 40% ����
				{
					delete grid[pos.x][pos.y];
					grid[pos.x][pos.y] = new Gold(Random::randint(50, 100));
					grid[pos.x][pos.y]->set_pos(pos);
				}
				else
				{
					delete grid[pos.x][pos.y];
					grid[pos.x][pos.y] = new Trap(Random::randint(40, 80));
					grid[pos.x][pos.y]->set_pos(pos);
				}
			}
		}
	}
}

void MazeLayer::generate_lock()
{
	std::vector<Vector2> doors;
	
	auto path = OptimalPath::find_direct_path(start_pos, end_pos, this->get_resource_grid(), this->get_simple_grid());
	for (auto pos : path.path)
	{
		if (grid[pos.x][pos.y]->get_type() == TileType::DOOR)
		{
			doors.push_back(pos);
		}
	}

	if (doors.empty()) return;

	std::shuffle(doors.begin(), doors.end(), Random::get_rng());
	auto locker_pos = doors[0]; 

	delete grid[locker_pos.x][locker_pos.y];
	grid[locker_pos.x][locker_pos.y] = new Locker();
	grid[locker_pos.x][locker_pos.y]->set_pos(locker_pos);
}

void MazeLayer::reset(int row, int col)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			delete grid[i][j];
		}
	}

	rows = row;
	cols = col;
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

	BeginBatchDraw();
	cleardevice();
	on_render(false);
	FlushBatchDraw();
	Sleep(150);

	generate_gold_and_trap();
	generate_lock();
	BeginBatchDraw();
	cleardevice();
	on_render(false);
	FlushBatchDraw();
	Sleep(150);
}

void MazeLayer::on_render(bool is_show_resource)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			grid[i][j]->on_render(is_show_resource);
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

std::vector<std::vector<TileType> > MazeLayer::get_simple_grid() const
{
	std::vector<std::vector<TileType> > simple_grid(rows, std::vector<TileType>(cols, TileType::Empty));
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			simple_grid[i][j] = grid[i][j]->get_type();
		}
	}

	return simple_grid;
}

std::vector<std::vector<int> > MazeLayer::get_resource_grid() const
{
	std::vector<std::vector<int> > resource_grid(rows, std::vector<int>(cols, -1)); 

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j]->get_type() == TileType::Gold)
				resource_grid[i][j] = static_cast<Gold*>(grid[i][j])->get_value();
			else if (grid[i][j]->get_type() == TileType::Trap)
				resource_grid[i][j] = -static_cast<Trap*>(grid[i][j])->get_damage();
		}
	}

	return resource_grid;
}

std::vector<Vector2> MazeLayer::get_coins_pos() const
{
	// �������н�ҵ�λ��
	std::vector<Vector2> coins_pos;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (grid[i][j]->get_type() == TileType::Gold)
			{
				coins_pos.emplace_back(i, j);
			}
		}
	}

	return coins_pos;
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

void Maze::on_render(int layer, bool is_show_resource)
{
	maze[layer].on_render(is_show_resource);
}

void Maze::on_update(int delta, int layer)
{
	maze[layer].on_update(delta);
}

void Maze::reset(int layer, int row, int col)
{
	layers = layer;
	rows = row;
	cols = col;

	maze.clear();
	maze.reserve(layers);
	for (int i = 0; i < layers; ++i)
	{
		maze.emplace_back(rows, cols);
	}

}

