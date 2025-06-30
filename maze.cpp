#include "maze.h"
#include <iostream>
#include "optimal_path.h"
#include <fstream>
#include "json/json.h"
#include "session.h"

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

MazeLayer::MazeLayer(const MazeLayer& ml)
{
	this->rows = ml.rows;
	this->cols = ml.cols;

	this->grid.resize(rows, std::vector<Tile*>(cols, nullptr));
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			if (ml.grid[i][j])
				this->grid[i][j] = ml.grid[i][j]->clone();  
			else
				this->grid[i][j] = nullptr;
		}
	}

	this->start_pos = ml.start_pos;
	this->end_pos = ml.end_pos;
	this->gold_pos = ml.gold_pos;
	this->trap_pos = ml.trap_pos;
	this->locker_pos = ml.locker_pos;
	this->boss_pos = ml.boss_pos;

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

//void MazeLayer::divide(int x, int y, int width, int height, bool is_show_process)
//{
//	if (is_show_process)
//	{
//		BeginBatchDraw();
//		cleardevice();
//		on_render(false);
//		FlushBatchDraw();
//		Sleep(30);
//	}
//	if (width <= 2 || height <= 2) return;
//
//
//	// 优先从较大的一边分割
//	bool prefer_vertical = (width > height);
//	bool can_vertical = (width > 1);
//	bool can_horizontal = (height > 1);
//	bool do_vertical;
//
//	if (width == height)
//	{
//		do_vertical = Random::randint(0, 1);
//	}
//	else
//	{
//		do_vertical = (prefer_vertical && can_vertical) || (!can_horizontal && can_vertical);
//	}
//
//	const int MAX_RETRY = 10;
//
//	if (!can_vertical && !can_horizontal) return;
//
//	if (do_vertical)
//	{
//		if (width < 3) return;
//
//		int wall_x;
//		bool valid = false;
//
//		// 尝试多次，找到不含门的横墙位置
//		for (int attempt = 0; attempt < MAX_RETRY; ++attempt)
//		{
//			wall_x = x + Random::randint(1, width - 2);
//
//			bool has_door = false;
//			for (int i = max(0, y - 1); i < min(y + height + 1, rows); i++)
//			{
//				if (grid[i][wall_x]->get_type() == TileType::DOOR)
//				{
//					has_door = true;
//					break;
//				}
//			}
//
//			if (!has_door)
//			{
//				valid = true;
//				break;
//			}
//		}
//
//		if (!valid) return;
//		int door_y = y + Random::randint(0, height - 1);   // 门可以在整条墙上
//
//		for (int i = y; i < y + height; i++)
//		{
//			if (i == door_y)
//			{
//				grid[i][wall_x]->set_type(TileType::DOOR);
//				continue;
//			}
//			grid[i][wall_x]->set_type(TileType::Wall);
//		}
//
//		// 递归左右区域
//		divide(x, y, wall_x - x, height, is_show_process);                      // 左区域
//		divide(wall_x + 1, y, x + width - wall_x - 1, height, is_show_process); // 右区域
//
//
//	}
//	else
//	{
//		if (height < 3) return;
//
//		int wall_y;
//		bool valid = false;
//
//		// 尝试多次，找到不含门的横墙位置
//		for (int attempt = 0; attempt < MAX_RETRY; ++attempt)
//		{
//			wall_y = y + Random::randint(1, height - 2);
//
//			bool has_door = false;
//			for (int i = max(0, x - 1); i < min(x + width + 1, cols); i++)
//			{
//				if (grid[wall_y][i]->get_type() == TileType::DOOR)
//				{
//					has_door = true;
//					break;
//				}
//			}
//
//			if (!has_door)
//			{
//				valid = true;
//				break;
//			}
//		}
//
//		if (!valid) return;
//
//		int door_x = x + Random::randint(0, width - 1);     // 门可以在整条墙上
//
//		for (int i = x; i < x + width; i++)
//		{
//			if (i == door_x)
//			{
//				grid[wall_y][i]->set_type(TileType::DOOR);
//				continue;
//			}
//			grid[wall_y][i]->set_type(TileType::Wall);
//		}
//
//		// 递归上下区域
//		divide(x, y, width, wall_y - y, is_show_process);                       // 上区域
//		divide(x, wall_y + 1, width, y + height - wall_y - 1, is_show_process); // 下区域
//	}
//}

void MazeLayer::divide(int x, int y, int width, int height, bool is_show_process)
{
	if(is_show_process)
	{
		BeginBatchDraw();
		cleardevice();
		on_render(false);
		FlushBatchDraw();
		Sleep(30);
	}
	if (width <= 2 || height <= 2) return;


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

	if (!can_vertical && !can_horizontal) return;

	if (do_vertical) 
	{
		if (width < 3) return;

		int wall_x = x + 2 * Random::randint(0, (width - 2) / 2) + 1;
		int door_y = y + 2 * Random::randint(0, (height - 1) / 2);

		for (int i = y; i < y + height; i++) 
		{
			if (i == door_y)
				continue;
			grid[i][wall_x]->set_type(TileType::Wall);
		}
		 
		// 递归左右区域
		divide(x, y, wall_x - x, height, is_show_process);                      // 左区域
		divide(wall_x + 1, y, x + width - wall_x - 1, height, is_show_process); // 右区域
	}
	else 
	{
		if (height < 3) return;

		int wall_y = y + 2 * Random::randint(0, (height - 2) / 2) + 1;
		int door_x = x + 2 * Random::randint(0, (width - 1) / 2);   

		for (int i = x; i < x + width; i++)
		{
			if (i == door_x)
				continue;
			grid[wall_y][i]->set_type(TileType::Wall);
		}

		// 递归上下区域
		divide(x, y, width, wall_y - y, is_show_process);                       // 上区域
		divide(x, wall_y + 1, width, y + height - wall_y - 1, is_show_process); // 下区域
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
	int best_dist = (rows + cols) / 1.2f;
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
						grid[i + di][j + dj]->get_type() == TileType::Path)
						empty_cells.push_back(Vector2((float)(i + di), (float)(j + dj)));
				}
			}

			// 从这一小块中随机挑一个位置
			if (!empty_cells.empty() && Random::chance(0.8)) // 35% 的块可能生成资源
			{
				Vector2 pos = Random::choice(empty_cells);

				if (Random::chance(0.6))  // 60% 概率金币 / 40% 陷阱
				{
					delete grid[pos.x][pos.y];
					grid[pos.x][pos.y] = new Gold(Random::randint(150, 200));
					grid[pos.x][pos.y]->set_pos(pos);
				}
				else
				{
					delete grid[pos.x][pos.y];
					grid[pos.x][pos.y] = new Trap(Random::randint(100, 140));
					grid[pos.x][pos.y]->set_pos(pos);
				}
			}
		}
	}
}

void MazeLayer::generate_lock_and_boss()
{
	std::vector<Vector2> doors;
	
	auto path = OptimalPath::find_direct_path(start_pos, end_pos, this->get_resource_grid(), this->get_simple_grid());
	for (auto pos : path.path)
	{
		if (grid[pos.x][pos.y]->get_type() == TileType::Path)
		{
			doors.push_back(pos);
		}
	}

	if (doors.empty()) return;

	std::shuffle(doors.begin(), doors.end(), Random::get_rng());
	locker_pos = doors[0]; 

	delete grid[locker_pos.x][locker_pos.y];
	grid[locker_pos.x][locker_pos.y] = new Locker();
	grid[locker_pos.x][locker_pos.y]->set_pos(locker_pos);

	if (doors.size() < 2) return;

	boss_pos = doors[1];
	delete grid[boss_pos.x][boss_pos.y];
	grid[boss_pos.x][boss_pos.y] = new Boss();
	grid[boss_pos.x][boss_pos.y]->set_pos(boss_pos);
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

void MazeLayer::load_maze_from_json(std::wstring filename)
{
	std::ifstream ifs(filename);
	if (!ifs.is_open())
	{
		std::cerr << "无法打开文件: " << filename.c_str() << std::endl;
		return;
	}

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			delete grid[i][j];
		}
	}

	Json::Value root;
	Json::CharReaderBuilder builder;
	std::string errs;
	Json::parseFromStream(builder, ifs, &root, &errs);
	const auto& json_maze = root["maze"];

	std::vector<std::vector<TileType> > simple_grid;
	for (const auto& row : json_maze)
	{
		std::vector<TileType> grid_row;
		for (const auto& cell : row)
		{
			std::string val = cell.asString();
			if (val == "#")       grid_row.push_back(TileType::Wall);
			else if (val == " ")  grid_row.push_back(TileType::Path);
			else if (val == "S")  grid_row.push_back(TileType::Start);
			else if (val == "E")  grid_row.push_back(TileType::End);
			else if (val == "G")  grid_row.push_back(TileType::Gold);
			else if (val == "T")  grid_row.push_back(TileType::Trap);
			else if (val == "L")  grid_row.push_back(TileType::Locker);
			else if (val == "B")  grid_row.push_back(TileType::Boss);
			else                  grid_row.push_back(TileType::Empty);
		}
		simple_grid.push_back(grid_row);
	}
	cols = simple_grid[0].size();
	rows = simple_grid.size();

	grid.clear();
	grid.resize(rows, std::vector<Tile*>(cols, new Tile()));

	gold_pos.clear();
	trap_pos.clear();
	for (int j = 0; j < rows; j++)
	{
		for (int i = 0; i < cols; i++)
		{
			switch (simple_grid[j][i])
			{
			case TileType::Wall:
				grid[i][j] = new Tile(TileType::Wall);
				break;
			case TileType::Path:
			case TileType::Empty:
				grid[i][j] = new Tile();
				break;
			case TileType::Start:
				grid[i][j] = new Start();
				start_pos = { (float)i, (float)j };
				break;
			case TileType::End:
				grid[i][j] = new End();
				end_pos = { (float)i, (float)j };
				break;
			case TileType::Gold:
				grid[i][j] = new Gold(100);
				gold_pos.emplace_back((float)i, (float)j);
				break;
			case TileType::Trap:
				grid[i][j] = new Trap(30);
				trap_pos.emplace_back((float)i, (float)j);
				break;
			case TileType::Locker:
			{
				Locker* l = new Locker();
				std::vector<std::vector<int>> clue;
				if (root.isMember("C") && root["C"].isArray())
				{
					for (const auto& row : root["C"])
					{
						if (!row.isArray()) continue;
						std::vector<int> temp;
						for (const auto& val : row) {
							temp.push_back(val.asInt());
						}
						clue.push_back(temp);
					}
					l->set_clue(clue);
				}
				if (root.isMember("L") && root["L"].isString())
				{
					auto p = CrackingSession::hashToPassword(root["L"].asString());
					l->set_password(std::stoi(p));
				}
				grid[i][j] = l;
				locker_pos = { (float)i, float(j) };
				break;
			}
			case TileType::Boss:
			{
				grid[i][j] = new Boss();
				break;
			}
			default:
				break;
			}
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



void MazeLayer::generate(bool is_show_process)
{
	divide(0, 0, cols, rows, is_show_process);
	if (!(cols & 1))
	{
		for (int i = 0; i < rows; i++)
		{
			grid[i][cols - 1]->set_type(TileType::Wall);
		}
	}

	if (!(rows & 1))
	{
		for (int i = 0; i < cols; i++)
		{
			grid[rows - 1][i]->set_type(TileType::Wall);
		}
	}
	generate_entry_and_exit();

	if(is_show_process)
	{
		BeginBatchDraw();
		cleardevice();
		on_render(false);
		FlushBatchDraw();
		Sleep(150);
	}

	generate_gold_and_trap();
	generate_lock_and_boss();

	if (is_show_process)
	{
		BeginBatchDraw();
		cleardevice();
		on_render(false);
		FlushBatchDraw();
		Sleep(150);
	}
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
	// 返回所有金币的位置
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

void MazeLayer::save_maze_to_json(const std::vector<std::vector<TileType>>& maze, const std::string& filename, unsigned int seed, MazeLayer& ml)
{
	Json::Value root;
	Json::Value maze_array(Json::arrayValue);

	for (const auto& row : maze)
	{
		Json::Value json_row(Json::arrayValue);
		for (TileType c : row)
		{
			std::string s;
			switch (c)
			{
			case TileType::Wall:
				s = "#";
				break;
			case TileType::Path:
			case TileType::Empty:
			case TileType::DOOR:
				s = " ";
				break;
			case TileType::Start:
				s = "S";
				break;
			case TileType::End:
				s = "E";
				break;
			case TileType::Gold:
				s = "G";
				break;
			case TileType::Trap:
				s = "T";
				break;
			case TileType::Boss:
				s = "B";
				break;
			case TileType::Locker:
				s = "L";
				break;
			}
			json_row.append(s);
		}
		maze_array.append(json_row);
	}

	root["maze"] = maze_array;
	root["seed"] = seed;

	auto path = OptimalPath::find_best_path(ml.get_start_pos(), ml.get_end_pos(), ml.get_coins_pos(), ml.get_simple_grid(), ml.get_resource_grid());
	Json::Value json_path(Json::arrayValue); 

	for (const auto& vec : path.path) 
	{
		Json::Value point(Json::arrayValue); 
		point.append((int)vec.y);
		point.append((int)vec.x);
		json_path.append(point);
	}

	root["optimal_path"] = json_path;

	std::ofstream ofs(filename);
	if (!ofs.is_open())
	{
		std::cerr << "无法打开文件: " << filename << std::endl;
		return;
	}

	Json::StyledWriter writer;
	ofs << writer.write(root);

	ofs.close();
}

void MazeLayer::generate_multiple_mazes(int count, int rows, int cols)
{
	for (int i = 0; i < count; i++)
	{
		unsigned int seed = Random::init();
		MazeLayer maze(rows, cols);
		maze.generate(false); 

		std::string filename = "maze_result/maze_" + std::to_string(i + 1) + ".json";
		save_maze_to_json(maze.get_simple_grid(), filename, seed, maze);
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

void Maze::load_maze_from_json(std::wstring filename)
{
	layers = 1;
	maze.clear();

	MazeLayer ml(rows,cols);
	ml.load_maze_from_json(filename);

	rows = ml.getRows();
	cols = ml.getCols();
	maze.push_back(ml);

}

