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
	CNazrin();
	~CNazrin();

	void StartSetting() override;
	void Update() override {};
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override;
private:
	void Animation() override;
private:
	bool m_BulletShot;	//バレットを撃つ
	int m_BulletShotCo;	//撃つまでのカウント
};