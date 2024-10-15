#pragma once

#include "scene.h"
#include "main_menu.h"
#include "game_scene.h"
#include "camera.h"

extern Scene* main_menu;
extern Scene* game_scene;

class Scene_manager
{
public:
	enum class Scene_type //·½±ã³¡¾°ÇÐ»»
	{
		Menu,
		Game
	};

public:
	Scene_manager() = default;
	~Scene_manager() = default;

	void set_scene(Scene* scene) 
	{
		current_scene = scene;
		current_scene->on_enter();
	}

	void switch_scene(Scene_type type)
	{
		current_scene->on_exit();
		switch (type)
		{
		case Scene_type::Menu:
			current_scene = main_menu;
			break;
		case Scene_type::Game:
			current_scene = game_scene;
			break;
		default:
			break;
		}
		current_scene->on_enter();
	}

	void on_input(const ExMessage& msg)
	{
		current_scene->on_input(msg);
	}

	void on_update(int delta, Camera& camera)
	{
		current_scene->on_update(delta, camera);
	}

	void on_draw(const Camera& camera)
	{
		current_scene->on_draw(camera);
	}

private:
	Scene* current_scene = nullptr;
};