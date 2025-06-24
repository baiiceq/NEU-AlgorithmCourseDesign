#include "maze_game.h"

MazeGame::MazeGame(int layers, int rows, int cols) : maze(layers, rows, cols),layers(layers), rows(rows), cols(cols)
{
	// ��ʼ���Թ�
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

		if (player.get_pos() == maze.get_end_pos(now_layer) && player.get_is_exit())
		{
			maze.generate(++now_layer);
			player.set_position(maze.get_start_pos(now_layer));
		}
	}
}

void MazeGame::on_render()
{
	if (state == start)
	{
		maze.on_render(now_layer);
		player.on_render();

		settextstyle(26, 15, L"΢���ź�");
		std::wstring text = L"��ǰ����: " + std::to_wstring(now_layer + 1) + L" / " + std::to_wstring(layers);
		outtextxy(rows * 40 + 20, 20, text.c_str());

		text = L"���: " + std::to_wstring(player.get_resource());
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
