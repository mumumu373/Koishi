#pragma once

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

	//プレイヤーのマネージャーをセットする
//	void SetPlayerManager(CPlayerManager* pPlayerManager) { m_pPlayerManager = pPlayerManager; }

	//カメラをセットする
//	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }

public:
	//後でコメント解除
	////円どうしの当たり判定
	//bool CircleDetection(ObjectInfo A, ObjectInfo B)
	//{
	//	//斜辺
	//	double ObliqueLine;
	//	//斜辺を求める計算式
	//	ObliqueLine = sqrt((B.x + (B.xw / 2) - (A.x + (A.xw / 2))) * (B.x + (B.xw / 2) - (A.x + (A.xw / 2)))
	//		+ ((B.y + (B.yh / 2)) - (A.y + (A.yh / 2))) * ((B.y + (B.yh / 2)) - (A.y + (A.yh / 2))));
	//	//オブジェクトの半径どうしが重なったら
	//	if (ObliqueLine <= A.xw / 2 + B.xw / 2) {
	//		return true;
	//	}
	//	return false;
	//}

	////矩形どうしの当たり判定
	//bool SquareDetection(ObjectInfo A, ObjectInfo B)
	//{
	//	//矩形判定
	//	if ((A.x <= B.x + B.xw) && (B.x <= A.x + A.xw)
	//		&& (A.y <= B.y + B.yh) && (B.y <= A.y + A.yh))
	//	{
	//		//重なった時
	//		return true;
	//	}
	//	return false;
	//}

	////円と矩形の当たり判定
	//bool CircleToSquareDetection(ObjectInfo A, ObjectInfo B)
	//{
	//	if (SquareDetection(A, B)) { return true; }
	//	if (CircleDetection(A, { B.x,B.y,0,0 })) { return true; }
	//	if (CircleDetection(A, { B.x + B.xw,B.y,0,0 })) { return true; }
	//	if (CircleDetection(A, { B.x,B.y + B.yh,0,0 })) { return true; }
	//	if (CircleDetection(A, { B.x + B.xw,B.y + B.yh,0,0 })) { return true; }
	//	return false;
	//}


private:
	//オブジェクトを構造体に入れる処理
	//プレイヤー
	//void PlayerStruct();

private:
	////構造体に入れる関数
	//ObjectInfo JoindObjectInfo_A(VECTOR2_f Pos, CImage::FRAMESPLIT Frame)
	//{
	//	//計算してから渡すようにする
	//	ObjectInfo A;
	//	A.x = Pos.x;
	//	A.y = Pos.y;
	//	A.xw = Frame.w;
	//	A.yh = Frame.h;

	//	return A;
	//}
	//ObjectInfo JoindObjectInfo_B(VECTOR2_f Pos, CImage::FRAMESPLIT Frame)
	//{
	//	//計算してから渡すようにする
	//	ObjectInfo B;
	//	B.x = Pos.x;
	//	B.y = Pos.y;
	//	B.xw = Frame.w;
	//	B.yh = Frame.h;

	//	return B;
	//}

private:
	//ObjectInfo m_PlayerInfo[PLAYER_MAX];					//プレイヤーの位置情報
	//ObjectInfo m_BulletInfo[CBulletManager::BULLET_MAX];	//バレットの位置情報
	//ObjectInfo m_WallInfo[CWallManager::WALL_MAX];			//ウォールの位置情報

	//CCamera* m_pCamera;		//カメラを制御する
};