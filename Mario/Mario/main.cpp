#include <graphics.h>//easyXͼ�ο�

#include "constants.h"//���泣��

#include "camera.h"//�������,���ڸ��ٽ�ɫλ��

#include "atlas.h"//ͼ���࣬���ڼ��ض��Ž�ɫͼƬ

#include "util.h"//�����࣬���ڷ�תͼƬͼ��

#include "platform.h"//ƽ̨�࣬���ڻ���ƽ̨
#include "wall.h"//ǽ�࣬Ϊ�˼򻯴��룬�ܵ�Ҳ����ǽ

#include "scene.h"//������
#include "main_menu.h"//���˵���
#include "game_scene.h"//��Ϸ������
#include "game_over.h" //��Ϸ����������
#include "scene_manager.h"//����������

#include "monster.h"

#pragma comment(lib, "Winmm.lib")//������������Ҫ�Ŀ�

IMAGE bg;//���˵�����ͼ
IMAGE title;//���˵�����

IMAGE game_over; //��Ϸ����ͼ

IMAGE mario_child_stand_left;//��·С��վ��ͼ
IMAGE mario_child_turn_left;//�����С��ת��ͼ
IMAGE mario_child_jum_left;//�����С����Ծͼ
Atlas mario_child_walk_left;//�����С����·����
IMAGE mario_child_stand_right;//��·С��վ��ͼ
IMAGE mario_child_turn_right;//�����С��ת��ͼ
IMAGE mario_child_jum_right;//�����С����Ծͼ
Atlas mario_child_walk_right;//�����С����·����
IMAGE mario_child_dead;//�����С������ͼ

Atlas fireball;//���򶯻�

IMAGE chestnut_dead;//���ӹ�����ͼ
Atlas chestnut;//���ӹֶ���

Scene_manager* scene_manager;//����������
Scene* main_menu;//���˵�����
Scene* game_scene;//��Ϸ����
Scene* game_over_scene;//��Ϸ��������

Camera main_camera;//�������

std::vector<Platform> platforms;//��������
std::vector<Wall> walls;//ǽ����
std::vector<Fireball*> fireballs; //�ӵ����飬����Ϊȫ�ֱ�����������ײ���
std::vector<Monster> chestnuts;//���ӹ�����

bool is_debug = false;//���ڿ��ӻ������ײ����

//int dead_count = 0;//��������

void load_res();//������Դ

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
	AddFontResourceEx(_T("res/IPix.ttf"), FR_PRIVATE, NULL);//������������

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