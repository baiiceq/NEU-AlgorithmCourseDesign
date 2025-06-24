#include "optimal_path.h"
#include <queue>

std::vector<Vector2> OptimalPath::calculate(const Vector2& start, const Vector2& end, const std::vector<std::vector<int> > resource, const std::vector<std::vector<TileType> > grid)
{
    int rows = grid.size();
    int cols = grid[0].size();

    std::priority_queue<Node> open_list;

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<int>> g_cost(rows, std::vector<int>(cols, INT_MAX));

	static int dx[4] = { -1, 1, 0, 0 };
	static int dy[4] = { 0, 0, -1, 1 };

    open_list.push({ start, 0, heuristic(start, end), {start} });
    g_cost[start.x][start.y] = 0;

    while (!open_list.empty()) 
    {
        Node current = open_list.top();
        open_list.pop();
        Vector2 pos = current.pos;

        if (pos == end)
            return current.path;

        if (visited[pos.x][pos.y])
            continue;
        visited[pos.x][pos.y] = true;

        for (int i = 0; i < 4; i++) 
        {
            int nx = pos.x + dx[i], ny = pos.y + dy[i];
            if (nx < 0 || ny < 0 || nx >= rows || ny >= cols)
                continue;

            if (grid[nx][ny] == TileType::Wall)
                continue;

            int step_cost = resource[nx][ny]; 
            int new_g = current.g + step_cost;

            if (new_g < g_cost[nx][ny]) 
            {
                g_cost[nx][ny] = new_g;
                int f = new_g + heuristic({ (float)nx, (float)ny }, end);
                std::vector<Vector2> new_path = current.path;
                new_path.push_back({ (float)nx, (float)ny });
                open_list.push({ Vector2{ (float)nx, (float)ny }, new_g, f, new_path });
            }
        }
    }

    return {};
}
