#pragma once
#include "CGameObject/CBullet/CBullet.h"	//バレットクラス継承
#include "Global.h"

/******************************************************************************************
*		360°バレットを飛ばすクラス
*/
class CCircularBullet
	:public CBullet
{
public:
					//陣営			位置			属性		スピード		数			初期角度	サイズ		消える時間			加速度
	CCircularBullet(int Camp, VECTOR2_f Pos, int Color, double Speed, double Amount, double StartAngle, int Size, int ReleaseTime, double SpeedAcc);
	~CCircularBullet();

	void Update() override;
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void CharacterHit() override;		//ヒットした時のバレットの処理
private:
	void Animation() override;
private:
	VECTOR2_f m_MasterPosition;	//呼び出されたときの位置を記憶しておく

	VECTOR2_f m_Vector;		//打ち出すベクトル

	double m_SpeedAcc;		//加速度を設定

	double m_Amount;			//発射角度
	double m_StartAngle;		//初期角度
};