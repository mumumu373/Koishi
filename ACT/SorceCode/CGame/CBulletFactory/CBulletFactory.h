#pragma once
#include "CGameObject/CBullet/CRotateBullet/CRotateBullet.h"		//周りをまわるバレット
#include "CGameObject/CBullet/CCircularBullet/CCircularBullet.h"	//360°方向に撃てるバレット
#include "CGameObject/CBullet/CRockOnBullet/CRockOnBullet.h"		//狙い打てる撃てるバレット

/***********************************************************************************************
*		バレットを作るクラス
*/
class CBulletFactory
{
public:
	CBulletFactory();
	~CBulletFactory();

	//周りをまわるバレットを作る								
	static inline std::unique_ptr<CBullet> CreateRotateBullet(
		//	陣営			ポジション	属性			速さ			
		int Camp, VECTOR2_f Pos, int Color, double Speed,
		//	X方向の距離		Y方向の距離			開始角度			サイズ		消える時間	ナズのバレットを撃つか
		double X_Range, double Y_Range, double StartAngle, int Size, int ReleaseTime, bool NazrinBullet)
	{
		return std::make_unique<CRotateBullet>(Camp, Pos, Color, Speed, X_Range, Y_Range, StartAngle, Size, ReleaseTime, NazrinBullet);
	}

	//360°方向に撃てるバレット										
	static inline std::unique_ptr<CBullet> CreateCircularBullet(
		//陣営			位置			属性			スピード		
		int Camp, VECTOR2_f Pos, int Color, double Speed,
		//打ち出す角度			初期角度		サイズ		消える時間	加速度		キャラ
		double Amount, double StartAngle, int Size, int ReleaseTime, double SpeedAcc, bool NazrinBullet)
	{
		return std::make_unique<CCircularBullet>(Camp, Pos, Color, Speed, Amount, StartAngle, Size, ReleaseTime, SpeedAcc, NazrinBullet);
	}

	//狙い撃ちするバレット
	static inline std::unique_ptr<CBullet> CreateRockOnBullet(
		int Camp, VECTOR2_f Pos, int Color, double Speed,
		VECTOR2_f TargetPos, int Size, int ReleaseTime, bool NazrinBullet)
	{
		return std::make_unique<CRockOnBullet>(Camp, Pos, Color, Speed, TargetPos, Size, ReleaseTime, NazrinBullet);
	}
private:

};