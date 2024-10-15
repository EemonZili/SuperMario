#pragma once

#include "util.h"

extern bool is_debug;

class Platform
{
public:
	struct CollisionShape
	{
		float y;
		float left, right;
	};

	CollisionShape shape;
	IMAGE* img = nullptr;
	POINT render_position = { 0 };

public:
	Platform() = default;
	~Platform() = default;

	void on_draw(const Camera& camera) const
	{
		if (img != nullptr) putimage_alpha(render_position.x, render_position.y, img);

		if (is_debug)
		{
			setlinecolor(RGB(255, 0, 0));
			const Vector2& pos = camera.get_position();
			line((int)(shape.left - pos.x), (int)shape.y, (int)(shape.right - pos.x), (int)shape.y);
		}
	}

	void set(float left, float right, float y)
	{
		shape.left = left;
		shape.right = right;
		shape.y = y;
	}
};