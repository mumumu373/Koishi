#pragma once
#include "CGameObject/CBullet/CRotateBullet/CRotateBullet.h"	//周りをまわるバレット
#include "CGameObject/CCharacter/CBoss/CBoss.h"	//ボスクラス

/***********************************************************************************************
*		バレットを作るクラス
*/
class CBulletFactory
{
public:
	CBulletFactory();
	~CBulletFactory();

	//ボスがバレットを作ったかを確認する
	void CreateBossBullet(std::unique_ptr<CBoss>& upBoss, std::vector<std::unique_ptr<CBullet>>& upBullet);

	//周りをまわるバレットを作る							陣営			ポジション		X方向の速さ		Y方向の速さ
	inline std::unique_ptr<CBullet> CreateRotateBullet(int Camp, VECTOR2_f Pos, double X_Speed, double Y_Speed)
	{
		return std::make_unique<CRotateBullet>(Camp, Pos, X_Speed, Y_Speed);
	}

private:

};