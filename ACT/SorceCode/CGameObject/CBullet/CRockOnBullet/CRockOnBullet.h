#pragma once
#include "CGameObject/CBullet/CBullet.h"	//バレットクラス継承
#include "Global.h"

/*******************************************************************************************
*		狙い撃ちするバレット
*/
class CRockOnBullet
	:public CBullet
{
public:				//陣営		位置		属性			速度			対象の位置		大きさ		消える時間
	CRockOnBullet(int Camp, VECTOR2_f Pos, int Color, double Speed, VECTOR2_f TargetPos, int Size, int ReleaseTime);
	~CRockOnBullet();

	void Update() override;
	void Draw(std::unique_ptr<CCamera>& pCamera) override;
private:
	void Animation() override;
private:
	VECTOR2_f m_Vector;		//打ち出すベクトル
};