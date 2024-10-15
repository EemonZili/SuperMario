#pragma once
#include "atlas.h"
#include "timer.h"
#include "util.h" //存放各种工具函数

class Animation
{
public:
	Animation() = default;
	~Animation() = default;

	void set_atlas(Atlas* atlas)
	{
		this->atlas = atlas;
	}

	void set_interval(int interval)
	{
		this->interval = interval;
	}

	void set_loop(bool is_loop)
	{
		this->is_loop = is_loop;
	}

	void on_update(int delta)
	{
		timer += delta;
		if (timer >= interval)
		{
			idx_frame++;
			if (idx_frame == atlas->get_size())
			{
				idx_frame = is_loop ? 0 : idx_frame - 1;
			}
			timer = 0;
		}
	}

	void on_draw(Vector2 postion)
	{
		putimage_alpha((int)postion.x, (int)postion.y, atlas->get_image(idx_frame));
	}

	void on_draw(Vector2 postion, const Camera& camera)
	{
		putimage_alpha(camera, (int)postion.x, (int)postion.y, atlas->get_image(idx_frame));
	}

private:
	int timer = 0;
	int interval = 0;
	int idx_frame = 0;
	bool is_loop = false;
	Atlas* atlas = nullptr;
};