#pragma once
#include "CGameObject/CCharacter/CCharacter.h"	//キャラクタークラス
#include "CGame/CBulletFactory/CBulletFactory.h"//バレットを作るクラス

/*************************************************************************************************
*		ボスクラス(親)
*/
class CBoss
	:public CCharacter
{
public:
	bool AttackHit = false;	//攻撃をくらったか

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

	//ボスとの戦闘フラグを踏んだ
	virtual void BossBattleFlag(VECTOR2_f PlayerPos) = 0;

	//会話などのムービーシーンの時に動作する関数	現在何の会話をしているかを取得する
	virtual void MovieSceneUpdate() = 0;

	//プレイヤーの場所を見ておく
	void SetPlayerPos(VECTOR2_f PlayerPos) { m_PlayerPos = PlayerPos; }

	//カメラの場所をセット
	void SetCameraPos(VECTOR2_f CameraPos) { m_CameraPos = CameraPos; }
protected:
	void Animation() override {};

	//ステージとの当たり判定
	void StageCollision(double OffsetPos_X, double OffsetPos_Y) override{};
protected:
	VECTOR2_f m_PlayerPos;		//プレイヤーの位置	

	VECTOR2_f m_CameraPos;		//カメラの位置
};