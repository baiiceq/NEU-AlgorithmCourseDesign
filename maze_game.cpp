#include "maze_game.h"

MazeGame::MazeGame(int layers, int rows, int cols) : maze(layers, rows, cols),layers(layers), rows(rows), cols(cols)
{
	// 初始化迷宫
	maze.generate(0);
	player.set_position(maze.get_start_pos(0));
	state = start;
}


void MazeGame::on_update(int delta)
{
	if (state != generate)
	{
		player.on_update(delta, maze.get_layer(now_layer));
		maze.on_update(delta, now_layer);

		if (player.get_pos() == maze.get_end_pos(now_layer) && player.get_is_exit())
		{
			maze.generate(++now_layer);
			player.set_position(maze.get_start_pos(now_layer));
		}
	}

	if (state == ai)
	{
		if (path_with_no_resource_idx < path_with_no_resource.size())
		{
			if (player.move_to(path_with_no_resource[path_with_no_resource_idx], maze.get_layer(now_layer)))
			{
				std::cout << path_with_no_resource[path_with_no_resource_idx].x << " " << path_with_no_resource[path_with_no_resource_idx].y << std::endl;
				path_with_no_resource_idx++;
			}
		}
	}
}

void MazeGame::on_render()
{
	if (state != generate)
	{
		maze.on_render(now_layer, is_show_resource);
		player.on_render();

		settextstyle(37, 15, L"微软雅黑");
		std::wstring text = L"当前层数: " + std::to_wstring(now_layer + 1) + L" / " + std::to_wstring(layers);
		outtextxy(rows * 40 + 20, 20, text.c_str());

		text = L"金币: " + std::to_wstring(player.get_resource());
		outtextxy(rows * 40 + 20, 80, text.c_str());

		if (is_show_resource)
		{
			text = L"当前打开数值显示，按E关闭";
			outtextxy(rows * 40 + 20, 140, text.c_str());
		}
		else
		{
			text = L"当前关闭数值显示，按E打开";
			outtextxy(rows * 40 + 20, 140, text.c_str());
		}

	}
}

void MazeGame::on_input(const ExMessage& msg)
{
	if (msg.message == WM_KEYDOWN && msg.vkcode == 0x51)
	{
		state = ai;
		auto ans = OptimalPath::find_best_path(player.get_pos(), maze.get_end_pos(now_layer), maze.get_coins_pos(now_layer), maze.get_simple_grid(now_layer), maze.get_resource_grid(now_layer));
		path_with_no_resource = ans.path;
	}
	else if (msg.message == WM_KEYDOWN && msg.vkcode == 0x45)
	{
		is_show_resource = !is_show_resource;
	}
		
	if (state == start)
	{
		player.on_input(msg);
	}
}
