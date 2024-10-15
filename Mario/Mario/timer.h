#pragma once

#include <functional>

class Timer
{
public:
	Timer() = default;
	~Timer() = default;

	void on_update(int delta)
	{
		if (paused) return;

		pass_time += delta;
		if (pass_time >= wait_time)
		{
			if ((!one_shot || (one_shot && !shotted)) && callback)
			{
				callback();
			}
			shotted = true;
			pass_time = 0;
		}
	}


	void reset()
	{
		pass_time = 0;
		shotted = false;
	}

	void set_wait_time(int time)
	{
		wait_time = time;
	}

	void set_one_shot(bool one_shot)
	{
		this->one_shot = one_shot;
	}

	void set_callback(std::function<void()> callback)
	{
		this->callback = callback;
	}

	void pause()
	{
		paused = true;
	}

	void resume()
	{
		paused = false;
	}

private:
	int pass_time = 0; //过去的时间
	int wait_time = 0; //等待时间（设定的时间）
	bool paused = false; //是否暂停
	bool shotted = false; //是否已经触发过
	bool one_shot = false; //是否只触发一次
	std::function<void()> callback; //回调函数
};