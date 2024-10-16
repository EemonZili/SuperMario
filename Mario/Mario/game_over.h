#pragma once

#include "scene.h"
#include "scene_manager.h"

extern Scene_manager* scene_manager;

class Game_over : public Scene
{
public:
	Game_over() = default;
	~Game_over() = default;

	void on_enter() override
	{
		mciSendString(_T("play game_over from 0"), NULL, 0, NULL);
		game_over_timer.set_wait_time(6000);
		game_over_timer.set_one_shot(false);
		game_over_timer.set_callback([&]()
			{
				scene_manager->switch_scene(Scene_manager::Scene_type::Menu);
			});
	}

	void on_input(const ExMessage& msg) override
	{
	}

	void on_update(int delta, Camera& camera)
	{
		game_over_timer.on_update(delta);

	}
	void on_draw(const Camera& camera)
	{
	}

	void exit()
	{
	}

private:
	Timer game_over_timer;
};