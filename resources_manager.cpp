#include "util.h"
#include "resources_manager.h"
#include <fstream>  
#include <sstream>

struct ImageResInfo
{
	std::string id;
	LPCTSTR path;
};

struct AtlasResInfo
{
	std::string id;
	LPCTSTR path;
	int num_frame = 0;
};

static const std::vector<ImageResInfo> image_info_list =
{
	{"floor",                  _T(R"(resources\floor.png)")},
	{"wall",                   _T(R"(resources\wall.png)")},
	{"trap",                   _T(R"(resources\trap.png)")},
	{"start",                  _T(R"(resources\start.png)")},
	{"exit",                   _T(R"(resources\exit.png)")},
	{"locker",                 _T(R"(resources\locker.png)")},
	{"locker_right",           _T(R"(resources\locker_right.png)")},
	{"locker_red",             _T(R"(resources\locker_red.png)")},
	{"right_path",             _T(R"(resources\right_path.png)")},
	{"boss",                   _T(R"(resources\boss.png)")},
	{"die_boss",               _T(R"(resources\die_boss.png)")},
};

static const std::vector<AtlasResInfo> atlas_info_list =
{
	{"player_run_right",             _T(R"(resources\player\run\%d.png)") ,     6},
	{"player_idle_right",            _T(R"(resources\player\idle\%d.png)") ,    4},
	{"gold",                         _T(R"(resources\coin\Gold_%d.png)") ,      10},
};

static inline bool check_image_valid(IMAGE* image)
{
	return GetImageBuffer(image);
}

void ResourcesManager::load()
{
	AddFontResourceEx(_T("resources/font.ttf"), FR_PRIVATE, NULL);

	for (const auto& info : image_info_list)
	{
		IMAGE* image = new IMAGE();
		loadimage(image, info.path);
		if (!check_image_valid(image))
			throw info.path;
		image_pool[info.id] = image;
	}

	for (const auto& info : atlas_info_list)
	{
		Atlas* atlas = new Atlas();
		atlas->load_from_file(info.path, info.num_frame);
		for (int i = 0; i < info.num_frame; i++)
		{
			IMAGE* image = atlas->get_image(i);
			if (!check_image_valid(image))
				throw info.path;
		}

		atlas_pool[info.id] = atlas;
	}

	flip_atlas("player_run_right", "player_run_left");
	flip_atlas("player_idle_right", "player_idle_left");

}

ResourcesManager* ResourcesManager::manager = nullptr;

ResourcesManager* ResourcesManager::instance()
{
	if (!manager)
		manager = new ResourcesManager();

	return manager;
}

ResourcesManager::ResourcesManager()
{
}

ResourcesManager::~ResourcesManager()
{
}


Atlas* ResourcesManager::find_atlas(const std::string& id) const
{
	const auto& itor = atlas_pool.find(id);
	if (itor == atlas_pool.end())
		return nullptr;
	
	return itor->second;
}

IMAGE* ResourcesManager::find_image(const std::string& id) const
{
	const auto& itor = image_pool.find(id);
	if (itor == image_pool.end())
		return nullptr;

	return itor->second;
}

void ResourcesManager::flip_image(IMAGE* src_image, IMAGE* dst_image, int num_h)
{
	int w = src_image->getwidth();
	int h = src_image->getheight();
	int w_frame = w / num_h;
	Resize(dst_image, w, h);
	DWORD* src_buffer = GetImageBuffer(src_image);
	DWORD* dst_buffer = GetImageBuffer(dst_image);
	for (int i = 0; i < num_h; i++)
	{
		int x_left = i * w_frame;
		int x_right = (i + 1) * w_frame;
		for (int y = 0; y < h; y++)
		{
			for (int x = x_left; x < x_right; x++)
			{
				int idx_src = y * w + x;
				int idx_dst = y * w + x_right - (x - x_left);
				dst_buffer[idx_dst] = src_buffer[idx_src];
			}
		}
	}
	
}

void ResourcesManager::flip_image(const std::string& src_id, const std::string dst_id, int num_h)
{
	IMAGE* src_image = image_pool[src_id];
	IMAGE* dst_image = new IMAGE();

	flip_image(src_image, dst_image, num_h);

	image_pool[dst_id] = dst_image;
}

void ResourcesManager::flip_atlas(const std::string& src_id, const std::string dst_id)
{
	Atlas* src_atlas = atlas_pool[src_id];
	Atlas* dst_atlas = new Atlas();

	for (int i = 0; i < src_atlas->get_size(); i++)
	{
		IMAGE img_flipped;
		flip_image(src_atlas->get_image(i), &img_flipped);
		dst_atlas->add_image(img_flipped);
	}

	atlas_pool[dst_id] = dst_atlas;
}

