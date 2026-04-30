#pragma once
#include "CGameObject/CGameObject.h"	//ゲームオブジェクトクラス

/***********************************************************************************
*		ゲームタイトルクラス
*/
class CTitleImage
{
public:
	const double SetGroundPos = 0;	//地面を映する場所を指定

	bool SetCompleted = false;		//セットが完了したか
public:
	CTitleImage();
	~CTitleImage();

	void Update();
	void Draw();

	//画面配置を規定のものにする関数
	void SetInitializePos();

private:
	VECTOR2_f m_Position;	

	double m_Speed;		//速度
};