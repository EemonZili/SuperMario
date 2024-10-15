#pragma once

#include <graphics.h>
#include <vector>

class Atlas
{
public:
	Atlas() = default;
	~Atlas() = default;

	//加载图集文件，begin参数代表开始的图片序号
	void load_from_file(LPCTSTR path_template, int num, int begin)
	{
		img_list.clear();
		img_list.resize(num);

		TCHAR path_file[256];
		for (int i = 0; i < num; i++)
		{
			_stprintf_s(path_file, path_template, i + begin);
			loadimage(&img_list[i], path_file);
		}
	}

	void clear()
	{
		img_list.clear();
	}

	int get_size()
	{
		return img_list.size();
	}

	IMAGE* get_image(int index)
	{
		return &img_list[index];
	}

	void add_image(IMAGE img)
	{
		img_list.push_back(img);
	}

private:
	std::vector<IMAGE> img_list;
};