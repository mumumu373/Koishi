#pragma once
#include "CGameObject/CCharacter/CEnemy/CEnemy.h"		//継承

/***********************************************************************
*	エネミー妖精クラス
*/
class CFairy
	: public CEnemy
{
public:
	//敵の動作タイプ
	enum enMoveType
	{
		Stop = 0,		//停止
		Rotation,		//回転
		MovingAround,	//移動関連の動作
		ChargengTarget,	//プレイヤーに向かって突進する
	};

	int m_MoveType = enMoveType::Stop;
public:
	//呼び出すときに種類を決める
	CFairy(int Kinds, VECTOR2_f SetPos, VECTOR2_f Speed, int MoveType, int MoveOp_1, int MoveOp_2);
	~CFairy();

	void StartSetting() override;
	void Update() override {};
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override;

	//プレイヤーの攻撃がヒットした時の動作
	void PlayerAttackHit(int Damage) override;
private:
	void Animation() override;

	//ステージとの当たり判定	妖精には必要なし
	void StageCollision(double OffsetPos_X, double OffsetPos_Y) override {};

	//ムーブの動作関数
	void MoveControl();
private:
	bool m_BulletShot;		//バレットを撃ったか
	int m_BulletShotTiming;	//バレットを撃ちだすタイミング	
	int m_BulletShotCo;		//バレットを撃つタイミングをカウント
	int m_ShotReleaseTime;	//バレットの持続時間

	int m_MoveTypeOption_1;	//ムーブタイプそれぞれの設定項目
	int m_MoveTypeOption_2;	//2つ目の設定項目

	int m_MoveCo;			//ムーブの動作切り替えのカウント

	VECTOR2_f m_MovingSpeed;	//ムーブのスピードを参照する変数

	VECTOR2_f m_Vector;			//ベクトルを算出する

	VECTOR2_f m_MasterPosition;	//呼び出された時の位置
};