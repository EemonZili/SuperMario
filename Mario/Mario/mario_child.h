#pragma once

#include "player.h"
#include "animation.h"
#include "fireball.h"

extern IMAGE mario_child_stand_left;//马路小孩站立图
extern IMAGE mario_child_turn_left;//马里奥小孩转身图
extern IMAGE mario_child_jum_left;//马里奥小孩跳跃图
extern Atlas mario_child_walk_left;//马里奥小孩走路动画
extern IMAGE mario_child_stand_right;//马路小孩站立图
extern IMAGE mario_child_turn_right;//马里奥小孩转身图
extern IMAGE mario_child_jum_right;//马里奥小孩跳跃图
extern Atlas mario_child_walk_right;//马里奥小孩走路动画
extern IMAGE mario_child_dead;//马里奥小孩死亡图

extern std::vector<Platform> platforms;
extern std::vector<Wall> walls;
extern std::vector<Fireball*> fireballs;
extern std::vector<Monster> chestnuts;

class Mario_child : public Player 
{
public:
	Mario_child()
	{
		walk_left.set_atlas(&mario_child_walk_left);
		walk_left.set_interval(100);
		walk_left.set_loop(true);
		walk_right.set_atlas(&mario_child_walk_right);
		walk_right.set_interval(100);
		walk_right.set_loop(true);

		size.x = 40; size.y = 40;
		velocity.x = 0; velocity.y = 0;
		position.x = 150; position.y = 0;

		timer_attack_cd.set_wait_time(attack_cd);
		timer_attack_cd.set_one_shot(true);
		timer_attack_cd.set_callback([&]()
		{
			can_attack = true;
		});
	}
	~Mario_child() = default;


	void set_postion(Vector2 postion)
	{
		this->position = postion;
	}
	
	void on_input(const ExMessage& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{
			case 0x41://'A'
				is_left = true;
				break;
			case 0x44://'D'
				is_right = true;
				break;
			case 0x57://'W'
				if (can_jump)
					on_jump();
				break;
			case 0x4B://'K'
				if (can_attack)
				{
					on_attack();
					can_attack = false;
					timer_attack_cd.reset();
				}
				break;
			}
			break;
		case WM_KEYUP:
			switch (msg.vkcode)
			{
			case 0x41:
				is_left = false;
				break;
			case 0x44:
				is_right = false;
				break;
			}
			break;
		}
	}

	//有很大的问题,需要完善
	void on_jump()
	{
		velocity.y -= jump_bgm ? 0.5f : jump_speed;
		if (jump_bgm)
		{
			mciSendString(_T("play small_jump from 0"), NULL, 0, NULL);
			jump_bgm = false;
		}
	}

	void on_update(int delta, Camera& camera)
	{
		int direction = is_right - is_left;
		if (direction != 0)
		{
			is_face_right = direction > 0;
			if (is_face_right)
				walk_right.on_update(delta);
			else
				walk_left.on_update(delta);

			float distance = direction * speed * delta;
			on_run(distance);
		}
		camera.set_position(position);
		timer_attack_cd.on_update(delta);

		move_and_collide(delta);
		
		//控制跳跃高度
		if (position.y == 485)
		{
			can_jump = true;
			jump_bgm = true;
		}
		else if (position.y <= 390)	can_jump = false;
		
	}

	void on_draw(const Camera& camera)
	{
		//增加debug角色碰撞框
		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			rectangle(camera, (int)position.x, (int)position.y, (int)(position.x + size.x), (int)(position.y + size.y));
		}


		int direction = is_right - is_left;
		if (is_dead)//加载死亡图片
		{
			putimage_alpha(camera, (int)position.x, (int)position.y, &mario_child_dead);
		}
		else if (position.y <= 480)//加载跳跃图片
		{
			if (is_face_right)
				putimage_alpha(camera, (int)position.x, (int)position.y, &mario_child_jum_right);
			else
				putimage_alpha(camera, (int)position.x, (int)position.y, &mario_child_jum_left);
		}
		else if (direction != 0)//加载走路动画
		{
			is_face_right = direction > 0;
			if (is_face_right)
				walk_right.on_draw(position, camera);
			else
				walk_left.on_draw(position, camera);
		}
		else//加载站立图片
		{
			if (is_face_right)
				putimage_alpha(camera, (int)position.x, (int)position.y, &mario_child_stand_right);
			else
				putimage_alpha(camera, (int)position.x, (int)position.y, &mario_child_stand_left);
		}
		//加载结束动画重新开始
		if (is_dead && position.y >= 600)
		{

		}

	}

	void move_and_collide(int delta)
	{
		if (delta > 100) return; //防止初始加载时delta过大，导致角色穿过地面
		velocity.y += gravity * delta;
		position += velocity * (float)delta;
		if (velocity.y > 0)
		{
			for (const Platform& platform : platforms)//检测角色与地面碰撞
			{
				const Platform::CollisionShape& shape = platform.shape;
				bool is_collide_x = (max(position.x + size.x, shape.right) - min(position.x, shape.left) <= size.x + (shape.right - shape.left));
				bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);

				if (is_collide_x && is_collide_y)
				{
					float delta_pos_y = velocity.y * delta;
					float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;
					if (last_tick_foot_pos_y <= shape.y)
					{
						position.y = shape.y - size.y;
						velocity.y = 0;

						break;
					}
				}
			}
			for (const Wall& wall : walls)//检测角色与墙碰撞
			{
				const Wall::CollisionShape& shape = wall.shape;
				bool is_collide_y = (max(position.y + size.y, shape.bottom) - min(position.y, shape.top) <= size.y + (shape.bottom - shape.top));
				bool is_collide_x = (shape.x >= position.x && shape.x <= position.x + size.x);

				if (is_collide_x && is_collide_y)
				{
					if (is_face_right) position.x = shape.x - size.x;
					else position.x = shape.x;
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

			for (Monster chestnut : chestnuts)
			{
				if ()
			}
		}
		if (position.y >= 600)
		{
			position.y = 600;
			is_dead = true;
			velocity.y = -1.0f;
		}
	}

	void on_run(float distance)
	{
		position.x += distance;
	}

	void on_attack()
	{
		mciSendString(_T("play fireball from 0"), NULL, 0, NULL);

		Fireball* fireball = new Fireball();
		Vector2 pos, speed;
		const Vector2& ball_size = fireball->get_size();
		pos.x = is_face_right
			? position.x + size.x
			: position.x - ball_size.x;
		pos.y = position.y;
		speed.x = is_face_right ? 0.3f : -0.3f;
		speed.y = 0.2f;

		fireball->set_position(pos);
		fireball->set_speed(speed.x, speed.y);

		fireballs.push_back(fireball);
	}

private:
	const float speed = 0.3f;
	const float gravity = 1.6e-3f;
	const float jump_speed = 0.1f;
	Animation walk_left;
	Animation walk_right;

	Vector2 position;//角色位置
	Vector2 velocity;//角色速度
	Vector2 size;//角色尺寸

	bool is_left = false;
	bool is_right = false;
	bool is_jump = false;
	bool is_face_right = true;
	bool is_dead = false;
	bool can_jump = true;
	bool jump_bgm = true;

	int attack_cd = 500;
	bool can_attack = true;
	Timer timer_attack_cd;
};