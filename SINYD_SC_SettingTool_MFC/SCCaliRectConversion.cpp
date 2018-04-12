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
	maxH = 150;//����������ʾ����ͼ ����ͼ���߶��ݶ�λ150
	float hCoef = (float)maxH / (float)height;
	float wCoef = (float)maxW / (float)width;

	//����10֮�����5
	hCoef = hCoef * 10 / 5;
	wCoef = wCoef * 10 / 5;
	//�������뱣������λ��
	int ihCoef = floor(hCoef * 10 + 0.5) / 10;
	int iwCoef = floor(wCoef * 10 + 0.5) / 10;
	//�ٴγ���5������ʼֵ��10��ֵ��Լ��5��֮�����10���õ����ս��Ϊ0.5�ı���
	hCoef = ihCoef * 5.0 / 10.0;
	wCoef = iwCoef * 5.0 / 10.0;
	//���շ��ر���С���Ǹ�ϵ��
	return hCoef >= wCoef ? wCoef : hCoef;
}

RECT SCCaliRectConversion::MakeRect(CPoint POrigin, CPoint PEnd)
{
	RECT rt;
	if (POrigin.x <= PEnd.x && POrigin.y <= PEnd.y)
	{
		//��������¿�ѡ
		rt.left = POrigin.x;
		rt.top = POrigin.y;
		rt.right = PEnd.x;
		rt.bottom = PEnd.y;
	}
	else if (POrigin.x > PEnd.x && POrigin.y > PEnd.y)
	{
		//��������Ͽ�ѡ
		rt.left = PEnd.x;
		rt.top = PEnd.y;
		rt.right = POrigin.x;
		rt.bottom = POrigin.y;
	}
	else if (POrigin.x <= PEnd.x && POrigin.y > PEnd.y)
	{
		//��������Ͽ�ѡ
		rt.left = POrigin.x;
		rt.top = PEnd.y;
		rt.right = PEnd.x;
		rt.bottom = POrigin.y;
	}
	else
	{
		//��������¿�ѡ
		rt.left = PEnd.x;
		rt.top = POrigin.y;
		rt.right = POrigin.x;
		rt.bottom = PEnd.y;
	}
	return rt;
}
