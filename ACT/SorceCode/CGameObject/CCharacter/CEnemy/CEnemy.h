#pragma once
#include "CGameObject/CCharacter/CCharacter.h"

/************************************************************************************
*		エネミークラス
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

protected:
	void Animation() override {};

};