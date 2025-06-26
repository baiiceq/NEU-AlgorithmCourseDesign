﻿#include <iostream>
#include <graphics.h>
#include "random.h"
#include "maze.h"
#include "maze_game.h"
#include "session.h" 



int main()
{
	// 禁用 C4996 警告 (针对 Visual Studio 中的某些标准库函数)
	#ifdef _MSC_VER
	#define _CRT_SECURE_NO_WARNINGS
	#endif

	HWND hwnd = initgraph(800, 600, EW_SHOWCONSOLE);

	try
	{
		ResourcesManager::instance()->load();
	}
	catch (const LPCTSTR id)
	{
		TCHAR err_msg[512];
		_stprintf_s(err_msg, _T("无法加载：%s"), id);
		MessageBox(hwnd, err_msg, _T("资源加载失败"), MB_OK | MB_ICONERROR);
		return -1;
	}

	ExMessage msg;
	const int FPS = 60;

	bool running = true;

	MazeGame game;

	BeginBatchDraw();

	

	while (running)
	{
		DWORD frame_start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			game.on_input(msg);
		}

		static DWORD last_tick_time = GetTickCount();
		DWORD current_tick_time = GetTickCount();
		DWORD delta_tick = current_tick_time - last_tick_time;
		last_tick_time = current_tick_time;

		game.on_update(delta_tick);

		cleardevice();

		game.on_render();

		FlushBatchDraw();
		
		DWORD frame_end_time = GetTickCount();
		DWORD frame_delta_time = frame_end_time - frame_start_time;
		if (frame_delta_time < 1000 / FPS)
			Sleep(1000 / FPS - frame_delta_time);
	}
	EndBatchDraw();

	return 0;
}