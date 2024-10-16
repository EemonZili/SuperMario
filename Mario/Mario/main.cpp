#include <graphics.h>//easyX图形库

#include "constants.h"//保存常量

#include "camera.h"//摄像机类,用于跟踪角色位置

#include "atlas.h"//图集类，用于加载多张角色图片

#include "util.h"//工具类，用于翻转图片图集

#include "platform.h"//平台类，用于绘制平台
#include "wall.h"//墙类，为了简化代码，管道也属于墙

#include "scene.h"//场景类
#include "main_menu.h"//主菜单类
#include "game_scene.h"//游戏场景类
#include "game_over.h" //游戏结束场景类
#include "scene_manager.h"//场景管理类

#include "monster.h"

#pragma comment(lib, "Winmm.lib")//加载音乐所需要的库

IMAGE bg;//主菜单背景图
IMAGE title;//主菜单标题

IMAGE game_over; //游戏结束图

IMAGE mario_child_stand_left;//马路小孩站立图
IMAGE mario_child_turn_left;//马里奥小孩转身图
IMAGE mario_child_jum_left;//马里奥小孩跳跃图
Atlas mario_child_walk_left;//马里奥小孩走路动画
IMAGE mario_child_stand_right;//马路小孩站立图
IMAGE mario_child_turn_right;//马里奥小孩转身图
IMAGE mario_child_jum_right;//马里奥小孩跳跃图
Atlas mario_child_walk_right;//马里奥小孩走路动画
IMAGE mario_child_dead;//马里奥小孩死亡图

Atlas fireball;//火球动画

IMAGE chestnut_dead;//栗子怪死亡图
Atlas chestnut;//栗子怪动画

Scene_manager* scene_manager;//场景管理器
Scene* main_menu;//主菜单场景
Scene* game_scene;//游戏场景
Scene* game_over_scene;//游戏结束场景

Camera main_camera;//主摄像机

std::vector<Platform> platforms;//地面数组
std::vector<Wall> walls;//墙数组
std::vector<Fireball*> fireballs; //子弹数组，声明为全局变量，方便碰撞检测
std::vector<Monster> chestnuts;//栗子怪数组

bool is_debug = false;//用于可视化检查碰撞对象

//int dead_count = 0;//死亡计数

void load_res();//加载资源

int main()
{
	initgraph(WIDTH, HEIGHT);

	load_res();

	ExMessage msg;

	BeginBatchDraw();

	main_menu = new Main_menu();
	game_scene = new Game_scene();
	game_over_scene = new Game_over();
	scene_manager = new Scene_manager();
	scene_manager->set_scene(main_menu);

	while (true)
	{
		DWORD main_last_time = GetTickCount();
		while (peekmessage(&msg))
		{
			scene_manager->on_input(msg);
		}

		static DWORD last_time = GetTickCount();
		DWORD now_time = GetTickCount();
		DWORD delta_time = now_time - last_time;
		scene_manager->on_update(delta_time, main_camera);
		last_time = now_time;

		cleardevice();
		scene_manager->on_draw(main_camera);
		FlushBatchDraw();

		DWORD main_now_time = GetTickCount();
		DWORD main_delta_time = main_now_time - main_last_time;
		if (main_delta_time < 1000 / FPS)
			Sleep(1000 / FPS - main_delta_time);
	}
	EndBatchDraw();

}

void load_res()
{
	AddFontResourceEx(_T("res/IPix.ttf"), FR_PRIVATE, NULL);//加载像素字体

	loadimage(&bg, _T("res/image/main_menu/bg.png"));
	loadimage(&title, _T("res/image/main_menu/title.png"));

	//loadimage(&game_over, _T("res/image/game_over.png"));
	
	loadimage(&mario_child_stand_right, _T("res/image/entity/mario/0/child0.png"));
	loadimage(&mario_child_turn_right, _T("res/image/entity/mario/0/child4.png"));
	loadimage(&mario_child_jum_right, _T("res/image/entity/mario/0/child5.png"));
	loadimage(&mario_child_dead, _T("res/image/entity/mario/0/child6.png"));

	loadimage(&chestnut_dead, _T("res/image/entity/monster/ground/chestnut2.png"));

	flip_image(&mario_child_stand_right, &mario_child_stand_left);
	flip_image(&mario_child_turn_right, &mario_child_turn_left);
	flip_image(&mario_child_jum_right, &mario_child_jum_left);

	mario_child_walk_right.load_from_file(_T("res/image/entity/mario/0/child%i.png"), 3, 1);
	flip_atlas(mario_child_walk_right, mario_child_walk_left);

	chestnut.load_from_file(_T("res/image/entity/monster/ground/chestnut%i.png"), 2, 0);

	fireball.load_from_file(_T("res/image/entity/fireball/fireball%i.png"), 4, 0);

	mciSendString(_T("open res/audio/fireball.mp3 alias fireball"), NULL, 0, NULL);
	mciSendString(_T("open res/audio/small_jump.mp3 alias small_jump"), NULL, 0, NULL);
	mciSendString(_T("open res/audio/death.mp3 alias death"), NULL, 0, NULL);
	mciSendString(_T("open res/audio/game_over.mp3 alias game_over"), NULL, 0, NULL);
}