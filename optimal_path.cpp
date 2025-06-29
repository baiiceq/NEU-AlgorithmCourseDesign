#include "optimal_path.h"
#include <queue>
#include <map>

PathResult OptimalPath::find_direct_path(const Vector2& start, const Vector2& end, const std::vector<std::vector<int> > resource, const std::vector<std::vector<TileType> > grid)
{
    if (grid.empty() || grid[0].empty()) 
    {
        return {};
    }
    int rows = grid.size();
    int cols = grid[0].size();

    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open_list;

    std::vector<std::vector<int>> g_costs(rows, std::vector<int>(cols, INT_MAX));

    std::map<Vector2, Vector2> came_from;

    static int dx[] = { -1, 1, 0, 0 };
    static int dy[] = { 0, 0, -1, 1 };

    open_list.push({ start, 0, heuristic(start, end) });
    g_costs[start.x][start.y] = 0;

    while (!open_list.empty())
    {
        Node current = open_list.top();
        open_list.pop();

        if (current.pos == end)
        {
            
            std::vector<Vector2> path;
            Vector2 step = end;
            while (!(step == start))
            {
				
                path.push_back(step);
                if (came_from.find(step) == came_from.end())
                {
                    return {};
                }
                step = came_from.at(step);
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());

            return { path, current.g, true }; 
        }

        int cur_x = static_cast<int>(current.pos.x);
        int cur_y = static_cast<int>(current.pos.y);
        if (current.g > g_costs[cur_x][cur_y]) 
        {
            continue;
        }

        for (int i = 0; i < 4; ++i)
        {
            int neighbor_x = cur_x + dx[i];
            int neighbor_y = cur_y + dy[i];
            // 检查边界
            if (neighbor_x < 0 || neighbor_y < 0 || neighbor_x >= rows || neighbor_y >= cols)
            {
                continue;
            }

            // 检查是否是墙壁
            if (grid[neighbor_x][neighbor_y] == TileType::Wall) 
            {
                continue;
            }

            Vector2 neighbor_pos = { (float)neighbor_x, (float)neighbor_y };

            // 计算移动到邻居的成本
            int move_cost = 1;
            // 若是陷阱，则加上成本
			if (grid[neighbor_x][neighbor_y] == TileType::Trap)
			{
                move_cost -= resource[neighbor_x][neighbor_y];
			}

            int new_g_cost = current.g + move_cost;

            // 如果通过当前节点到达邻居的路径是更优的
            if (new_g_cost < g_costs[neighbor_x][neighbor_y]) 
            {
                // 更新成本和路径来源
                g_costs[neighbor_x][neighbor_y] = new_g_cost;
                came_from[neighbor_pos] = current.pos;

                // 计算新的f_cost并加入open_list
                int f_cost = new_g_cost + heuristic(neighbor_pos, end);
                open_list.push({ neighbor_pos, new_g_cost, f_cost });
            }
        }
    }

    return {};
}

PathResult OptimalPath::find_best_path(const Vector2& start, const Vector2& end, const std::vector<Vector2>& all_coins, const std::vector<std::vector<TileType>>& grid, const std::vector<std::vector<int>>& resource_grid)
{
	std::vector<Point> points;
	int coin_count = all_coins.size();

    points.push_back({ 0, start, 0, -1 });

    // 添加金币 
    for (int i = 0; i < coin_count; i++) 
    {
        points.push_back({ i + 1, all_coins[i], resource_grid[all_coins[i].x][all_coins[i].y], i });
    }

    int end_point_id = coin_count + 1;
    points.push_back({ end_point_id , end, 0, -1 });

    int num_points = points.size();

    std::vector<std::vector<int>> cost_matrix(num_points, std::vector<int>(num_points));
    std::vector<std::vector<std::vector<Vector2>>> path_matrix(num_points, std::vector<std::vector<Vector2>>(num_points));

    for (int i = 0; i < num_points; i++) 
    {
        for (int j = 0; j < num_points; j++)
        {
            if (i == j) continue;
            PathResult result = OptimalPath::find_direct_path(points[i].pos, points[j].pos, resource_grid, grid);
            if (result.found) 
            {
                cost_matrix[i][j] = result.cost;
                path_matrix[i][j] = result.path;
            }
            else
            {
                cost_matrix[i][j] = -1; // -1 表示不可达
            }
        }
    }

    long long num_masks = static_cast<long long>(1) << coin_count;

    // DP表: dp[mask][i] = 访问了mask中的金币，当前停在金币i时的最大收益
    std::vector<std::vector<int>> dp(num_masks + 1, std::vector<int>(num_points + 1, -1));
    // Parent表，用于回溯路径: parent[mask][i] = {上一个状态的mask, 上一个Point的ID}
    std::vector<std::vector<std::pair<int, int>>> parent(num_masks + 1, std::vector<std::pair<int, int>>(num_points + 1, { -1, -1 }));

    // 起点
    int start_point_id = 0;
    // 初始化从起点到任意一个点的情况
    for (int i = 1; i <= coin_count; i++)
    { 
        if (cost_matrix[start_point_id][i] != -1)
        {
            int coin_id = points[i].coin_id;
            int mask = 1 << coin_id;
            dp[mask][i] = points[i].value - cost_matrix[start_point_id][i];
            parent[mask][i] = { 0, start_point_id };
        }
    }

    auto mask_list = OptimalPath::generate_sorted_numbers_by_ones(coin_count);

    for (auto mask : mask_list)
    {
        for (int i = 1; i <= coin_count; i++) 
        {       
            // i是当前所在的金币ID
            if (dp[mask][i] == -1) continue; // 如果当前状态不可达，跳过

            // 检查当前mask是否真的包含了金币i
            if (!(mask & (1 << points[i].coin_id))) continue;

            // 尝试从金币i移动到下一个未访问过的金币j
            for (int j = 1; j <= coin_count; j++) 
            {
                if (i == j) continue; 

                int j_coin_id = points[j].coin_id;
                // 如果金币j已经被访问过，跳过
                if (mask & (1 << j_coin_id)) continue;

                if (cost_matrix[i][j] != -1)
                {
                    int new_mask = mask | (1 << j_coin_id);
                    int new_score = dp[mask][i] + points[j].value - cost_matrix[i][j];

                    if (new_score > dp[new_mask][j]) 
                    {
                        dp[new_mask][j] = new_score;
                        parent[new_mask][j] = { mask, i };
                    }
                }
            }
        }
    }

    int max_score = 0;
    int final_mask = 0;
    int last_point_id = start_point_id;

    // 不吃任何金币，直接从起点到终点
    if (cost_matrix[start_point_id][end_point_id] != -1) 
    {
        max_score = -cost_matrix[start_point_id][end_point_id];
    }

    // 吃了一些金币，最后从某个金币到终点
    for (int mask = 1; mask < num_masks; mask++) 
    {
        for (int i = 1; i <= coin_count; ++i)
        {
            if (dp[mask][i] == -1) continue;

            if (cost_matrix[i][end_point_id] != -1) 
            {
                int final_score = dp[mask][i] - cost_matrix[i][end_point_id];
                if (final_score > max_score)
                {
                    max_score = final_score;
                    final_mask = mask;
                    last_point_id = i;
                }
            }
        }
    }

	std::vector<int> point_path;

    if (final_mask == 0)
    {
        point_path = { start_point_id, end_point_id };
    }
    else
    {
        int current_mask = final_mask;
        int current_point_id = last_point_id;
        while (current_point_id != start_point_id)
        {
            point_path.push_back(current_point_id);
            auto p = parent[current_mask][current_point_id];
            current_mask = p.first;
            current_point_id = p.second;
        }
        point_path.push_back(start_point_id);
        std::reverse(point_path.begin(), point_path.end());
        point_path.push_back(end_point_id);
    }

    std::vector<Vector2> final_path;

    // 使用path_matrix连接POI路径
    for (size_t i = 0; i < point_path.size() - 1; ++i) 
    {
        int from_id = point_path[i];
        int to_id = point_path[i + 1];
        const auto& segment = path_matrix[from_id][to_id];
        if (final_path.empty()) 
        {
            final_path.insert(final_path.end(), segment.begin(), segment.end());
        }
        else
        {
            // 去掉重复的衔接点
            final_path.insert(final_path.end(), segment.begin() + 1, segment.end());
        }
    }

    return { final_path, max_score, true };

    return {};
}
