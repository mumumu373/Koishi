#pragma once
#include "CGameObject/CGameObject.h"	
#include "CGameObject//CCharacter/CPlayer/CPlayer.h"	//プレイヤー

//ワイヤーの先端
class CWire
	:public CGameObject
{
public:
	CWire(double rad, std::unique_ptr<CPlayer>&& Player);//ラジアンの角度とプレイヤーのポインタを渡す
	~CWire();
	void Update()override;
	void Draw(std::unique_ptr<CCamera>& pCamera)override;


private:
	std::unique_ptr<CPlayer>*m_DpPlayer;

};