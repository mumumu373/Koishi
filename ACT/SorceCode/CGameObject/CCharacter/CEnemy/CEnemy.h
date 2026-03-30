#pragma once
#include "CGameObject/CCharacter/CCharacter.h"

/************************************************************************************
*		エネミークラス(親)
*/
class CEnemy
	:public CCharacter
{
public:
	CEnemy() {};
	virtual ~CEnemy() {};

	virtual void StartSetting() override {};
	virtual void Update() override {};
	virtual void Draw(std::unique_ptr<CCamera>& pCamera) override {};

	//こっちのほうを使う
	virtual void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override {};

protected:
	void Animation() override {};

};