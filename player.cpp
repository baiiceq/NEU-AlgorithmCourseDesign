#include "player.h"
#include "maze.h"

Player::Player(Vector2 start, int hp, int initResource)
{
	timer_move.set_one_shot(true);
	timer_move.set_wait_time(RUN_TIME);
	timer_move.set_on_timeout([&]()
		{
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

void Player::on_input(const ExMessage& msg)
{
	if (msg.message == WM_KEYDOWN)
	{
		switch (msg.vkcode)
		{
		case VK_LEFT:
		case 0x41:
			is_running_left = true;
			break;
		case VK_RIGHT:
		case 0x44:
			is_running_right = true;
			break;
		case VK_UP:
		case 0x57:
			is_running_up = true;
			break;
		case VK_DOWN:
		case 0x53:
			is_running_down = true;
			break;
		default:
			break;
		}
	}
	else if (msg.message == WM_KEYUP)
	{
		switch (msg.vkcode)
		{
		case VK_LEFT:
		case 0x41:
			is_running_left = false;
			break;
		case VK_RIGHT:
		case 0x44:
			is_running_right = false;
			break;
		case VK_UP:
		case 0x57:
			is_running_up = false;
			break;
		case VK_DOWN:
		case 0x53:
			is_running_down = false;
			break;
		default:
			break;
		}
	}
}

void Player::on_update(int delta, MazeLayer& ml)
{
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

	Vector2 target_image_pos = Grid::get_image_pos(pos);

	if (!(image_pos == target_image_pos))
	{
		timer_move.on_update(delta);
		is_idle = false;

		float t = timer_move.get_ratio();
		image_pos = last_image_pos + (target_image_pos - last_image_pos) * t;

	}
	else
	{
		if (is_running_right)
		{
			is_facing_right = true;
			if (move_to(pos + Vector2(1, 0), ml))
				return;
		}

		if (is_running_left)
		{
			is_facing_right = false;
			if (move_to(pos + Vector2(-1, 0), ml))
				return;

		}

		if (is_running_up)
		{
			if (move_to(pos + Vector2(0, -1), ml))
				return;
		}

		if (is_running_down)
		{
			if (move_to(pos + Vector2(0, 1), ml))
				return;
		}

		is_idle = true;

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

bool Player::move_to(Vector2 pos, MazeLayer& ml)
{
	if (pos.x < 0 || pos.x >= ml.getCols() || pos.y < 0 || pos.y >= ml.getRows())
	{
		return false; // Ô½½ç
	}

	if (ml.isWalkable(pos.x, pos.y))
	{
		last_image_pos = image_pos;
		this->pos = pos; 
		timer_move.restart();
		ml.get_tile(pos.x, pos.y)->on_enter(*this);
		return true;
	}

	return false;
}

void Player::set_position(Vector2 pos)
{
	this->pos = pos;
	image_pos = Grid::get_image_pos(pos);	
}
