#pragma once
#include "CGameObject/CGameObject.h"	
#include "CGameObject//CCharacter/CPlayer/CPlayer.h"	//プレイヤー


class CWirepoint;
class CPlayer;

//ワイヤークラス
class CWire
	:public CGameObject
{
private:
	static constexpr int size = 60;//ワイヤーの長さ
	static constexpr int IMGSize = 96;//ワイヤーの最大の長さ
	static constexpr int GoSpeed = 30;//ワイヤーの速さ
	static constexpr int ComebackSpeed = 60;//ワイヤーの速さ
	static constexpr int WireMaxRongr = 600;//ワイヤーの最大の長さ
public:
	enum ShotSteto{
		no,
		forward,
		Rock,
		back,
	};

	CWire();//プレイヤーのダブルポインタと目標地点を渡す
	~CWire();
	void Shot(std::unique_ptr<CPlayer>& Player, VECTOR2_f Targetpoint) ;
	void Update()override;
	void Draw(std::unique_ptr<CCamera>& pCamera)override;
	void StatoWire(VECTOR2_f pos);
	bool Getcatch();
	void Startcatch(CWirepoint* point);
	int GetSize() { return size; }
	bool GetRock() { 
		if ( m_ShotState == ShotSteto::Rock) {
			return true;
		}
		return false;
		 }
	CWirepoint* GetCatchPoint() { return m_CatchWirepoint; }
	bool canShot();
	VECTOR2_f GetTopPoint() { return m_Toptpoint; };
	void WireEnd() { m_ShotState = ShotSteto::back; }
	int  GetWireMaxRongr() { return WireMaxRongr; }
private:
	int m_ShotState = ShotSteto::no;//ワイヤーの状態
	CWirepoint* m_CatchWirepoint;//掴んだワイヤーポイントのポインタ
	CPlayer* m_DpPlayer;//プレイヤーのポインタ
	double m_Radian;//ワイヤーの角度
	VECTOR2_f m_Toptpoint;//ワイヤーの先端の座標
	VECTOR2_f m_Targetpoint;//ワイヤーの目標地点
};