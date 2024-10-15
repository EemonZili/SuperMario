#pragma once
#include <graphics.h>
#include "constants.h"
#include "camera.h"
#include "timer.h"

class Scene
{
public:

	Scene() = default;
	~Scene() = default;

	virtual void on_enter() {} //��ʼ����������ʼ��˽�б���������֮�ࣩ
	virtual void on_input(const ExMessage& msg) {} //��������
	virtual void on_update(int delta, Camera& camera) {} //����������³���
	virtual void on_draw(const Camera& camera) {} //���Ƴ���
	virtual void on_exit() {} //�˳��������ر����֣�
};