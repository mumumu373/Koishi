#pragma once
#include "CGameObject/CCharacter/CBoss/CBoss.h"	//ボスクラス(継承)
#include "CGame/CBulletFactory/CBulletFactory.h"//バレットを作るクラス

/**********************************************************************************************************
*		ナズーリン(1面ボス)
*/
class CNazrin
	:public CBoss
{
public:
	//攻撃のパターン
	enum enAttackMove
	{
		Standby,	//待機
		Move_01,	//左下端からバレットを飛ばしてから右下端からバレットを出す
		Move_02,	//左右にステップしてから上空に行って、バレットを降らせる
	};

	int m_AttackMove = enAttackMove::Standby;
public:
	CNazrin();
	~CNazrin();

	void StartSetting() override;
	void Update() override {};
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override;
private:
	void Animation() override;

	//ステージとの当たり判定
	void StageCollision(double OffsetPos_X, double OffsetPos_Y) override {};
private:
	bool m_Jumping;			//今ジャンプしているか
	int m_JumpingCo;		//ジャンプするタイミングをカウント

	bool m_BulletShot;		//バレットを撃つ
	int m_BulletShotCo;		//撃つまでのカウント

	double m_FallingSpeed;	//落下速度
	bool m_GroundStand;		//地面に立っているか	

	int m_AttackMoveChangeCo;	//攻撃のパターンを変えるタイミングをカウント
};