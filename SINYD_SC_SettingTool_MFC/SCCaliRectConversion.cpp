#include "stdafx.h"
#include "SCCaliRectConversion.h"


SCCaliRectConversion::SCCaliRectConversion()
{
}

SCCaliRectConversion::~SCCaliRectConversion()
{
}

float SCCaliRectConversion::CalcAreaCoef(int maxH, int maxW, int height, int width)
{
	maxH = 150;//由于上下显示两张图 单张图最大高度暂定位150
	float hCoef = (float)maxH / (float)height;
	float wCoef = (float)maxW / (float)width;

	//乘以10之后除以5
	hCoef = hCoef * 10 / 5;
	wCoef = wCoef * 10 / 5;
	//四舍五入保留到个位数
	int ihCoef = floor(hCoef * 10 + 0.5) / 10;
	int iwCoef = floor(wCoef * 10 + 0.5) / 10;
	//再次乘以5，将初始值的10倍值修约到5，之后除以10，得到最终结果为0.5的倍数
	hCoef = ihCoef * 5.0 / 10.0;
	wCoef = iwCoef * 5.0 / 10.0;
	//最终返回倍数小的那个系数
	return hCoef >= wCoef ? wCoef : hCoef;
}

RECT SCCaliRectConversion::MakeRect(CPoint POrigin, CPoint PEnd)
{
	RECT rt;
	if (POrigin.x <= PEnd.x && POrigin.y <= PEnd.y)
	{
		//鼠标向右下框选
		rt.left = POrigin.x;
		rt.top = POrigin.y;
		rt.right = PEnd.x;
		rt.bottom = PEnd.y;
	}
	else if (POrigin.x > PEnd.x && POrigin.y > PEnd.y)
	{
		//鼠标向左上框选
		rt.left = PEnd.x;
		rt.top = PEnd.y;
		rt.right = POrigin.x;
		rt.bottom = POrigin.y;
	}
	else if (POrigin.x <= PEnd.x && POrigin.y > PEnd.y)
	{
		//鼠标向右上框选
		rt.left = POrigin.x;
		rt.top = PEnd.y;
		rt.right = PEnd.x;
		rt.bottom = POrigin.y;
	}
	else
	{
		//鼠标向左下框选
		rt.left = PEnd.x;
		rt.top = POrigin.y;
		rt.right = POrigin.x;
		rt.bottom = PEnd.y;
	}
	return rt;
}
