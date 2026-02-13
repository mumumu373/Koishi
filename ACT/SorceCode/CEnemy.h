#pragma once
#include "CCharacter.h"

/************************************************************************************
*		エネミークラス
*/
class CEnemy
	:public CCharacter
{
public:
	CEnemy();
	~CEnemy();

	void StartSetting() override;
	void Update() override;
	void Draw(CCamera* pCamera) override;

private:
	void Animation() override;
private:

};