#pragma once

#include "util.h"

extern bool is_debug;

class Wall
{
public:
	struct CollisionShape
	{
		float x;
		float top, bottom;
	};

	CollisionShape shape;
	IMAGE* img = nullptr;
	POINT render_position = { 0 };

public:
	Wall() = default;
	~Wall() = default;

	void on_draw(const Camera& camera) const
	{
		if (img != nullptr) putimage_alpha(render_position.x, render_position.y, img);

		if (is_debug)
		{
			setlinecolor(RGB(255, 0, 0));
			const Vector2& pos = camera.get_position();
			line((int)(shape.x - pos.x), (int)shape.top, (int)(shape.x - pos.x), (int)shape.bottom);
		}
	}

	void set(float top, float bottom, float x)
	{
		shape.top = top;
		shape.bottom = bottom;
		shape.x = x;
	}
};