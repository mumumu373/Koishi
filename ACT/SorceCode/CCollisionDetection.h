#pragma once
#include <math.h>
#include "CPlayer.h"	//プレイヤークラス
#include "CEnemy.h"		//エネミークラス

/****************************************************************************
*		当たり判定クラス
*/
class CCollisionDetection
{
public:
	//オブジェクトの位置と描画の情報
	struct ObjectInfo
	{
		double x;	//x座標
		double y;	//y座標
		int xw;		//幅
		int yh;		//高さ
	};
public:
	CCollisionDetection();
	~CCollisionDetection();

	void Update();

	//各クラスをセット
	void SetCharacter(CPlayer* pPlayer, CEnemy* pEnemy)
	{
		m_pPlayer = pPlayer;
		m_pEnemy = pEnemy;
	}

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
	//キャラクター同士の当たり判定
	void CharacterCollision();

	//キャラクターの位置情報をセットする
	ObjectInfo SetCharacterInfo(CCharacter* pCharacter);
private:
	CPlayer* m_pPlayer;
	CEnemy* m_pEnemy;
};