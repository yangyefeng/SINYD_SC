#pragma once
class SCCaliRectConversion
{
public:
	SCCaliRectConversion();
	~SCCaliRectConversion();

	static const int areaPicSide = 250;

	float CalcAreaCoef(int maxH, int maxW, int height, int width);
	RECT MakeRect(CPoint POrigin, CPoint PEnd);
};

