#pragma once
#include "CGameObject/CCharacter/CCharacter.h"

/************************************************************************************
*		エネミークラス(親)
*/
class CEnemy
	:public CCharacter
{
public:
	CEnemy();
	virtual ~CEnemy() {};

	enum CatchWire {
		No,
		Catch,

	};
	

	virtual void StartSetting() override {};
	virtual void Update() override {};
	virtual void Draw(std::unique_ptr<CCamera>& pCamera) override {};

	//こっちのほうを使う
	virtual void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override {};

	//プレイヤーの位置を取得
	void SetPlayerPos(VECTOR2_f PlayerPos) { m_PlayerPos = PlayerPos; }

	void SetPosition(VECTOR2_f pos) { m_Position = pos; }

	void CatchWire() { m_CatchWire = CatchWire::Catch; }
	int GetChtchWire() { return m_CatchWire; }
protected:
	void Animation() override {};

	//ステージとの当たり判定
	virtual void StageCollision(double OffsetPos_X, double OffsetPos_Y) override {};
protected:
	VECTOR2_f m_PlayerPos;
	int m_CatchWire;
};