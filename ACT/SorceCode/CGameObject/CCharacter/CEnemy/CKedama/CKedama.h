#pragma once
#include "CGameObject/CCharacter/CEnemy/CEnemy.h"		//継承

/***********************************************************************
*	エネミー毛玉クラス
*/
class CKedama
	: public CEnemy
{
public:
	//最大落下速度
	static constexpr double MAX_FALLING_SPEAD = 25;
public:
	//呼び出すときに種類を決める
	CKedama(int Kinds, VECTOR2_f SetPos);
	~CKedama();

	void StartSetting() override;
	void Update() override;
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

private:
	void Animation() override;
};