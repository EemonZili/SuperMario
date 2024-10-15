#pragma once

#include "vector2.h"

class Camera
{
public:
	Camera() {}
	~Camera() {}

	void set_position(const Vector2& pos)
	{
		position.x = pos.x - 150;
		if (position.x < 0)
		{
			position.x = 0;
		}
	}

	Vector2 get_position() const
	{
		return position;
	}

	void reset()
	{
		position = Vector2(0, 0);
	}


private:
	Vector2 position;
};