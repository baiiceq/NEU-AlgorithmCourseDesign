#include "optimal_path.h"
#include <queue>

std::vector<Vector2> OptimalPath::calculate(const Vector2& start, const Vector2& end, const std::vector<std::vector<int> > resource, const std::vector<std::vector<TileType> > grid)
{
    int rows = grid.size();
    int cols = grid[0].size();

    std::priority_queue<Node> open_list;

    std::vector<std::vector<bool>> closed(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<int>> g_cost(rows, std::vector<int>(cols, INT_MAX));

	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

    open_list.push({ start, 0, heuristic(start, end), {start} });
    g_cost[start.x][start.y] = 0;
}
