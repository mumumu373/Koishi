#pragma once
#include "CGameObject/CCharacter/CEnemy/CEnemy.h"		//継承
#include "CGameObject/CCharacter/CEnemy/CYinYangBall/CYinYangDeco/CYinYangDeco.h"	//装飾クラス

/***********************************************************************
*	エネミー陰陽玉クラス
*/
class CYinYangBall
	: public CEnemy
{
public:
	//最大落下速度
	static constexpr double MAX_FALLING_SPEAD = 25;
public:
	//呼び出すときに種類を決める
	CYinYangBall(int Kinds, VECTOR2_f SetPos);
	~CYinYangBall();

	void StartSetting() override;
	void Update() override {};
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override;
private:
	void Animation() override;
private:
	int m_AmountDeco;		//装飾の量

	//装飾クラスをここで作る(装飾含めての陰陽玉)
	std::vector<std::unique_ptr<CYinYangDeco>> m_upYinYangDeco;
};