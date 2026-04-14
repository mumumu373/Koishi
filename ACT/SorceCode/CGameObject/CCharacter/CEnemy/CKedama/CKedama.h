#pragma once
#include "CGameObject/CCharacter/CEnemy/CEnemy.h"		//継承
#include "CGameObject/CStage/CStageCollision/CStageCollision.h"	//ステージ当たり判定クラス

/***********************************************************************
*	エネミー毛玉クラス
*/
class CKedama
	: public CEnemy
{
public:
	//最大落下速度
	static constexpr double MAX_FALLING_SPEED = 25;
public:
	//呼び出すときに 種類		位置		スピード			ジャンプ力		ムーブを変えるタイミング	ジャンプするタイミング
	CKedama(int Kinds, VECTOR2_f SetPos, double MoveSpeed, double JumpPower, int ChangeMoveTime, int JumpingTime);
	~CKedama();

	void StartSetting() override;
	void Update() override {};
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override;

private:
	void Animation() override;

	//ステージとの当たり判定	
	void StageCollision(double OffsetPos_X, double OffsetPos_Y) override;
private:
	int m_ChangeMoveTime;	//ムーブを変えるタイミング
	int m_ChangeMoveCo;		//ムーブを変えるタイミングをカウント

	double m_JumpPower;		//ジャンプ力
	bool m_Jumping;			//今ジャンプしているか
	int m_JumpingTime;		//ジャンプするタイミング
	int m_JumpingCo;		//ジャンプするタイミングをカウント

	double m_FallingSpeed;	//落下速度
	bool m_GroundStand;		//地面に立っているか			
};