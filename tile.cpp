#include "tile.h"
#include "resources_manager.h"
#include "util.h"

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

	anim_gold.on_render(0.05);
}


