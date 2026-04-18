#pragma once

//ベクトル構造体
struct VECTOR2
{
	int x;
	int y;

	VECTOR2(int xSrc,int ySrc)
		:x(xSrc),y(ySrc)			//初期化
	{
	}
	VECTOR2()
		:VECTOR2(0, 0)
	{
	}
};

//ベクトルフロート構造体
struct VECTOR2_f
{
	double x;
	double y;

	VECTOR2_f(double xSrc, double ySrc)
		:x(xSrc),y(ySrc)	//初期化
	{
	}
	VECTOR2_f()
		:VECTOR2_f(0.0,0.0)
	{
	}
};

//ベクトル3方向構造体
struct VECTOR3
{
	double x;
	double y;
	double z;

	VECTOR3(double xSrc, double ySrc, double zSrc)
		:x(xSrc), y(ySrc), z(zSrc)
	{
	}
	VECTOR3()
		:VECTOR3(0, 0, 0)
	{
	}
};