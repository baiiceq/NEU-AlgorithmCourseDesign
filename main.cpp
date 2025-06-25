#include <iostream>
#include <graphics.h>
#include "random.h"
#include "maze.h"
#include "maze_game.h"


int main()
{
	HWND hwnd = initgraph(800 + 400, 800, EW_SHOWCONSOLE);

	try
	{
		ResourcesManager::instance()->load();
	}
	catch (const LPCTSTR id)
	{
		TCHAR err_msg[512];
		_stprintf_s(err_msg, _T("�޷����أ�%s"), id);
		MessageBox(hwnd, err_msg, _T("��Դ����ʧ��"), MB_OK | MB_ICONERROR);
		return -1;
	}

	Random::init(114514);

	ExMessage msg;
	const int FPS = 60;

	bool running = true;

	MazeGame game(3, 20, 20);

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