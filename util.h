#pragma once
#include "resources_manager.h"
#include <graphics.h>
#include <windows.h>
#include <string>
#include <mciapi.h>
#include "vector2.h"

class Grid
{
public:
	static const int cellSize = 40;

	static int toPixelX(int row) 
	{
		return row * cellSize;
	}

	static int toPixelY(int col)
	{
		return col * cellSize;
	}

	static int getTileSize()
	{
		return cellSize;
	}

	static Vector2 get_image_pos(Vector2 pos)
	{
		return { pos.x * cellSize, pos.y * cellSize };
	}

	static Vector2 fromPixel(int x, int y)
	{
		return { (float)x / cellSize, (float)y / cellSize };
	}


};


struct Rect
{
	int x, y;
	int w, h;
};

inline void flip_image(IMAGE* src, IMAGE* dst)
{
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h);
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int idx_src = y * w + x;
			int idx_dst = y * w + (w - x - 1);
			dst_buffer[idx_dst] = src_buffer[idx_src];
		}
	}
}

#pragma comment(lib, "MSIMG32.LIB")
#pragma comment(lib,"WINMM.lib")
#pragma comment(lib,"MSIMG32.lib")

inline void putimage_alpha(IMAGE* img, const Rect* rect_dst, const Rect* rect_src = nullptr)
{
	int dst_x = rect_dst->x;
	int dst_y = rect_dst->y;

	static BLENDFUNCTION blend_func = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, rect_dst->w, rect_dst->h,
		GetImageHDC(img), rect_src ? rect_src->x : 0, rect_src ? rect_src->y : 0,
		rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(), blend_func);

}

inline void load_audio(LPCTSTR path, LPCTSTR id)
{
	static TCHAR str_cmd[512];
	_stprintf_s(str_cmd, _T("open %s alias %s"), path, id);
	mciSendString(str_cmd, NULL, 0, NULL);
}

inline void play_audio(LPCTSTR id, int volume, bool is_loop = false)
{
	static TCHAR str_cmd[512];
	_stprintf_s(str_cmd, _T("play %s %s from 0"), id, is_loop ? _T("repeat") : _T(""));
	mciSendString(str_cmd, NULL, 0, NULL);
	_stprintf_s(str_cmd, _T("setaudio \"%s\" volume to %d"), id, volume);
	mciSendString(str_cmd, NULL, 0, NULL);
}

inline void stop_audio(LPCTSTR id)
{
	static TCHAR str_cmd[512];
	_stprintf_s(str_cmd, _T("stop %s"), id);
	mciSendString(str_cmd, NULL, 0, NULL);
}

inline int range_random(int min_num, int max_num)
{
	return min_num + rand() % (max_num - min_num + 1);
}

inline std::wstring get_current_time()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	wchar_t buf[32];
	swprintf(buf, 32, L"%04d.%02d.%02d-%02d.%02d.%02d",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	return buf;
}

template<typename T>
T clamp(T val, T min_val, T max_val) 
{
	return max(min_val, min(val, max_val));
}