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
	if (state == start)
	{
		player.on_update(delta, maze.get_layer(now_layer));
		maze.on_update(delta, now_layer);
	}
}

void MazeGame::on_render()
{
	if (state == start)
	{
		maze.on_render(now_layer);
		player.on_render();

		settextstyle(30, 15, L"微软雅黑");
		std::wstring text = L"当前层数: " + std::to_wstring(now_layer + 1) + L" / " + std::to_wstring(layers);
		outtextxy(rows * 40 + 20, 20, text.c_str());

		text = L"金币: " + std::to_wstring(player.get_resource());
		outtextxy(rows * 40 + 20, 60, text.c_str());

	}
}

void MazeGame::on_input(const ExMessage& msg)
{
	if (state == start)
	{
		player.on_input(msg);
	}
}
