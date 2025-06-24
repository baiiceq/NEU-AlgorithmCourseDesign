#include"Greedy.h"


std::vector<Vector2> moving = { {-1,-1},{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0} };
std::vector<Vector2> moving1 = { {0,-1},{1,0},{0,1},{-1,0} };
std::vector<int> dis = { 2,1,2,1,2,1,2,1 };

bool GreedyPick::in_bounds(Vector2 p, MazeLayer mazelayer)
{
	return p.x >= 0 && p.x < mazelayer.getRows() && p.y >= 0 && p.y < mazelayer.getCols();
}
GreedyPick::GreedyPick(Vector2 start,int start_layer)
{
	pos = start;
	layer = start_layer;
}
void GreedyPick::PickGoldLayer(MazeLayer mazelayer)
{
	std::set<Vector2> is_Pick;
	std::vector<std::vector<Tile*>> grid = mazelayer.getGrid();
	Path[layer].push_back(pos);
	while (mazelayer.get_tile(pos.x, pos.y)->get_type() != TileType::End)
	{
		double Max = 0;
		Vector2 Nextpos = pos;
		for (int i = 0; i < 8; i++)
		{
			Vector2 WillNextPos = pos + moving[i];
			if (is_Pick.find(WillNextPos)==is_Pick.end()&&mazelayer.get_tile(WillNextPos.x, WillNextPos.y)->get_type() == TileType::Gold)
			{
				Tile* tile = grid[WillNextPos.x][WillNextPos.y];
				Gold* gold = dynamic_cast<Gold*>(tile);
				if (double(gold->get_value()) / double(dis[i]) > Max)
				{
					Max = 0;
					Nextpos = WillNextPos;
				}
			}
		}
		if (Max != 0)
		{
			pos = Nextpos;
			Path[layer].push_back(pos);
			is_Pick.insert(pos);
			continue;
		}
		findNearestGold(is_Pick, mazelayer);

	}
}
void GreedyPick::findNearestGold(std::set<Vector2> is_Pick, MazeLayer mazelayer)
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
			while (!(p == pos)) {
				path.push_back(p);
				p = parent[p];
			}
			std::reverse(path.begin(), path.end()); // 从 start 到目标
			pos = curr;
			for (auto step : path)
			{
				Path[layer].push_back(step);
			}
			pos = curr;
			return;
		}

		for (int i = 0; i < 4; i++) {
			Vector2 next = curr + moving1[i]; // moving[] 为上下左右
			if (in_bounds(next, mazelayer)) continue;
			if (visited.count(next)) continue;

			Tile* next_tile = mazelayer.get_tile(next.x, next.y);
			if (!next_tile->is_walkable()) continue;

			visited.insert(next);
			parent[next] = curr;
			q.push(next);
		}
	}

	layer++;
}