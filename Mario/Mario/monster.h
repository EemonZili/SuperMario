#pragma once

#include "wall.h"
#include "fireball.h"
#include "animation.h"

extern bool is_debug;

extern std::vector<Wall> walls;//墙数组
extern std::vector<Fireball*> fireballs; 

class Monster
{
public:
	Monster()
	{
		position.y = 485; //这是地面上的位置，怪物默认生成在地面上
		animation.set_interval(500);
		animation.set_loop(true);
	}
	~Monster() = default;

	void set_atlas(Atlas* atlas)
	{
		this->animation.set_atlas(atlas);
	}

	void set_position(const Vector2& pos)
	{
		position = pos;
	}

	void set_x(float x)
	{
		position.x = x;
	}

	void set_size(const Vector2& size)
	{
		this->size = size;
	}
	
	void set_speed(const Vector2& speed)
	{
		this->speed = speed;
	}

	void on_update(int delta)
	{
		animation.on_update(delta);
		collision();
		position += speed;
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

	void collision()
	{
		for (const Wall& wall : walls)//检测与墙的碰撞
		{
			const Wall::CollisionShape& shape = wall.shape;
			bool is_collide_y = (max(position.y + size.y, shape.bottom) - min(position.y, shape.top) <= size.y + (shape.bottom - shape.top));
			bool is_collide_x = (shape.x >= position.x && shape.x <= position.x + size.x);

			if (is_collide_x && is_collide_y)
			{
				speed.x = -speed.x;
			}
		}

		for (Fireball* fireball : fireballs)
		{
			if (fireball->check_collision(position, size))
			{
				fireball->set_delete(true);
				is_dead = true;
				break;
			}
		}
	}

	bool check_collision(const Vector2& position, const Vector2& size)
	{
		return (this->position.x + this->size.x / 2 >= position.x
			&& this->position.x + this->size.x / 2 <= position.x + size.x
			&& this->position.y + this->size.y / 2 >= position.y
			&& this->position.y + this->size.y / 2 <= position.y + size.y);
	}

private:
	Animation animation;
	Vector2 position;
	Vector2 speed;
	Vector2 size;

	bool is_dead = false;
};