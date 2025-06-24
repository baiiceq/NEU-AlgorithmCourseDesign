#include "tile.h"
#include "resources_manager.h"
#include "util.h"

void Tile::on_render()
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

Gold::Gold(int value) : Tile(TileType::Gold), value(value) 
{
	anim_gold.add_frame(ResourcesManager::instance()->find_atlas("gold"));
	anim_gold.set_position(Grid::get_image_pos(pos));
	anim_gold.set_anchor_mode(Animation::AnchorMode::BottomCentered);
	anim_gold.set_loop(true);
	anim_gold.set_interval(120);
}

void Gold::on_render()
{
	Tile::on_render();

	if (is_triggered)
		return;
	
	anim_gold.set_position(Grid::get_image_pos(pos) + Vector2(OFFSET_X, OFFSET_Y));
	anim_gold.on_render(0.05);
}

void Gold::on_update(int delta)
{
	if (is_triggered)
		return;

	anim_gold.on_update(delta);
}

void Gold::on_enter(Player& player)
{
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

void Trap::on_render()
{
	Tile::on_render();

	anim_trap.set_position(Grid::get_image_pos(pos) + Vector2(OFFSET_X, OFFSET_Y));
	anim_trap.on_render();
}

void Trap::on_update(int delta)
{
	anim_trap.on_update(delta);
}

void Trap::on_enter(Player& player)
{
	player.add_resource(-damage);
}

void Start::on_render()
{
	Tile::on_render();
	const int TILE_SIZE = Grid::cellSize;
	Rect rect = { Grid::toPixelX(pos.x), Grid::toPixelY(pos.y), TILE_SIZE, TILE_SIZE };
	putimage_alpha(ResourcesManager::instance()->find_image("start"), &rect);
}

void End::on_render()
{
	Tile::on_render();
	const int TILE_SIZE = Grid::cellSize;
	Rect rect = { Grid::toPixelX(pos.x), Grid::toPixelY(pos.y), TILE_SIZE, TILE_SIZE };
	putimage_alpha(ResourcesManager::instance()->find_image("exit"), &rect);
}

void End::on_enter(Player& player)
{

}
