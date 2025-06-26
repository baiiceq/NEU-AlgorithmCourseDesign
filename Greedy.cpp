#include"Greedy.h"


std::vector<Vector2> moving = { {-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0} };
std::vector<Vector2> moving1 = { {0,-1},{1,0},{0,1},{-1,0} };
std::vector<int> dis = { 2,1,2,1,2,1,2,1 };

bool GreedyPick::in_bounds(Vector2 p, const MazeLayer& mazelayer)
{
	return p.x >= 0 && p.x < mazelayer.getRows() && p.y >= 0 && p.y < mazelayer.getCols();
}

GreedyPick::GreedyPick(Vector2 start,int start_layer)
{
	pos = start;
}

void GreedyPick::set_pos(Vector2 newpos)
{
	pos = newpos;
}

void GreedyPick::PickGoldLayer(MazeLayer& mazelayer)
{
	Path.clear();
	pos = mazelayer.get_start_pos();
	std::set<Vector2> is_Pick;
	std::vector<std::vector<Tile*>> grid = mazelayer.getGrid();
	Vector2 end = mazelayer.get_end_pos();

	std::vector<Vector2> path;
	path = avoidTrap_lastTonext(pos, end, mazelayer);
	if (path.empty())
	{
		path = moveTonext(end, mazelayer);
		path.push_back(end);
		path = avoidTrap(path, mazelayer);
	}
	path.push_back(end);
	Vector2 step = path[0];
	int i = 0;
	while (!(step == end))
	{
		double Max = 0;
		Vector2 Nextpos = step;
		for (int i = 0; i < 8; i++)
		{
			Vector2 WillNextPos = step + moving[i];
			if (!in_bounds(WillNextPos, mazelayer)) continue;
			if (is_Pick.find(WillNextPos) == is_Pick.end() && mazelayer.get_tile(WillNextPos.x, WillNextPos.y)->get_type() == TileType::Gold)
			{
				Tile* tile = grid[WillNextPos.x][WillNextPos.y];
				Gold* gold = dynamic_cast<Gold*>(tile);
				if (double(gold->get_value()) / double(dis[i]) > Max)
				{
					Max = double(gold->get_value()) / double(dis[i]);
					Nextpos = WillNextPos;
				}
			}
		}
		if (Max != 0)
		{
			std::vector<Vector2> jubu_path = avoidTrap_lastTonext(step, Nextpos, mazelayer);
			if (jubu_path.empty())
			{
				jubu_path = moveTonext(Nextpos, mazelayer);
			}
			jubu_path.push_back(Nextpos);
			for (auto step1 : jubu_path)
				Path.push_back(step1);
			pos = Nextpos;

			is_Pick.insert(pos);
			path = avoidTrap_lastTonext(pos, end, mazelayer);
			if (path.empty())
			{
				path = moveTonext(end, mazelayer);
				path.push_back(end);
				path = avoidTrap(path, mazelayer);
			}
			path.push_back(end);
			step = path[0];
			i = 0;
			continue;
		}
		Path.push_back(step);
		pos = step;
		i++;
		step = path[i];
	}
	Path.push_back(end);
}

void GreedyPick::findNearestGold(std::set<Vector2>& is_Pick, MazeLayer& mazelayer)
{
	std::queue<Vector2> q;
	std::map<Vector2, Vector2> parent;
	std::set<Vector2> visited;

	q.push(pos);
	visited.insert(pos);
	while (!q.empty()) 
	{
		Vector2 curr = q.front(); 
		q.pop();

		Tile* tile = mazelayer.get_tile(curr.x, curr.y);
		Gold* gold = dynamic_cast<Gold*>(tile);
		if (gold && is_Pick.find(curr)!=is_Pick.end()) 
		{
			// 找到资源了，回溯路径
			std::vector<Vector2> path;
			Vector2 p = curr;
			for (Vector2 p = curr; !(p == pos); p = parent[p])
				path.push_back(p);

			std::reverse(path.begin(), path.end()); // 从 start 到目标
			for (auto step : path)
				Path.push_back(step);
			pos = curr;
			is_Pick.insert(curr);
			return;
		}

		for (int i = 0; i < 4; i++) {
			Vector2 next = curr + moving1[i]; // moving[] 为上下左右
			if (!in_bounds(next, mazelayer)) continue;
			if (visited.count(next)) continue;

			Tile* next_tile = mazelayer.get_tile(next.x, next.y);
			if (!next_tile->is_walkable()) continue;

			visited.insert(next);
			parent[next] = curr;
			q.push(next);
		}
	}

	moveTonext(mazelayer.get_end_pos(), mazelayer);

}

std::vector<Vector2> GreedyPick::moveTonext(Vector2 end, const MazeLayer& mazelayer)
{
	std::queue<Vector2> q;
	std::map<Vector2, Vector2> parent;
	std::set<Vector2> visited;

	q.push(pos);
	visited.insert(pos);

	while (!q.empty())
	{
		Vector2 curr = q.front();
		q.pop();

		if (curr == end)
		{
			std::vector<Vector2> path;
			for (Vector2 p = curr; !(p == pos); p = parent[p])
				path.push_back(p);
			path.push_back(pos);
			std::reverse(path.begin(), path.end());
			if (!path.empty())path.pop_back();
			return path;
		}

		for (int i = 0; i < 4; i++) {
			Vector2 next = curr + moving1[i];
			if (!in_bounds(next, mazelayer)) continue;
			if (visited.count(next)) continue;

			Tile* next_tile = mazelayer.get_tile(next.x, next.y);
			if (!next_tile->is_walkable()) continue;

			visited.insert(next);
			parent[next] = curr;
			q.push(next);
		}
	}

	// 如果连出口都到不了，可以记录失败或者报错
	std::cerr << "Warning: Cannot reach exit!" << std::endl;
	return {};
}

std::vector<Vector2> GreedyPick::avoidTrap(const std::vector<Vector2>& path, const MazeLayer& mazelayer)
{
	std::vector<Vector2> adjusted_path;
	for (size_t i = 0; i < path.size(); ++i) 
	{
		Vector2 step = path[i];

		if (mazelayer.get_tile(step.x,step.y)->get_type()==TileType::Trap) 
		{
			// 尝试绕过陷阱
			bool avoided = false;
			Vector2 last;
			if (!adjusted_path.empty())
			{
				last = adjusted_path.back();
			}
			else
			{
				std::cerr << "empty vector" << std::endl;
			}
			if (i + 1 >= path.size())
			{
				std::cerr << "yunjie" << std::endl;
			}
			std::vector<Vector2> jubu_path = avoidTrap_lastTonext(last, path[i + 1], mazelayer);
			if (!jubu_path.empty())
			{
				avoided = 1;
				for (auto p : jubu_path)
					adjusted_path.push_back(p);
			}
			if (!avoided) 
			{
				adjusted_path.push_back(step); // 实在绕不过，硬走陷阱
			}
		}
		else 
		{
			adjusted_path.push_back(step); // 正常 tile
		}
	}
	return adjusted_path;
}

std::vector<Vector2> GreedyPick::avoidTrap_lastTonext(Vector2 start, Vector2 end, const MazeLayer& mazelayer)
{
	std::queue<Vector2> q;
	std::map<Vector2, Vector2> parent;
	std::set<Vector2> visited;

	q.push(start);
	visited.insert(start);

	while (!q.empty())
	{
		Vector2 curr = q.front();
		q.pop();

		if (curr == end)
		{
			std::vector<Vector2> path;
			for (Vector2 p = curr; !(p == start); p = parent[p])
				path.push_back(p);
			path.push_back(start);
			std::reverse(path.begin(), path.end());
			path.pop_back();
			return path;
		}

		for (int i = 0; i < 4; i++) 
		{
			Vector2 next = curr + moving1[i];
			if (!in_bounds(next, mazelayer)) continue;
			if (visited.count(next)) continue;
			
			Tile* next_tile = mazelayer.get_tile(next.x, next.y);
			if (!next_tile->is_walkable()) continue;
			if (next_tile->get_type() == TileType::Trap) continue;
			visited.insert(next);
			parent[next] = curr;
			q.push(next);
		}
	}
	return {};
}

void GreedyPick::PickGold(std::vector<MazeLayer>& maze)
{
	for (int i = 0; i < maze.size(); i++)
	{
		set_pos(maze[i].get_start_pos());
		PickGoldLayer(maze[i]);
	}
}