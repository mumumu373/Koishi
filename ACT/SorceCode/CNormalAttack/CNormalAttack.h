#pragma once
#include "Global.h"
#include <CCamera/CCamera.h>


class CNormalAttack
{
	static constexpr int CollisionMAX = 2;
	static constexpr int imageW = 64;
	static constexpr int imageH = 32;
	static constexpr int Time = 4;//継続時間
	static constexpr int AtakuSize = 120;

public:
	int m_PlayerColor = 0;	//プレイヤーの属性をセット
public:
	
	CNormalAttack();
	~CNormalAttack();
	void Strat(double rag, VECTOR2_f PlayerCenterPos);
	void Update();
	void Draw(std::unique_ptr<CCamera>& pCamera);
	void DrawColion(HDC hdc, HWND hwnd, std::unique_ptr<CCamera>& pCamera);

	ObjectInfo GetColion(int No) { 
		return Collision[No];
	}
	int GetMAX() {
		return CollisionMAX;
	}
	int GetAttack() {
		return Attacked;
	}

	//プレイヤーの属性をセット
	void SetPlayerColor(int PlayerColor) { m_PlayerColor = PlayerColor; }
private:
	ObjectInfo Collision[CollisionMAX];
	VECTOR2_f IMagePos;
	int Imagerag;
	int Antimension;
	bool Attacked;
};

