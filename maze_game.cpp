#include "maze_game.h"
#include <graphics.h>

MazeGame::MazeGame()
{
	state = Init;

	float left_x = 90;
	float right_x = 460;
	float top_y = 120;
	float gap_y = 80; 

	seed_input.set_pos(1.8f * left_x, top_y + 0 * gap_y);
	seed_input.set_size(340, 40);
	seed_input.set_maxlen(32);
	seed_input.set_text_when_blank(L"����������");

	row_selector.set_position({ 2 * left_x, top_y + 1 * gap_y });
	row_selector.set_size({ 120, 40 });
	row_selector.set_options({ L"7", L"8", L"9", L"10", L"11", L"12", L"13", L"14", L"15", L"16", L"17", L"18" });

	col_selector.set_position({ 2 * left_x, top_y + 2 * gap_y });
	col_selector.set_size({ 120, 40 });
	col_selector.set_options({ L"7", L"8", L"9", L"10", L"11", L"12", L"13", L"14", L"15", L"16", L"17", L"18" });

	layer_selector.set_position({ 2 * left_x, top_y + 3 * gap_y });
	layer_selector.set_size({ 120, 40 });
	layer_selector.set_options({ L"1", L"2", L"3", L"4", L"5" });

	ai_selector.set_position({ 2 * left_x, top_y + 4 * gap_y });
	ai_selector.set_size({ 120, 40 });
	ai_selector.set_options({ L"����", L"AI" });

	start_button.set_text(L"��ʼ��Ϸ");
	start_button.set_pos(right_x, top_y + 5 * gap_y);
	start_button.set_size(120, 50);
	start_button.set_on_click([this]()
	{
		cols = std::stoi(row_selector.GetSelectedText());
		rows = std::stoi(col_selector.GetSelectedText());
		layers = std::stoi(layer_selector.GetSelectedText());
		maze.reset(layers, rows, cols);
		auto seed_text = seed_input.get_wtext();
		if (seed_text.empty())
		{
			seed = Random::init();
		}
		else
		{
			seed = std::stoi(seed_text);
			Random::init(seed);
		}
		
		if (ai_selector.GetSelectedText() == L"AI")
		{
			is_ai = true;
			state = Ai;
		}
		else
		{
			is_ai = false;
			state = Gamer;
		}

		closegraph();               
		initgraph(max(rows, cols) * Grid::getTileSize() + 300, max(rows, cols) * Grid::getTileSize());
	});

	exit_button.set_text(L"�˳���Ϸ");
	exit_button.set_pos(right_x + 160, top_y + 5 * gap_y);
	exit_button.set_size(120, 50);
	exit_button.set_on_click([this]()
	{
		exit(0);
	});
}


void MazeGame::on_update(int delta)
{
	switch (state)
	{
	case Init:
		seed_input.on_update(delta);
		break;
	case Gamer:
		if (!is_maze_generate) // ����Թ���û�����ɣ��������Թ�
		{
			maze.generate(now_layer);
			player.set_position(maze.get_start_pos(now_layer));
			is_maze_generate = true;
		}

		player.on_update(delta, maze.get_layer(now_layer));
		maze.on_update(delta, now_layer);

		if (player.get_pos() == maze.get_end_pos(now_layer) && player.get_is_exit())
		{
			maze.generate(++now_layer);
			player.set_position(maze.get_start_pos(now_layer));
		}
		break;
	case Ai:
		break;
	}

	/*if (state != generate)
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
	}*/
}

void MazeGame::on_render()
{
	setbkcolor(RGB(150, 160, 200));
	switch (state)
	{
	case Init:
		setbkmode(TRANSPARENT);
		settextstyle(25, 10, L"΢���ź�");
		outtextxy(40, 130, L"����");
		outtextxy(40, 210, L"����");
		outtextxy(40, 290, L"����");
		outtextxy(40, 370, L"����");
		outtextxy(40, 450, L"��������");
		exit_button.on_render();
		start_button.on_render();
		seed_input.on_render();
		row_selector.on_render();
		col_selector.on_render();
		layer_selector.on_render();
		ai_selector.on_render();
		break;
	case Gamer:
	case Ai:
		maze.on_render(now_layer, is_show_resource);
		player.on_render();

		settextstyle(37, 15, L"΢���ź�");
		std::wstring text = L"��ǰ����: " + std::to_wstring(now_layer + 1) + L" / " + std::to_wstring(layers);
		outtextxy(rows * 40 + 20, 20, text.c_str());

		text = L"���: " + std::to_wstring(player.get_resource());
		outtextxy(rows * 40 + 20, 80, text.c_str());

		if (is_show_resource)
		{
			text = L"��ǰ����ֵ��ʾ����E�ر�";
			outtextxy(rows * 40 + 20, 140, text.c_str());
		}
		else
		{
			text = L"��ǰ�ر���ֵ��ʾ����E��";
			outtextxy(rows * 40 + 20, 140, text.c_str());
		}
		break;
	}
	/*if (state != generate)
	{
		maze.on_render(now_layer, is_show_resource);
		player.on_render();

		settextstyle(37, 15, L"΢���ź�");
		std::wstring text = L"��ǰ����: " + std::to_wstring(now_layer + 1) + L" / " + std::to_wstring(layers);
		outtextxy(rows * 40 + 20, 20, text.c_str());

		text = L"���: " + std::to_wstring(player.get_resource());
		outtextxy(rows * 40 + 20, 80, text.c_str());

		if (is_show_resource)
		{
			text = L"��ǰ����ֵ��ʾ����E�ر�";
			outtextxy(rows * 40 + 20, 140, text.c_str());
		}
		else
		{
			text = L"��ǰ�ر���ֵ��ʾ����E��";
			outtextxy(rows * 40 + 20, 140, text.c_str());
		}

	}*/
}

void MazeGame::on_input(const ExMessage& msg)
{
	switch (state)
	{
	case Init:
		exit_button.on_input(msg);
		start_button.on_input(msg);
		seed_input.on_input(msg);
		row_selector.on_input(msg);
		col_selector.on_input(msg);
		layer_selector.on_input(msg);
		ai_selector.on_input(msg);
		break;
	case Gamer:
		if (msg.message == WM_KEYDOWN && msg.vkcode == 0x45)
		{
			is_show_resource = !is_show_resource;
		}
		player.on_input(msg);
		break;
	case Ai:
		if (msg.message == WM_KEYDOWN && msg.vkcode == 0x45)
		{
			is_show_resource = !is_show_resource;
		}
		break;
	}
	/*if (msg.message == WM_KEYDOWN && msg.vkcode == 0x51)
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
	}*/
}
