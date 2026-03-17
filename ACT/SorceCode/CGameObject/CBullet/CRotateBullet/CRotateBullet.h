#pragma once
#include "CGameObject/CBullet/CBullet.h"	//バレットクラス継承

/******************************************************************************************
*		自分の周りをまわるバレットクラス
*/
class CRotateBullet
	:public CBullet
{
public:
	//			どの陣営のバレットか	ポジション	X方向の速さ		Y方向の速さ
	CRotateBullet(int Camp, VECTOR2_f Pos, double X_Speed, double Y_Speed);
	~CRotateBullet();

	void Update() override;
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void CharacterHit() override;		//ヒットした時のバレットの処理
private:
	void Animation() override;
private:
	VECTOR2_f m_MaxSpeed;	//最大の速度を記憶しておく
};