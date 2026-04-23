#pragma once
#include "CGameObject/CCharacter/CEnemy/CEnemy.h"		//継承
#include "CGameObject/CCharacter/CEnemy/CYinYangBall/CYinYangDeco/CYinYangDeco.h"	//装飾クラス

/***********************************************************************
*	エネミー陰陽玉クラス
*/
class CYinYangBall
	: public CEnemy
{
public:
	//呼び出すときに種類を決める
	CYinYangBall(int Kinds, VECTOR2_f SetPos);
	~CYinYangBall();

	void StartSetting() override;
	void Update() override {};
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override;

	//プレイヤーの攻撃がヒットした時の動作
	void PlayerAttackHit(int Damage) override;
private:
	void Animation() override;

	//ステージとの当たり判定	陰陽玉には必要なし
	void StageCollision(double OffsetPos_X, double OffsetPos_Y) override {};
private:
	int m_AmountDeco;		//装飾の量

	bool m_BulletShot;		//バレットを撃ったか
	int m_BulletShotTiming;	//バレットを撃ちだすタイミング	
	int m_BulletShotCo;		//バレットを撃つタイミングをカウント
	int m_AmountBullet;		//打ち出すバレットの量
	double m_ShotBulletAngle;	//打ち出すバレットの角度
	int m_ShotReleaseTime;	//バレットの持続時間

	//装飾クラスをここで作る(装飾含めての陰陽玉)
	std::vector<std::unique_ptr<CYinYangDeco>> m_upYinYangDeco;
};