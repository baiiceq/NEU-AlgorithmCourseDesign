#pragma once
#include "maze.h"
#include "text_input.h"
#include "option_selector.h"
#include "player.h"
#include "random.h"
#include "optimal_path.h"
#include "button.h"
#include "util.h"
#include "session.h"
#include "password.h"
#include "Greedy.h"
#include <string>
#include <graphics.h>
#include <sstream>
#include <iomanip>
#include <codecvt>

class MazeGame
{
private:
	enum GameState
	{
		Init,
		Generate,
		Gamer,
		Ai,
		Locker,
		Boss
	};

public:
    MazeGame();
    
	void on_update(int delta);

	void on_render();

	void on_input(const ExMessage& msg);

private:
	std::string to_three_digit_string(int num) 
	{
		std::ostringstream oss;
		oss << std::setw(3) << std::setfill('0') << num;
		return oss.str();
	}

	std::wstring string_to_wstring(const std::string& str) 
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.from_bytes(str);
	}


private:
    Maze maze;
    Player player;
	GameState state;

	std::vector<Vector2> path_with_no_resource; // ��������Դ��·��
	int path_with_no_resource_idx = 0;

	int layers;
	int rows;
	int cols;

	int now_layer = 0; // ��ǰ����

	bool is_show_resource = false;
	bool is_ai = false;
	bool is_maze_generate = false;

	unsigned int seed;

private:
	Button start_button;
	Button exit_button;
	TextInput seed_input;
	OptionSelector row_selector;
	OptionSelector col_selector;
	OptionSelector layer_selector;
	OptionSelector ai_selector;
	OptionSelector ai_mode_selector;

	TextInput password_input;
	Button enter_button;
	Button skip_button;
	Button load_button;

	OptionSelector count_selector;
	Button generate_button;

	GreedyPick gp;

	int ai_mode;
	std::vector<Vector2> ai_path;
	int ai_path_idx;
	
private:
	int password;
	int try_password;
	std::vector<std::vector<int> > clue;

	int ai_try_password_idx;
	std::vector<std::string> ai_try_passwords;

	std::wstring open_json_file_dialog() 
	{
		wchar_t filename[MAX_PATH] = L"";

		OPENFILENAME ofn = { 0 };
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = GetHWnd(); 
		ofn.lpstrFilter = L"JSON �ļ� (*.json)\0*.json\0�����ļ� (*.*)\0*.*\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
		ofn.lpstrTitle = L"ѡ��һ���Թ� JSON �ļ�";

		if (GetOpenFileName(&ofn))
		{
			return std::wstring(filename);
		}
		else 
		{
			return L""; 
		}
	}
};

