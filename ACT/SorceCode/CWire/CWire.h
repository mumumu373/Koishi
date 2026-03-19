#pragma once
#include "CGameObject/CGameObject.h"	
#include "CGameObject//CCharacter/CPlayer/CPlayer.h"	//プレイヤー

//ワイヤーの先端
class CPlayer;
class CWire
	:public CGameObject
{
private:
	static constexpr int size = 60;//ワイヤーの長さ
	static constexpr int IMGSize = 96;//ワイヤーの最大の長さ
	static constexpr int GoSpeed = 5;//ワイヤーの速さ
	static constexpr int ComebackSpeed = 20;//ワイヤーの速さ
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
	bool canShot() {
		if (m_ShotState == ShotSteto::no) {
			return true;
		}
	}


private:
	int m_ShotState = ShotSteto::no;//ワイヤーの状態

	CPlayer* m_DpPlayer;//プレイヤーのポインタ
	double m_Radian;//ワイヤーの角度
	VECTOR2_f m_Toptpoint;//ワイヤーの先端の座標
	VECTOR2_f m_Targetpoint;//ワイヤーの目標地点
};