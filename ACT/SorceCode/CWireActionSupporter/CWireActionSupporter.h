#pragma once
#include "CGameObject/CCharacter/CPlayer/CPlayer.h"
#include "CGameObject/CCharacter/CWirepoint/CWirepoint.h"
#include <CGameObject/CCharacter/CEnemy/CEnemy.h>
class CWireActionSupporter
{
private:
	static constexpr double AttackEria=100;//敵ENEMIへの攻撃射程
	static constexpr double EnemiReedPower = 50;
public:
	CWireActionSupporter();
	~CWireActionSupporter();
	void StartWireAction(CPlayer*m_DPlayer, CWire* m_DPWire,CWirepoint* m_DPWirePoint);
	void StartWireActionEnemi(CPlayer* m_DPlayer, CWire* m_DPWire, CEnemy* m_DPEnemi);
	void Update() ;
	void WireActionEnd();//ワイヤーアクション終了


private:
	//ステージとの当たり判定
	void StageCollision(double OffsetPos_X, double OffsetPos_Y);

	void WirePointAction();
	void EnemitoAction();
	void EnemiActionEnd(); 
	void AllNullptr();
	void PlayerTurnaround();
private:
	bool WireActioning;//ワイヤーアクション中か
	double NawSpeed;//-なら左、+なら右
	CPlayer* m_dpPlayer;//プレイヤーのダブルポインタ
	CWire* m_dpWire;//ワイヤーのポインタ
	CWirepoint* m_dpWirePoint;//ワイヤーポイントのダブルポインタ
	CEnemy* m_dpEnemi;//エネミー
	VECTOR2_f pos[2];
	bool OldWallHit;
};

