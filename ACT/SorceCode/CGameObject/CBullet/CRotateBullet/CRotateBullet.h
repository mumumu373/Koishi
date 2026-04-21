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
	//			どの陣営のバレットか	ポジション	属性	速さ		X方向の中心からの位置	Y方向の中心からの位置	開始角度		サイズ		消える時間	ナズのバレットを撃つか
	CRotateBullet(int Camp, VECTOR2_f Pos, int Color, double Speed, double X_Range, double Y_Range, double StartAngle, int Size, int ReleaseTime, bool NazrinBullet);
	~CRotateBullet();

	void Update() override;
	void Draw(std::unique_ptr<CCamera>& pCamera) override;
private:
	void Animation() override;
private:
	VECTOR2_f m_MaxSpeed;	//最大の速度を記憶しておく

	VECTOR2_f m_MasterPosition;	//呼び出されたときの位置を記憶しておく

	VECTOR2_f m_Angle;		//まわる角度
	VECTOR2_f m_Range;		//キャラからの距離(半径で)
	VECTOR2_f m_MasterRange;//元のキャラからの位置
};