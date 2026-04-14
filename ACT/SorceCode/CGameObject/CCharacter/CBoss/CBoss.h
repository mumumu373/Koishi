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
	CBoss() {};
	virtual ~CBoss() {};

	virtual void StartSetting() override {};
	virtual void Update() override {};
	virtual void Draw(std::unique_ptr<CCamera>& pCamera) override {};

	virtual void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override {};

protected:
	void Animation() override {};

	//ステージとの当たり判定
	void StageCollision(double OffsetPos_X, double OffsetPos_Y) override{};
protected:
};