#pragma once
#include "CGameObject/CCharacter/CCharacter.h"

/************************************************************************************
*		エネミークラス(親)
*/
class CEnemy
	:public CCharacter
{
public:
	//ワイヤーでつかまれた状態を見る
	enum enCatchWire {
		No,
		Catch,
	};

	int m_CatchWire = enCatchWire::No;

	int NoHitAttackCo = 0;	//攻撃があたらない時間をカウント
	const int NoHitAttackTime = 40;	//攻撃があたらない時間

	VECTOR2_f ThrowSpeed = { 30,15 };		//投げられる速度
	bool EnemyThrown = false;		//投げらました！
	const int ThrowDamage = 100;	//投げたエネミーのダメージ

	const int HitEnemyStopTime = 7;//投げられたときにエネミーに当たったときのヒットストップの時間
	bool HitStop = false;			//ヒットストップを発生させるか

	const int ThrowEnemyDeadTime = 50;	//投げられたエネミーが死亡するまでの時間

	int DeadAnimChangeTime = 8;		//死んだときのアニメーションを変更する時間
public:
	CEnemy();
	virtual ~CEnemy() {};	

	virtual void StartSetting() override {};
	virtual void Update() override {};
	virtual void Draw(std::unique_ptr<CCamera>& pCamera) override {};

	//こっちのほうを使う
	virtual void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override {};

	//プレイヤーの攻撃がヒットした時の動作
	virtual void PlayerAttackHit(int Damage, int Color) = 0;

	//プレイヤーに投げられた時の動作
	void ThrowEnemy();

	//投げられたエネミーにヒットした時の処理
	void ThrowEnemyHit(int Damage, VECTOR2_f ThrowEnemyPos);

	//死んだときの処理
	void EnemyIsDead();

	//死んだときのアニメーションを行う処理
	void DeadAnimationDraw(std::unique_ptr<CCamera>& pCamera);

	//投げられたときに別のエネミーに当たったときの処理
	void SetHitStop()
	{
		HitStop = true;
		m_HitStopCo = 0;
	}

	//プレイヤーの位置を取得
	void SetPlayerPos(VECTOR2_f PlayerPos) { m_PlayerPos = PlayerPos; }

	void SetPosition(VECTOR2_f pos) { m_Position = pos; }

	//エネミーを飛ばす準備
	void SetThrowEnemy() { 
		EnemyThrown = true; 

		//右側にプレイヤーがいるなら
		if (m_PlayerPos.x >= GetCenterPosition().x) {
			m_ThrowVect.x = -ThrowSpeed.x;
		}
		//左側にプレイヤーがいるなら
		else {
			m_ThrowVect.x = ThrowSpeed.x;
		}
		m_ThrowVect.y = ThrowSpeed.y;
	}

	//ワイヤーキャッチ状態にする
	void CatchWire() { m_CatchWire = enCatchWire::Catch; }
	int GetCatchWire() { return m_CatchWire; }
protected:
	void Animation() override {};

	//ステージとの当たり判定
	virtual void StageCollision(double OffsetPos_X, double OffsetPos_Y) override {};
protected:
	VECTOR2_f m_PlayerPos;

	int m_AnimationCo;		//アニメーション変更時間カウント
	int m_AnimationTime;	//アニメーション変更時間

	bool m_HitBack;			//攻撃を受けたときのヒットバック中かを見る
	int m_HitBackCo;		//ヒットバック中のカウント
	VECTOR2_f m_HitBackSpeed;	//ヒットバックする速度

	VECTOR2_f m_ThrowVect;		//投げたベクトルを見る

	int m_HitStopCo;			//ヒットストップする時間をカウント

	VECTOR2_f m_Vector;			//移動ベクトル

	int m_ThrowTimeCo;			//投げられた時間をカウント

	int m_DeadAnimCo;			//死んだ時のアニメーションのカウント
};