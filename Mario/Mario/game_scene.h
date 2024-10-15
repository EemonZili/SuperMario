#pragma once

#include "scene.h"
#include "scene_manager.h"

#include "atlas.h"
#include "animation.h"

#include "player.h"
#include "mario_child.h"

#include "platform.h"

#include "monster.h"

extern IMAGE bg;
extern IMAGE mario_child_stand;
extern IMAGE mario_child_turn;
extern IMAGE mario_child_jum;
extern IMAGE mario_child_dead;
extern Atlas mario_child_walk;
extern Atlas chestnut;

extern std::vector<Platform> platforms;
extern std::vector<Wall> walls;
extern std::vector<Fireball*> fireballs;
extern std::vector<Monster> chestnuts;

class Game_scene : public Scene
{
public:
	void on_enter() override
	{
		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);

		platforms.resize(4);
		platforms[0].set(0, 2800, 525);
		platforms[1].set(2910, 3450, 525);
		platforms[2].set(3590, 6150, 525);
		platforms[3].set(6250, 9532, 525);

		walls.resize(12);
		walls[0].set(445, 525, 1130);
		walls[1].set(445, 525, 1200);
		walls[2].set(405, 525, 1535);
		walls[3].set(405, 525, 1605);
		walls[4].set(365, 525, 1857);
		walls[5].set(365, 525, 1927);
		walls[6].set(365, 525, 2300);
		walls[7].set(365, 525, 2370);
		walls[8].set(445, 525, 6565);
		walls[9].set(445, 525, 6635);
		walls[10].set(445, 525, 7210);
		walls[11].set(445, 525, 7280);

		chestnuts.resize(1);
		chestnuts[0].set_atlas(&chestnut);
		chestnuts[0].set_x(1320);
		chestnuts[0].set_size(Vector2(40, 40));
		chestnuts[0].set_speed(Vector2(0.5f, 0));
	}

	void on_input(const ExMessage& msg) override
	{
		switch (msg.message)
		{
		case WM_KEYUP:
			if (msg.vkcode == 0x51)
				is_debug = !is_debug;
			break;
		default:
			break;
		}
		player1.on_input(msg);
	}

	void on_update(int delta, Camera& camera) override
	{
		player1.on_update(delta, camera);

		//fireballs.erase(std::remove_if(
		//	fireballs.begin(), fireballs.end(),
		//	[](Fireball* fireball)
		//	{
		//		bool deletable = fireball->check_delete();
		//		if (deletable) delete fireball;
		//		return deletable;
		//	}), fireballs.end());

		for (Fireball* fireball : fireballs)
		{
			fireball->on_update(delta);
		}

		for (Monster& chestnut : chestnuts)
		{
			chestnut.on_update(delta);
		}
	}

	void on_draw(const Camera& camera) override
	{
		putimage(camera, 0, 0, &bg);

		player1.on_draw(camera);

		for (Fireball* fireball : fireballs)
		{
			fireball->on_draw(camera);
		}

		for (Monster& chestnut : chestnuts)
		{
			chestnut.on_draw(camera);
		}

		if (is_debug)
		{
			settextcolor(RGB(255, 0, 0));
			outtextxy(15, 15, _T("Debug Mode,°´ 'Q' ¹Ø±Õ"));
			for (const Platform& platform : platforms)
			{
				platform.on_draw(camera);
			}
			for (const Wall& wall : walls)
			{
				wall.on_draw(camera);
			}
		}
	}

	void on_exit() override
	{
		mciSendString(_T("close bgm_game"), NULL, 0, NULL);
	}

private:
	Mario_child player1;
};