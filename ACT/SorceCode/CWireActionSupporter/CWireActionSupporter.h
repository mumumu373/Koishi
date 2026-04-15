#pragma once
#include "CGameObject/CCharacter/CPlayer/CPlayer.h"
#include "CGameObject/CCharacter/CWirepoint/CWirepoint.h"
class CWireActionSupporter
{
public:
	CWireActionSupporter();
	~CWireActionSupporter();
	void StartWireAction(CPlayer*m_DPlayer, CWire* m_DPWire,CWirepoint* m_DPWirePoint);
	void Update() ;
	void WireActionEnd();//ワイヤーアクション終了
private:
	//ステージとの当たり判定
	void StageCollision(double OffsetPos_X, double OffsetPos_Y);
private:
	bool WireActioning;//ワイヤーアクション中か
	double NawSpeed;//-なら左、+なら右
	CPlayer* m_dpPlayer;//プレイヤーのダブルポインタ
	CWire* m_dpWire;//ワイヤーのポインタ
	CWirepoint* m_dpWirePoint;//ワイヤーポイントのダブルポインタ

	VECTOR2_f pos[2];
};

