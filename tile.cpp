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
	if (is_triggered)
		return;
	
	Tile::on_render();

	anim_gold.set_position(Grid::get_image_pos(pos) + Vector2(OFFSET_X, OFFSET_Y));
	anim_gold.on_render(0.05);
}

void Gold::on_update(int delta)
{
	if (is_triggered)
		return;

	anim_gold.on_update(delta);
}

