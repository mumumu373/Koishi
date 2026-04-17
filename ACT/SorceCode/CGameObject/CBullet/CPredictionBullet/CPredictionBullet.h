#pragma once
#include "CGameObject/CBullet/CBullet.h"	//バレットクラス継承
#include "Global.h"

/******************************************************************************************
*		予測線からバレットを出すクラス
*/
class CPredictionBullet
	:public CBullet
{
public:
	//バレットを打ち出した処理が通ったかを見る
	bool HowStartBulletShot = false;
public:
						//陣営			位置		属性		スピード	初期角度	サイズ		撃つタイミング
	CPredictionBullet(int Camp, VECTOR2_f Pos, int Color, double Speed, double StartAngle, int Size, int ShotTime);
	~CPredictionBullet();

	void Update() override;
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void CharacterHit() override;		//ヒットした時のバレットの処理
private:
	void Animation() override;
	//バレットを撃つときの初期設定
	void StartBulletShot();
private:
	VECTOR2_f m_MasterPosition;	//呼び出されたときの位置を記憶しておく

	VECTOR2_f m_Vector;			//打ち出すベクトル

	double m_StartAngle;		//初期角度

	int m_BulletShotCo;			//バレットを打ち出すタイミングを計測
	int m_BulletShotTime;		//バレットを打ち出すタイミング

	int m_WarningLineCo;		//警告線の出た回数をカウント
	bool m_WarningTime;			//現在警告線が出ているか

	VECTOR2_f m_CameraPos;		//カメラのポジションを決める
};