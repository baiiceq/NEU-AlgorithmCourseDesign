#include "maze_game.h"
#include <graphics.h>
#include "sha256.h"

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

	ai_mode_selector.set_position({ 2 * left_x, top_y + 5 * gap_y });
	ai_mode_selector.set_size({ 120, 40 });
	ai_mode_selector.set_options({ L"��̬�滮���Ž�", L"3x3̰��" });

	count_selector.set_position({6.5f * left_x, top_y + 1 * gap_y });
	count_selector.set_size({ 120, 40 });
	count_selector.set_options({ L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10"});

	generate_button.set_text(L"����");
	generate_button.set_pos(6.5f * left_x, top_y + 2 * gap_y);
	generate_button.set_size(120, 50);
	generate_button.set_on_click([&]()
		{
			cols = std::stoi(row_selector.GetSelectedText());
			rows = std::stoi(col_selector.GetSelectedText());
			int cnt = std::stoi(count_selector.GetSelectedText());
			MazeLayer::generate_multiple_mazes(cnt, rows, cols);
			MessageBox(GetHWnd(), _T("���ɳɹ�"), _T("�ɹ�"), MB_OK);
		});

	load_button.set_text(L"��ȡ");
	load_button.set_pos(6.5f * left_x, top_y + 3 * gap_y);
	load_button.set_size(120, 50);
	load_button.set_on_click([&]() 
		{
			std::wstring filename = open_json_file_dialog();
		});

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
			ai_mode = ai_mode_selector.GetSelected();
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

	password_input.set_pos(80, 60);
	password_input.set_size(640, 60);
	password_input.set_text_when_blank(L"����������");
	password_input.set_maxlen(3);

	skip_button.set_pos(80, 600);
	skip_button.set_size(120, 60);
	skip_button.set_text(L"����");
	skip_button.set_on_click([&]
		{

		});

	enter_button.set_pos(760, 60);
	enter_button.set_size(120, 60);
	enter_button.set_text(L"ȷ��");
	enter_button.set_on_click([&]
		{
			if (password_input.get_text().size() == 3)
			{
				try_password = std::stoi(password_input.get_text());

				if (password == try_password)
				{
					closegraph();
					initgraph(max(rows, cols) * Grid::getTileSize() + 300, max(rows, cols) * Grid::getTileSize());
					state = is_ai ? Ai : Gamer;
					player.set_is_locker(false);
					player.run_reset();
					try_password = -1;
					password = -1;
					clue.clear();
				}
				else
				{
					player.add_resource(-1);
				}
			}
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

		if (player.get_is_locker())
		{
			state = Locker;
			try_password = -1;
			password = maze.get_password(now_layer);
			clue = maze.get_clue(now_layer);
			closegraph();
			initgraph(1080, 720);
		}
		break;
	case Ai:
		if (!is_maze_generate) // ����Թ���û�����ɣ��������Թ�
		{
			maze.generate(now_layer);
			player.set_position(maze.get_start_pos(now_layer));
			is_maze_generate = true;
		}
		player.on_update(delta, maze.get_layer(now_layer));
		maze.on_update(delta, now_layer);

		if (ai_path.empty())
		{
			switch (ai_mode)
			{
			case 0:
			{
				auto paths = OptimalPath::find_best_path(maze.get_start_pos(now_layer), maze.get_end_pos(now_layer), maze.get_coins_pos(now_layer), maze.get_simple_grid(now_layer), maze.get_resource_grid(now_layer));
				ai_path = paths.path;
				break;
			}
			case 1:
			{
				gp.PickGoldLayer(maze.get_layer(now_layer));
				ai_path = gp.getPath(0);
			}
			default:
				break;
			}
		}
		else
		{
			if (ai_path_idx < ai_path.size())
			{
				if (player.move_to(ai_path[ai_path_idx], maze.get_layer(now_layer)))
				{
					ai_path_idx++;
				}
			}
		}
		if (player.get_is_locker())
		{
			state = Locker;
			try_password = -1;
			password = maze.get_password(now_layer);
			clue = maze.get_clue(now_layer);
			CrackingSession cs;
			cs.set_clue(clue);
			cs.run(to_three_digit_string(password));
			ai_try_passwords.clear();
			ai_try_passwords = cs.get_attempts();
			ai_try_password_idx = 0;
			closegraph();
			initgraph(1080, 720);
		}

		if (player.get_pos() == maze.get_end_pos(now_layer) && ai_path_idx == ai_path.size())
		{
			maze.generate(++now_layer);
			player.set_position(maze.get_start_pos(now_layer));
			ai_path_idx = 0;
			ai_path.clear();
		}
		break;
	case Locker:
		if(!is_ai)
			password_input.on_update(delta);
		else
		{
			if (ai_try_password_idx < ai_try_passwords.size())
			{
				try_password = std::atoi(ai_try_passwords[ai_try_password_idx].c_str());
				ai_try_password_idx++;
				Sleep(50);
				if (ai_try_password_idx == ai_try_passwords.size())
				{
					closegraph();
					initgraph(max(rows, cols) * Grid::getTileSize() + 300, max(rows, cols) * Grid::getTileSize());
					state = is_ai ? Ai : Gamer;
					player.set_is_locker(false);
					player.run_reset();
					try_password = -1;
					password = -1;
					clue.clear();
				}
				else
				{
					player.add_resource(-1);
				}
			}
		}
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
		outtextxy(400, 210, L"���n���Թ�");
		if (ai_selector.GetSelectedText() == L"AI")
		{
			outtextxy(40, 530, L"AI����");
			ai_mode_selector.on_render();
		}
		exit_button.on_render();
		start_button.on_render();
		seed_input.on_render();
		row_selector.on_render();
		col_selector.on_render();
		layer_selector.on_render();
		ai_selector.on_render();
		count_selector.on_render();
		generate_button.on_render();
		load_button.on_render();
		break;
	case Gamer:
	case Ai:
	{
		maze.on_render(now_layer, is_show_resource);
		for (auto pos : ai_path)
		{
			Rect rect = { pos.x * Grid::getTileSize(), pos.y * Grid::getTileSize(), 40, 40 };
			putimage_alpha(ResourcesManager::instance()->find_image("right_path"), &rect);
		}
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
	case Locker:
	{
		settextstyle(28, 11, L"΢���ź�");

		std::wstring text = L"��ǰ���: " + std::to_wstring(player.get_resource());
		outtextxy(20, 160, text.c_str());
		text = L"Ŀ������Ĺ�ϣֵ:" + string_to_wstring(sha256::hash_string(to_three_digit_string(password)));
		outtextxy(20, 200, text.c_str());
		if (try_password != -1)
		{
			text = L"��������:" + string_to_wstring(to_three_digit_string(try_password));
			outtextxy(20, 240, text.c_str());
			text = L"��������Ĺ�ϣֵ:" + string_to_wstring(sha256::hash_string(to_three_digit_string(try_password)));
			outtextxy(20, 280, text.c_str());
		}

		for (int i = 0; i < clue.size(); i++)
		{
			if (clue[i].size() == 2)
			{
				if (clue[i][0] == -1)
				{
					text = L"����" + std::to_wstring(i) + L": ��λ�������������Ҳ��ظ�";
				}
				else
				{
					text = L"����" + std::to_wstring(i) + L": ��" + std::to_wstring(clue[i][0]) + L"λ��" + (clue[i][1] ? L"������" : L"��ż��");
				}
			}
			else
			{
				for (int j = 0; j < 3; j++)
				{
					if (clue[i][j] != -1)
					{
						text = L"����" + std::to_wstring(i) + L": ��" + std::to_wstring(j + 1) + L"λ��" + std::to_wstring(clue[i][j]);
						break;
					}
				}
			}
			outtextxy(20, 320 + 40 * i, text.c_str());

		}
		if(!is_ai)
		{
			password_input.on_render();
			enter_button.on_render();
			skip_button.on_render();
		}
	}
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
		count_selector.on_input(msg);
		generate_button.on_input(msg);
		load_button.on_input(msg);
		if (ai_selector.GetSelectedText() == L"AI")
		{
			ai_mode_selector.on_input(msg);
		}
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
	case Locker:
		if(!is_ai)
		{
			enter_button.on_input(msg);
			password_input.on_input(msg);
			skip_button.on_input(msg);
		}
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
