#pragma once
#include "CGameObject/CCharacter/CPlayer/CPlayer.h"
#include "CGameObject/CCharacter/CWirepoint/CWirepoint.h"
class CWireActionSupporter
{
public:
	CWireActionSupporter();
	~CWireActionSupporter();
	void StartWireAction(CPlayer**m_DPlayer, CWire** m_dpWire,CWirepoint** m_dpWirePoint);
	void Update() ;
private:
	CPlayer** m_dpPlayer;//プレイヤーのダブルポインタ
	CWire** m_dpWire;//ワイヤーのポインタ
	CWirepoint** m_dpWirePoint;//ワイヤーポイントのダブルポインタ
};

