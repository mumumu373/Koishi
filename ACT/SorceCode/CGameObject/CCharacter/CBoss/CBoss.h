#pragma once
#include "CGameObject/CCharacter/CCharacter.h"	//キャラクタークラス
#include "CGame/Factory/CBulletFactory/CBulletFactory.h"//バレットを作るクラス

/*************************************************************************************************
*		ボスクラス(親)
*/
class CBoss
	:public CCharacter
{
public:
	//攻撃のパターン
	enum enAttackMove
	{
		Standby,	//待機
		Move_01,	//行動1
		Move_02,	//行動2
	};

	int m_AttackMove = enAttackMove::Standby;

	//ナズーリンのフェーズ
	enum enBossPhase
	{
		Phase_1,	//フェーズ1
		Phase_2,	//フェーズ2
	};

	int m_BossPhase = enBossPhase::Phase_1;
public:
	bool NoHit = false;		//問答無用で無敵状態にする

	int NoHitAttackCo = 0;	//攻撃があたらない時間をカウント
	const int NoHitAttackTime = 40;	//攻撃があたらない時間
public:
	CBoss() {};
	virtual ~CBoss() {};

	virtual void StartSetting() override {};
	virtual void Update() override {};
	virtual void Draw(std::unique_ptr<CCamera>& pCamera) override {};

	virtual void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override {};

	//プレイヤーの攻撃がヒットした時の動作
	virtual void PlayerAttackHit(int Damage) = 0;

	//投げられたエネミーにヒットした時の動作
	virtual void ThrowEnemyHit(int Damage) = 0;

	//ボスとの戦闘フラグを踏んだ
	virtual void BossBattleFlag(VECTOR2_f PlayerPos) = 0;

	//会話などのムービーシーンの時に動作する関数	現在何の会話をしているかを取得する
	virtual void MovieSceneUpdate() = 0;

	//プレイヤーの場所を見ておく
	void SetPlayerPos(VECTOR2_f PlayerPos) { m_PlayerPos = PlayerPos; }

	//カメラの場所をセット
	void SetCameraPos(VECTOR2_f CameraPos) { m_CameraPos = CameraPos; }

	//ボス攻略アイテムをゲットしたか
	virtual bool GetBossClearFlag() = 0;
protected:
	void Animation() override {};

	//ステージとの当たり判定
	void StageCollision(double OffsetPos_X, double OffsetPos_Y) override{};
protected:
	VECTOR2_f m_PlayerPos = { 0,0 };		//プレイヤーの位置	

	VECTOR2_f m_CameraPos = { 0,0 };		//カメラの位置

	int m_DeadAnimCo = 0;					//死んだ時のアニメーションのカウント
};