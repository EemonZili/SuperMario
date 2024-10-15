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

	virtual void on_enter() {} //初始化场景（初始化私有变量，音乐之类）
	virtual void on_input(const ExMessage& msg) {} //控制输入
	virtual void on_update(int delta, Camera& camera) {} //根据输入更新场景
	virtual void on_draw(const Camera& camera) {} //绘制场景
	virtual void on_exit() {} //退出场景（关闭音乐）
};