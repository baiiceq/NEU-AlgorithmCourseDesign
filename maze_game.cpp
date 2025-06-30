#include "maze_game.h"
#include <graphics.h>
#include "sha256.h"
#include "json/json.h"
#include <fstream>

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
	seed_input.set_text_when_blank(L"请输入种子");

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
	ai_selector.set_options({ L"真人", L"AI" });

	ai_mode_selector.set_position({ 2 * left_x, top_y + 5 * gap_y });
	ai_mode_selector.set_size({ 120, 40 });
	ai_mode_selector.set_options({ L"动态规划最优解", L"3x3贪心" });

	count_selector.set_position({6.5f * left_x, top_y + 1 * gap_y });
	count_selector.set_size({ 120, 40 });
	count_selector.set_options({ L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10"});

	generate_button.set_text(L"生成");
	generate_button.set_pos(6.5f * left_x, top_y + 2 * gap_y);
	generate_button.set_size(120, 50);
	generate_button.set_on_click([&]()
		{
			cols = std::stoi(row_selector.GetSelectedText());
			rows = std::stoi(col_selector.GetSelectedText());
			int cnt = std::stoi(count_selector.GetSelectedText());
			MazeLayer::generate_multiple_mazes(cnt, rows, cols);
			MessageBox(GetHWnd(), _T("生成成功"), _T("成功"), MB_OK);
		});

	load_button.set_text(L"读取");
	load_button.set_pos(6.5f * left_x, top_y + 3 * gap_y);
	load_button.set_size(120, 50);
	load_button.set_on_click([&]() 
		{
			std::wstring filename = open_json_file_dialog();
			maze.load_maze_from_json(filename);
			cols = maze.get_col();
			rows = maze.get_row();
			layers = 1;

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

			is_maze_generate = true;
			player.set_position(maze.get_start_pos(now_layer));
			closegraph();
			initgraph(max(rows, cols) * Grid::getTileSize() + 300, max(rows, cols) * Grid::getTileSize());
		});

	start_button.set_text(L"开始游戏");
	start_button.set_pos(right_x, top_y + 5 * gap_y);
	start_button.set_size(120, 50);
	start_button.set_on_click([this]()
	{
		is_maze_generate = false;
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

	exit_button.set_text(L"退出游戏");
	exit_button.set_pos(right_x + 160, top_y + 5 * gap_y);
	exit_button.set_size(120, 50);
	exit_button.set_on_click([this]()
	{
		exit(0);
	});

	password_input.set_pos(80, 60);
	password_input.set_size(640, 60);
	password_input.set_text_when_blank(L"请输入密码");
	password_input.set_maxlen(3);

	skip_button.set_pos(80, 600);
	skip_button.set_size(120, 60);
	skip_button.set_text(L"跳过");
	skip_button.set_on_click([&]
		{
			closegraph();
			initgraph(max(rows, cols)* Grid::getTileSize() + 300, max(rows, cols)* Grid::getTileSize());
			state = is_ai ? Ai : Gamer;
			player.set_is_locker(false);
			player.run_reset();
			try_password = -1;
			password = -1;
			clue.clear();
		});

	enter_button.set_pos(760, 60);
	enter_button.set_size(120, 60);
	enter_button.set_text(L"确认");
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
		if (!is_maze_generate) // 如果迷宫还没有生成，则生成迷宫
		{
			maze.generate(now_layer);
			player.set_position(maze.get_start_pos(now_layer));
			is_maze_generate = true;
		}

		player.on_update(delta, maze.get_layer(now_layer));
		maze.on_update(delta, now_layer);

		if (player.get_pos() == maze.get_end_pos(now_layer) && player.get_is_exit())
		{
			if (now_layer == layers - 1)
			{
				MessageBox(GetHWnd(), _T("迷宫结束"), _T("成功"), MB_OK);
				
				ai_try_passwords.clear();
				ai_try_password_idx = 0;
				clue.clear();
				try_password = -1;
				password = -1;
				ai_mode = -1;
				ai_path.clear();
				ai_path_idx = 0;
				seed = 0;
				bool is_show_resource = false;
				bool is_ai = false;
				bool is_maze_generate = false;
				now_layer = 0;

				state = Init;
				maze.reset(1, 1, 1);
				player.reset();
				
				closegraph();
				initgraph(800, 600);
				return;
			}
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

		if (player.get_is_boss())
		{
			state = Boss;
			try_password = -1;
			closegraph();
			initgraph(1080, 720);
		}
		break;
	case Ai:
		if (!is_maze_generate) // 如果迷宫还没有生成，则生成迷宫
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
			method1_tries = cs.get_attempts1().size();
			method2_tries = cs.get_attempts2().size();
			closegraph();
			initgraph(1080, 720);
		}

		if (player.get_is_boss())
		{
			state = Boss;
			try_password = -1;
			closegraph();
			initgraph(1080, 720);
		}

		if (player.get_pos() == maze.get_end_pos(now_layer) && ai_path_idx == ai_path.size())
		{
			if (now_layer == layers - 1)
			{
				if (layers == 1)
				{
					std::string filename = generate_filename();
					save_ai_result_to_json(filename);
					MessageBox(GetHWnd(), _T("迷宫结束,AI的结果已保存在maze_test文件夹里"), _T("成功"), MB_OK);
				}
				else
				{
					MessageBox(GetHWnd(), _T("迷宫结束"), _T("成功"), MB_OK);
				}

				ai_try_passwords.clear();
				ai_try_password_idx = 0;
				clue.clear();
				try_password = -1;
				password = -1;
				ai_mode = -1;
				ai_path.clear();
				ai_path_idx = 0;
				seed = 0;
				bool is_show_resource = false;
				bool is_ai = false;
				bool is_maze_generate = false;
				now_layer = 0;

				state = Init;
				maze.reset(1, 1, 1);
				player.reset();

				closegraph();
				initgraph(800, 600);
				return;
			}
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
		settextstyle(25, 10, L"微软雅黑");
		outtextxy(40, 130, L"种子");
		outtextxy(40, 210, L"行数");
		outtextxy(40, 290, L"列数");
		outtextxy(40, 370, L"层数");
		outtextxy(40, 450, L"游玩类型");
		outtextxy(400, 210, L"输出n个迷宫");
		if (ai_selector.GetSelectedText() == L"AI")
		{
			outtextxy(40, 530, L"AI策略");
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
		break;
	}
	case Locker:
	{
		settextstyle(28, 11, L"微软雅黑");

		std::wstring text = L"当前金币: " + std::to_wstring(player.get_resource());
		outtextxy(20, 160, text.c_str());
		text = L"目标密码的哈希值:" + string_to_wstring(sha256::hash_string(to_three_digit_string(password)));
		outtextxy(20, 200, text.c_str());
		if (try_password != -1)
		{
			text = L"尝试密码:" + string_to_wstring(to_three_digit_string(try_password));
			outtextxy(20, 240, text.c_str());
			text = L"尝试密码的哈希值:" + string_to_wstring(sha256::hash_string(to_three_digit_string(try_password)));
			outtextxy(20, 280, text.c_str());
		}

		for (int i = 0; i < clue.size(); i++)
		{
			if (clue[i].size() == 2)
			{
				if (clue[i][0] == -1)
				{
					text = L"线索" + std::to_wstring(i) + L": 三位都是是素数并且不重复";
				}
				else
				{
					text = L"线索" + std::to_wstring(i) + L": 第" + std::to_wstring(clue[i][0]) + L"位是" + (clue[i][1] ? L"是奇数" : L"是偶数");
				}
			}
			else
			{
				for (int j = 0; j < 3; j++)
				{
					if (clue[i][j] != -1)
					{
						text = L"线索" + std::to_wstring(i) + L": 第" + std::to_wstring(j + 1) + L"位是" + std::to_wstring(clue[i][j]);
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

void MazeGame::reset()
{
	ai_try_passwords.clear();
	ai_try_password_idx = 0;
	clue.clear();
	try_password = -1;
	password = -1;
	ai_mode = -1;
	ai_path.clear();
	ai_path_idx = 0;
	seed = 0;
	bool is_show_resource = false;
	bool is_ai = false;
	bool is_maze_generate = false;
	now_layer = 0;
	
	state = Init;
	maze.reset(1, 1, 1);
	player.run_reset();
}

void MazeGame::save_ai_result_to_json(std::string filename)
{
	Json::Value root;
	Json::Value maze_array(Json::arrayValue);

	auto m = maze.get_layer(0).get_simple_grid();

	for (const auto& row : m)
	{
		Json::Value json_row(Json::arrayValue);
		for (TileType c : row)
		{
			std::string s;
			switch (c)
			{
			case TileType::Wall:
				s = "#";
				break;
			case TileType::Path:
			case TileType::Empty:
			case TileType::DOOR:
				s = " ";
				break;
			case TileType::Start:
				s = "S";
				break;
			case TileType::End:
				s = "E";
				break;
			case TileType::Gold:
				s = "G";
				break;
			case TileType::Trap:
				s = "T";
				break;
			case TileType::Boss:
				s = "B";
				break;
			case TileType::Locker:
				s = "L";
				break;
			}
			json_row.append(s);
		}
		maze_array.append(json_row);
	}

	root["maze"] = maze_array;
	root["seed"] = seed;

	Json::Value json_path(Json::arrayValue);

	for (const auto& vec : ai_path)
	{
		Json::Value point(Json::arrayValue);
		point.append((int)vec.y);
		point.append((int)vec.x);
		json_path.append(point);
	}

	root["optimal_path"] = json_path;

	root["max_resource"] = player.get_resource();

	Json::Value json_C(Json::arrayValue);
	for (const auto& row : clue) 
	{
		Json::Value json_row(Json::arrayValue);
		for (int val : row) 
		{
			json_row.append(val);
		}
		json_C.append(json_row);
	}
	root["C"] = json_C;
	root["L"] = sha256::hash_string(to_three_digit_string(password));
	root["password"] = password;

	Json::Value results;

	auto build_method_result = [&](int tries) 
		{
		Json::Value result;
		result["tries"] = tries;
		return result; 
		};

	results["method1"] = build_method_result(method1_tries);
	results["method2"] = build_method_result(method2_tries);

	root["results"] = results;

	std::ofstream ofs(filename);
	if (!ofs.is_open())
	{
		std::cerr << "无法打开文件: " << filename << std::endl;
		return;
	}

	Json::StyledWriter writer;
	ofs << writer.write(root);

	ofs.close();
}
