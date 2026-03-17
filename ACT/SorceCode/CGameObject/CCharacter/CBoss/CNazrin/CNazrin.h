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
	void Update() override;
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

private:
	void Animation() override;
};