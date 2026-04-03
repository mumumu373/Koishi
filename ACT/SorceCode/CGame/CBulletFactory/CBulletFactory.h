#pragma once
#include "CGameObject/CBullet/CRotateBullet/CRotateBullet.h"	//周りをまわるバレット
#include "CGameObject/CBullet/CCircularBullet/CCircularBullet.h"	//360°方向に撃てるバレット

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
		//	陣営			ポジション	属性			X方向の速さ		Y方向の速さ			X方向の距離		Y方向の距離			開始角度
		int Camp, VECTOR2_f Pos, int Color, double X_Speed, double Y_Speed, double X_Range, double Y_Range, double StartAngle)
	{
		return std::make_unique<CRotateBullet>(Camp, Pos, Color, X_Speed, Y_Speed, X_Range, Y_Range, StartAngle);
	}
	//360°方向に撃てるバレット										
	static inline std::unique_ptr<CBullet> CreateCircularBullet(
		//陣営			位置			属性			スピード		打ち出す角度			初期角度			Xサイズ		Yサイズ
		int Camp, VECTOR2_f Pos, int Color, double Speed, double Vector, double StartAngle, int X_Size, int Y_Size)
	{
		return std::make_unique<CCircularBullet>(Camp, Pos, Color, Speed, Vector, StartAngle, X_Size, Y_Size);
	}
private:

};