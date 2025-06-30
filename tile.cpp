#include "tile.h"
#include "resources_manager.h"
#include "util.h"
#include "random.h"

Tile::Tile(const Tile& t)
{
	this->pos = t.pos;
	this->type = t.type;
}

void Tile::on_render(bool is_show_resource)
{
	const int TILE_SIZE = Grid::cellSize;
	if (type == TileType::Wall)
	{
		Rect rect = { Grid::toPixelX(pos.x), Grid::toPixelY(pos.y), TILE_SIZE, TILE_SIZE };
		putimage_alpha(ResourcesManager::instance()->find_image("wall"), &rect);
	}
	else
	{
		Rect rect = { Grid::toPixelX(pos.x), Grid::toPixelY(pos.y), TILE_SIZE, TILE_SIZE };
		putimage_alpha(ResourcesManager::instance()->find_image("floor"), &rect);
	}
}

void Tile::on_update(int delta)
{

}

void Tile::on_enter(Player& player)
{
	player.add_resource(-1);
}

Gold::Gold(int value) : Tile(TileType::Gold), value(value) 
{
	anim_gold.add_frame(ResourcesManager::instance()->find_atlas("gold"));
	anim_gold.set_position(Grid::get_image_pos(pos));
	anim_gold.set_anchor_mode(Animation::AnchorMode::BottomCentered);
	anim_gold.set_loop(true);
	anim_gold.set_interval(120);
}

Gold::Gold(const Gold& g) : Tile(g)
{
	this->value = g.value;
	anim_gold.add_frame(ResourcesManager::instance()->find_atlas("gold"));
	anim_gold.set_position(Grid::get_image_pos(pos));
	anim_gold.set_anchor_mode(Animation::AnchorMode::BottomCentered);
	anim_gold.set_loop(true);
	anim_gold.set_interval(120);
}

void Gold::on_render(bool is_show_resource)
{
	Tile::on_render(is_show_resource);

	if (is_triggered)
		return;
	
	anim_gold.set_position(Grid::get_image_pos(pos) + Vector2(OFFSET_X, OFFSET_Y));
	anim_gold.on_render(0.05);

	if (is_show_resource)
	{
		setbkmode(TRANSPARENT);
		settextstyle(20, 8, L"΢���ź�");
		settextcolor(BLACK);
		std::wstring gold_text = std::to_wstring(value);
		outtextxy(Grid::toPixelX(pos.x) + 10, Grid::toPixelY(pos.y) + 10, gold_text.c_str());
		settextcolor(WHITE);
	}
}

void Gold::on_update(int delta)
{
	if (is_triggered)
		return;

	anim_gold.on_update(delta);
}

void Gold::on_enter(Player& player)
{
	Tile::on_enter(player);
	if (is_triggered)return;
	player.add_resource(value);
	is_triggered = true;
}

Trap::Trap(int damage) : Tile(TileType::Trap), damage(damage)
{
	anim_trap.add_frame(ResourcesManager::instance()->find_image("trap"), 8);
	anim_trap.set_position(Grid::get_image_pos(pos));
	anim_trap.set_anchor_mode(Animation::AnchorMode::BottomCentered);
	anim_trap.set_loop(true);
	anim_trap.set_interval(120);
}

Trap::Trap(const Trap& t) : Tile(t)
{
	anim_trap.add_frame(ResourcesManager::instance()->find_image("trap"), 8);
	anim_trap.set_position(Grid::get_image_pos(pos));
	anim_trap.set_anchor_mode(Animation::AnchorMode::BottomCentered);
	anim_trap.set_loop(true);
	anim_trap.set_interval(120);
	this->damage = t.damage;
}

void Trap::on_render(bool is_show_resource)
{
	Tile::on_render(is_show_resource);

	anim_trap.set_position(Grid::get_image_pos(pos) + Vector2(OFFSET_X, OFFSET_Y));
	anim_trap.on_render();

	if (is_show_resource)
	{
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		settextstyle(20, 8, L"΢���ź�");
		std::wstring damage_text = std::to_wstring(damage);
		outtextxy(Grid::toPixelX(pos.x) + 10, Grid::toPixelY(pos.y) + 10, damage_text.c_str());
		settextcolor(WHITE);
	}
}

void Trap::on_update(int delta)
{
	anim_trap.on_update(delta);
}

void Trap::on_enter(Player& player)
{
	Tile::on_enter(player);
	player.add_resource(-damage);
}

void Start::on_render(bool is_show_resource)
{
	Tile::on_render(is_show_resource);
	const int TILE_SIZE = Grid::cellSize;
	Rect rect = { Grid::toPixelX(pos.x), Grid::toPixelY(pos.y), TILE_SIZE, TILE_SIZE };
	putimage_alpha(ResourcesManager::instance()->find_image("start"), &rect);
}

void End::on_render(bool is_show_resource)
{
	Tile::on_render(is_show_resource);
	const int TILE_SIZE = Grid::cellSize;
	Rect rect = { Grid::toPixelX(pos.x), Grid::toPixelY(pos.y), TILE_SIZE, TILE_SIZE };
	putimage_alpha(ResourcesManager::instance()->find_image("exit"), &rect);
}

void End::on_enter(Player& player)
{
	Tile::on_enter(player);
}

Locker::Locker() : Tile(TileType::Locker)
{
	password = Random::randint(0, 999);
	generate_clue();
}

void Locker::on_render(bool is_show_resource)
{
	Tile::on_render(is_show_resource);
	const int TILE_SIZE = Grid::cellSize;
	Rect rect = { Grid::toPixelX(pos.x) + 2, Grid::toPixelY(pos.y) + 2, TILE_SIZE, TILE_SIZE };
	if(is_triggered) putimage_alpha(ResourcesManager::instance()->find_image("locker_right"), &rect);
	else putimage_alpha(ResourcesManager::instance()->find_image("locker_red"), &rect);
}

void Locker::on_enter(Player& player)
{
	if (is_triggered)return;

	player.set_is_locker(true);
	is_triggered = true;
}

void Locker::generate_clue()
{
	int d1 = password / 100;
	int d2 = (password / 10) % 10;
	int d3 = password % 10;

	// ���п��ܵ�����
	std::vector<std::vector<int>> candidates;

	// [-1, -1]: ÿλ�������Ҳ��ظ�
	auto is_prime = [](int x)
	{
		return x == 2 || x == 3 || x == 5 || x == 7;
	};
	if (is_prime(d1) && is_prime(d2) && is_prime(d3) &&
		d1 != d2 && d1 != d3 && d2 != d3)
	{
		candidates.push_back({ -1, -1 });
	}

	// [a, 0] �� [a, 1]
	if (d1 % 2 == 0) candidates.push_back({ 1, 0 });
	else             candidates.push_back({ 1, 1 });

	if (d2 % 2 == 0) candidates.push_back({ 2, 0 });
	else             candidates.push_back({ 2, 1 });

	if (d3 % 2 == 0) candidates.push_back({ 3, 0 });
	else             candidates.push_back({ 3, 1 });

	// ��ȷĳһλ��ĳ������
	candidates.push_back({ d1, -1, -1 });
	candidates.push_back({ -1, d2, -1 });
	candidates.push_back({ -1, -1, d3 });

	// �������
	std::shuffle(candidates.begin(), candidates.end(), Random::get_rng());

	// ���ѡ1-3������
	int count = Random::randint(1, 3);
	for (int i = 0; i < count && i < candidates.size(); ++i)
	{
		clue.push_back(candidates[i]);
	}

}

Boss::Boss():Tile(TileType::Boss)
{
	generate_hps_and_skills();
}

void Boss::generate_hps_and_skills()
{
	int boss_cnt = Random::randint(3, 5);
	for (int i = 0; i < boss_cnt; i++)
	{
		hps.push_back(Random::randint(5, 30));
	}

	int skill_cnt = Random::randint(2, 4);

	// �չ�
	skills.push_back({ Random::randint(3,5),0 });

	for (int i = 1; i < skill_cnt; ++i)
	{
		int cooldown = Random::randint(1, 2 + i); 

		int damage_base = 6 + i * 2; 
		int damage = Random::randint(damage_base, damage_base + 3);

		skills.push_back({ damage, cooldown });
	}
}

void Boss::on_render(bool is_show_resource)
{
	Tile::on_render(is_show_resource);
	const int TILE_SIZE = Grid::cellSize;
	Rect rect = { Grid::toPixelX(pos.x), Grid::toPixelY(pos.y), TILE_SIZE, TILE_SIZE };
	if (is_triggered) putimage_alpha(ResourcesManager::instance()->find_image("die_boss"), &rect);
	else putimage_alpha(ResourcesManager::instance()->find_image("boss"), &rect);
}

void Boss::on_enter(Player& player)
{
	if (is_triggered)return;
	is_triggered = true;
	//player.set_is_boss(true);
}
