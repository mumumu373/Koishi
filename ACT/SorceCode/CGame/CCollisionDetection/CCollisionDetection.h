#pragma once
#include <math.h>
#include "CGameObject/CCharacter/CPlayer/CPlayer.h"	//プレイヤークラス
#include "CGameObject/CCharacter/CEnemy/CEnemy.h"		//エネミークラス
#include "CGameObject/CCharacter/CWirepoint/CWirepoint.h"//ワイヤーポイントクラス
#include "CCamera/CCamera.h"								//カメラクラス
/****************************************************************************
*		当たり判定クラス
*/

class CCollisionDetection
{
public:
	CCollisionDetection();
	~CCollisionDetection();

	void Update();

	//プレイヤーとエネミーの当たり判定
	void PlayerToEnemyCollision(std::unique_ptr<CPlayer>& upPlayer, std::vector<std::unique_ptr<CEnemy>>& upEnemy);

	void MouseToEnemyCollision(std::vector<std::unique_ptr<CEnemy>>& upEnemy, std::unique_ptr<CCamera>& Camera);
	//ワイヤーとエネミーの当たり判定
	void WireToWirepointCollision(std::vector<std::unique_ptr<CWirepoint>>& upEnemy, std::unique_ptr<CWire>& pWire);
public:

	//円どうしの当たり判定
	bool CircleDetection(ObjectInfo A, ObjectInfo B)
	{
		//斜辺
		double ObliqueLine;
		//斜辺を求める計算式
		ObliqueLine = sqrt((B.x + (B.xw / 2) - (A.x + (A.xw / 2))) * (B.x + (B.xw / 2) - (A.x + (A.xw / 2)))
			+ ((B.y + (B.yh / 2)) - (A.y + (A.yh / 2))) * ((B.y + (B.yh / 2)) - (A.y + (A.yh / 2))));
		//オブジェクトの半径どうしが重なったら
		if (ObliqueLine <= A.xw / 2 + B.xw / 2) {
			return true;
		}
		return false;
	}

	//矩形どうしの当たり判定
	bool SquareDetection(ObjectInfo A, ObjectInfo B)
	{
		//矩形判定
		if ((A.x <= B.x + B.xw) && (B.x <= A.x + A.xw)
			&& (A.y <= B.y + B.yh) && (B.y <= A.y + A.yh))
		{
			//重なった時
			return true;
		}
		return false;
	}

	//円と矩形の当たり判定
	bool CircleToSquareDetection(ObjectInfo A, ObjectInfo B)
	{
		if (SquareDetection(A, B)) { return true; }
		if (CircleDetection(A, { B.x,B.y,0,0 })) { return true; }
		if (CircleDetection(A, { B.x + B.xw,B.y,0,0 })) { return true; }
		if (CircleDetection(A, { B.x,B.y + B.yh,0,0 })) { return true; }
		if (CircleDetection(A, { B.x + B.xw,B.y + B.yh,0,0 })) { return true; }
		return false;
	}

private:
	//プレイヤーの位置情報+当たり判定かキャラクターサイズかを見る
	ObjectInfo SetPlayerInfo(std::unique_ptr<CPlayer>& upPlayer, bool RealSize);
	//エネミーの位置情報
	ObjectInfo SetEnemyInfo(std::unique_ptr<CEnemy>& upEnemy, bool RealSize);
private:
};