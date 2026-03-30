#pragma once
#include "CGameObject/CCharacter/CEnemy/CEnemy.h"		//継承

/***********************************************************************
*	エネミー毛玉クラス
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
};