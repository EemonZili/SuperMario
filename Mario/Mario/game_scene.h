#pragma once

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
extern Atlas chestnut_fly;

extern std::vector<Platform> platforms;
extern std::vector<Wall> walls;
extern std::vector<Fireball*> fireballs;
extern std::vector<Monster> chestnuts;

extern Scene_manager* scene_manager;
extern Camera main_camera;//主摄像机

class Game_scene : public Scene
{
public:
	void on_enter() override
	{
		mciSendString(_T("open res/audio/ground.mp3 alias bgm_game"), NULL, 0, NULL);
		mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);

		player1 = new Mario_child();

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
		chestnuts[0].set_fly_atlas(&chestnut_fly);
		chestnuts[0].set_x(1320);
		chestnuts[0].set_size(Vector2(40, 40));
		chestnuts[0].set_speed(Vector2(0.5f, 0));

		dead_over_timer.set_wait_time(3000);
		dead_over_timer.set_one_shot(false);
		dead_over_timer.set_callback([&]()
			{
				dead_count++;
				bgm_end = true;
				scene_manager->switch_scene(Scene_manager::Scene_type::Game);
			});

		game_over_timer.set_wait_time(6000);
		game_over_timer.set_one_shot(true);
		game_over_timer.set_callback([&]()
			{
			});
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
		player1->on_input(msg);
	}

	void on_update(int delta, Camera& camera) override
	{
		player1->on_update(delta, camera);

		fireballs.erase(std::remove_if(
			fireballs.begin(), fireballs.end(),
			[](Fireball* fireball)
			{
				bool deletable = fireball->check_delete();
				if (deletable) delete fireball;
				return deletable;
			}), fireballs.end());

		//如果monster类有非静态const，这段代码会报错
		for (auto it = chestnuts.begin(); it != chestnuts.end(); ) {
			if (it->check_dead()) { 
				it = chestnuts.erase(it);
			}
			else {
				++it;
			}
		}


		for (Fireball* fireball : fireballs)
		{
			fireball->on_update(delta);
		}

		for (Monster& chestnut : chestnuts)
		{
			chestnut.on_update(delta);
		}

		if (player1->get_dead_status())
		{
			if (bgm_end)
			{
				mciSendString(_T("close bgm_game"), NULL, 0, NULL);
				mciSendString(_T("play death from 0"), NULL, 0, NULL);
				bgm_end = false;
			}
			dead_over_timer.on_update(delta);
			if (dead_count == player1->get_life())
			{
				dead_count = 0;
				mciSendString(_T("close bgm_game"), NULL, 0, NULL);
				scene_manager->switch_scene(Scene_manager::Scene_type::Over);
			}
		}
	}

	void on_draw(const Camera& camera) override
	{
		putimage(camera, 0, 0, &bg);

		player1->on_draw(camera);

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
			outtextxy(15, 15, _T("Debug Mode,按 'Q' 关闭"));
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
		main_camera.reset();
		walls.clear();
		platforms.clear();
		chestnuts.clear();
		fireballs.clear();
		delete player1;
		player1 = nullptr;
		dead_over_timer.reset();
		game_over_timer.reset();
	}

private:
	Mario_child* player1;
	Timer dead_over_timer;
	Timer game_over_timer;
	bool bgm_end = true;
	bool game_over = false;
	int dead_count = 0;
};