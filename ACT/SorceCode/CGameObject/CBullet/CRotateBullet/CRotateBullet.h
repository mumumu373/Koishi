#pragma once
#include "CGameObject/CBullet/CBullet.h"	//バレットクラス継承
#include "Global.h"

/******************************************************************************************
*		自分の周りをまわるバレットクラス
*/
class CRotateBullet
	:public CBullet
{
public:
	//			どの陣営のバレットか	ポジション	X方向の速さ		Y方向の速さ	X方向の中心からの位置	Y方向の中心からの位置	開始角度
	CRotateBullet(int Camp, VECTOR2_f Pos, double X_Speed, double Y_Speed, double X_Range, double Y_Range, double StartAngle);
	~CRotateBullet();

	void Update() override;
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void CharacterHit() override;		//ヒットした時のバレットの処理
private:
	void Animation() override;
private:
	VECTOR2_f m_MaxSpeed;	//最大の速度を記憶しておく

	VECTOR2_f m_MasterPosition;	//呼び出されたときの位置を記憶しておく

	VECTOR2_f m_Angle;		//まわる角度
	VECTOR2_f m_Range;		//キャラからの距離(半径で)
	VECTOR2_f m_MasterRange;//元のキャラからの位置
};