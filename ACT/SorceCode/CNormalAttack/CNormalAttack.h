#pragma once
#include "Global.h"
#include <CCamera/CCamera.h>


class CNormalAttack
{
	static constexpr int ColionMAX = 2;
	static constexpr int imageW = 64;
	static constexpr int imageH = 32;
	static constexpr int Time = 6;//継続時間
	static constexpr int AtakuSize = 120;
public:
	
	CNormalAttack();
	~CNormalAttack();
	void Strat(double rag, VECTOR2_f PlayerCenterPos);
	void Update();
	void Draw(std::unique_ptr<CCamera>& pCamera);
	void DrawColion(HDC hdc, HWND hwnd, std::unique_ptr<CCamera>& pCamera);
	int GetMAX() {
		return ColionMAX;
	}
ObjectInfo GetColion(int No);
	int GetAttack() {
		return Attacked
			;
	}
private:
ObjectInfo Colion[ColionMAX];
	VECTOR2_f IMagePos;
	int Imagerag;
	int Antimension;
	bool Attacked;
};

