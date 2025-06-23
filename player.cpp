#include "player.h"

Player::Player(Vector2 start, int hp, int initResource)
{
	timer_move.set_one_shot(true);
	timer_move.set_wait_time(RUN_TIME);
	timer_move.set_on_timeout([&]()
		{
			is_idle = true;
			image_pos = Grid::get_image_pos(pos);
		});

	anim_idle_left.add_frame(ResourcesManager::instance()->find_atlas("player_idle_left"));
	anim_idle_left.set_position({ 0,0 });
	anim_idle_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
	anim_idle_left.set_loop(true);
	anim_idle_left.set_interval(120);

	anim_idle_right.add_frame(ResourcesManager::instance()->find_atlas("player_idle_right"));
	anim_idle_right.set_position({ 0,0 });
	anim_idle_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
	anim_idle_right.set_loop(true);
	anim_idle_right.set_interval(120);

	anim_run_left.add_frame(ResourcesManager::instance()->find_atlas("player_run_left"));
	anim_run_left.set_position({ 0,0 });
	anim_run_left.set_anchor_mode(Animation::AnchorMode::BottomCentered);
	anim_run_left.set_loop(true);
	anim_run_left.set_interval(120);

	anim_run_right.add_frame(ResourcesManager::instance()->find_atlas("player_run_right"));
	anim_run_right.set_position({ 0,0 });
	anim_run_right.set_anchor_mode(Animation::AnchorMode::BottomCentered);
	anim_run_right.set_loop(true);
	anim_run_right.set_interval(120);

	pos = start;
}

void Player::on_input(const ExMessage& msg, const MazeLayer& ml)
{
	if (!is_idle)
		return;

	if (msg.message == WM_KEYDOWN)
	{
		switch (msg.vkcode)
		{
		case VK_LEFT:
		case 0x41:
			move_to(pos + Vector2(-1, 0), ml);
			break;
		case VK_RIGHT:
		case 0x44:
			move_to(pos + Vector2(1, 0), ml);
			break;
		case VK_UP:
		case 0x57:
			move_to(pos + Vector2(0, -1), ml);
			break;
		case VK_DOWN:
		case 0x53:
			move_to(pos + Vector2(0, 1), ml);
			break;
		default:
			break;
		}
	}
}

void Player::on_update(int delta)
{
	std::cout << pos.x << " " << pos.y << std::endl;
	Vector2 target_image_pos = Grid::get_image_pos(pos);

	if (!(image_pos == target_image_pos))
	{
		timer_move.on_update(delta);
		is_idle = false;
		if (image_pos.x <= Grid::get_image_pos(pos).x)
			is_facing_right = true;
		else
			is_facing_right = false;

		float t = timer_move.get_ratio();
		image_pos = image_pos + (target_image_pos - image_pos) * t;

	}
	else
	{
		is_idle = true;
	}


	if (is_idle)
	{
		if (is_facing_right)
		{
			anim_idle_right.on_update(delta);
		}
		else
		{
			anim_idle_left.on_update(delta);
		}
	}
	else
	{
		if (is_facing_right)
		{
			anim_run_right.on_update(delta);
		}
		else
		{
			anim_run_left.on_update(delta);
		}
	}
}

void Player::on_render()
{
	if (is_idle)
	{
		if (is_facing_right)
		{
			anim_idle_right.set_position(image_pos + Vector2(OFFESET_X, OFFESET_Y));
			anim_idle_right.on_render(0.3);
		}
		else
		{
			anim_idle_left.set_position(image_pos + Vector2(OFFESET_X, OFFESET_Y));
			anim_idle_left.on_render(0.3);
		}
	}
	else
	{
		if (is_facing_right)
		{
			anim_run_right.set_position(image_pos + Vector2(OFFESET_X, OFFESET_Y));
			anim_run_right.on_render(0.3);
		}
		else
		{
			anim_run_left.set_position(image_pos + Vector2(OFFESET_X, OFFESET_Y));
			anim_run_left.on_render(0.3);
		}
	}
}

void Player::move_to(Vector2 pos, const MazeLayer& ml)
{
	if (ml.isWalkable(pos.x, pos.y))
	{
		this->pos = pos; 
		timer_move.restart();
	}
}
