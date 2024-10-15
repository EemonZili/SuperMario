#pragma once

#include <graphics.h>

#include "atlas.h"
#include "camera.h"

#pragma comment(lib, "MSIMG32.LIB") //加载AlphaBlend函数所需的库

inline void rectangle(const Camera& camera, int left, int top, int right, int bottom) //绘制矩形
{
	const Vector2& pos = camera.get_position();
	rectangle((int)(left - pos.x), top, (int)(right - pos.x), bottom);
}

inline void putimage_alpha(const Camera& camera, int dst_x, int dst_y, IMAGE* image) //根据相机位置绘制带透明度的图片
{
	int w = image->getwidth();
	int h = image->getheight();
	const Vector2& pos = camera.get_position();
	if (dst_x - pos.x < 0 || dst_x - pos.x > WIDTH || dst_y - pos.y < 0 || dst_y - pos.y > HEIGHT) return;
	AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(dst_x - pos.x), (int)(dst_y - pos.y), w, h, GetImageHDC(image), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}


inline void putimage_alpha(int dst_x, int dst_y, IMAGE* image) //绘制带透明度的图片
{
	int w = image->getwidth();
	int h = image->getheight();
	AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h, GetImageHDC(image), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

inline void putimage(const Camera& camera, int dst_x, int dst_y, IMAGE* image) //根据相机位置绘制带透明度的图片
{
	const Vector2& pos = camera.get_position();
	putimage((int)(dst_x - pos.x), (int)(dst_y - pos.y), image);
}

inline void flip_image(IMAGE* src, IMAGE* dst) //水平翻转图片
{
	int w = src->getwidth();
	int h = src->getheight();
	Resize(dst, w, h);
	DWORD* src_buf = GetImageBuffer(src);
	DWORD* dst_buf = GetImageBuffer(dst);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int idx_src = y * w + x;
			int idx_dst = y * w + (w - 1 - x);
			dst_buf[idx_dst] = src_buf[idx_src];
		}
	}
}

inline void flip_atlas(Atlas& src, Atlas& dst) //水平翻转图集
{
	dst.clear();
	for (int i = 0; i < src.get_size(); i++)
	{
		IMAGE img_flpipped;
		flip_image(src.get_image(i), &img_flpipped);
		dst.add_image(img_flpipped);
	}
}

