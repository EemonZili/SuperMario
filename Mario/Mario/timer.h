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
	int pass_time = 0; //��ȥ��ʱ��
	int wait_time = 0; //�ȴ�ʱ�䣨�趨��ʱ�䣩
	bool paused = false; //�Ƿ���ͣ
	bool shotted = false; //�Ƿ��Ѿ�������
	bool one_shot = false; //�Ƿ�ֻ����һ��
	std::function<void()> callback; //�ص�����
};