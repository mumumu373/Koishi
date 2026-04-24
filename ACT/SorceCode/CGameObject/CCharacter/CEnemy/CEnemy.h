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

	bool AttackHit = false;	//攻撃をくらったか

	int NoHitAttackCo = 0;	//攻撃があたらない時間をカウント
	const int NoHitAttackTime = 60;	//攻撃があたらない時間
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

	//プレイヤーの位置を取得
	void SetPlayerPos(VECTOR2_f PlayerPos) { m_PlayerPos = PlayerPos; }

	void SetPosition(VECTOR2_f pos) { m_Position = pos; }

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
};