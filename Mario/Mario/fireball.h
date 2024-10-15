#pragma once

#include "vector2.h"
#include "animation.h"
#include "platform.h"

#include <functional>

extern bool is_debug;

extern Atlas fireball;//火球动画
extern std::vector<Platform> platforms;//平台数组
extern std::vector<Wall> walls;//墙数组

class Fireball
{
public:
	Fireball()
	{
		//if (direction != 0) is_right = direction > 0;
		//position = pos;
		//if(is_right) position.x = pos.x + 40;
		//else position.x = pos.x - 20;
		animation.set_atlas(&fireball);
		animation.set_interval(100);
		animation.set_loop(true);

		size.x = 20; size.y = 20;
		//if (is_right) set_speed(0.3f, 0.1f);
		//else set_speed(-0.3f, 0.1f);
	}
	~Fireball() = default;


	void on_update(int delta)
	{
		//挺有意思的一段代码,实现跟踪角色方向功能
		//if (!direction) speed.x = -0.3f;
		//else speed.x = 0.3f;
		animation.on_update(delta);
		move_and_collide(delta);
	}

	void on_draw(const Camera& camera)
	{
		animation.on_draw(position, camera);
		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			rectangle(camera, (int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
		}
	}

	void set_speed(float x, float y)
	{
		speed.x = x;
		speed.y = y;

	}
	
	bool check_collision(const Vector2& position, const Vector2& size)
	{
		return this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y
			&& this->position.y + this->size.y / 2 <= position.y + size.y;
	}
	
	void move_and_collide(int delta)
	{

		if (position.y < 400) speed.y = -speed.y; //角色高度
		for (const Platform& platform : platforms)
		{
			const Platform::CollisionShape& shape = platform.shape;
			bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left) <= size.x + (shape.right - shape.left));
			bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);
			//这里的代码有点小问题
			if (is_collide_y && is_collide_x)
			{
				speed.y = -speed.y;
			}
		}
		for (const Wall& wall : walls)//检测角色与地面碰撞
		{
			const Wall::CollisionShape& shape = wall.shape;
			bool is_collide_y = (max(position.y + size.y, shape.bottom) - min(position.y, shape.top) <= size.y + (shape.bottom - shape.top));
			bool is_collide_x = (shape.x >= position.x && shape.x <= position.x + size.x);

			if (is_collide_x && is_collide_y)
			{
				speed.x = -speed.x;
			}
		}
		position += speed * (float)delta;
	}

	Vector2 get_size()
	{
		return size;
	}

	void set_position(const Vector2& pos)
	{
		position = pos;
	}

	bool check_delete()
	{
		return can_delete;
	}

	void set_delete(bool can_delete)
	{
		this->can_delete = can_delete;
	}

	void on_collide()
	{
		if (callback) callback();
	}

	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}

private:
	Vector2 size;
	Vector2 speed;
	Vector2 position;
	Animation animation;
	const float gravity = 1.6e-3f;

	bool is_right = true;
	bool can_delete = false;

	std::function<void()> callback;
};