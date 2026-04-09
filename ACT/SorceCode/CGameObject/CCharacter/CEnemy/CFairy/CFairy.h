#pragma once
#include "CGameObject/CCharacter/CEnemy/CEnemy.h"		//継承

/***********************************************************************
*	エネミー妖精クラス
*/
class CFairy
	: public CEnemy
{
public:
	//呼び出すときに種類を決める
	CFairy(int Kinds, VECTOR2_f SetPos);
	~CFairy();

	void StartSetting() override;
	void Update() override {};
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override;
private:
	void Animation() override;
private:
	bool m_BulletShot;		//バレットを撃ったか
	int m_BulletShotTiming;	//バレットを撃ちだすタイミング	
	int m_BulletShotCo;		//バレットを撃つタイミングをカウント
	int m_ShotReleaseTime;	//バレットの持続時間
};