#pragma once

#include "scene.h"
#include "scene_manager.h"

extern IMAGE bg;
extern IMAGE title;

extern Scene_manager* scene_manager;

class Main_menu : public Scene
{
public:
	Main_menu() = default;
	~Main_menu() = default;

	void on_enter() override
	{
	}

	void on_input(const ExMessage& msg) override
	{
		if (msg.message == WM_KEYUP)
		{
			scene_manager->switch_scene(Scene_manager::Scene_type::Game);
		}
	}

	void on_update(int delta, Camera& camera) override
	{

	}

	void on_draw(const Camera& camera) override
	{
		putimage(0, 0, &bg);
		putimage(WIDTH / 2 - title.getwidth() / 2, HEIGHT / 3 - title.getheight() / 2, &title);

		settextstyle(0, 0, _T("IPix"));
		setbkmode(TRANSPARENT);
		outtextxy(WIDTH / 2 - 137, HEIGHT / 2 + 100, _T("Press any key to start"));
	}
	
	void on_exit() override
	{
	}

};