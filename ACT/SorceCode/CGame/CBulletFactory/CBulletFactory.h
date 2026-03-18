#pragma once
#include "CGameObject/CBullet/CRotateBullet/CRotateBullet.h"	//周りをまわるバレット

/***********************************************************************************************
*		バレットを作るクラス
*/
class CBulletFactory
{
public:
	CBulletFactory();
	~CBulletFactory();

	//周りをまわるバレットを作る							陣営			ポジション		X方向の速さ		Y方向の速さ
	static inline std::unique_ptr<CBullet> CreateRotateBullet(int Camp, VECTOR2_f Pos, double X_Speed, double Y_Speed, double X_Range, double Y_Range)
	{
		return std::make_unique<CRotateBullet>(Camp, Pos, X_Speed, Y_Speed, X_Range, Y_Range);
	}

private:

};