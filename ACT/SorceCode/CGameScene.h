#pragma once
#include "Global.h"		//グローバルから色々とる
#include "CImage.h"

/**************************************************************************************
*		ゲームシーンクラス(仮想関数)
*/
class CGameScene
{
public:
	CGameScene();
	virtual ~CGameScene(){}

	virtual void Update() = 0;
	virtual void Draw() = 0;	//カメラを参照しなくて良い

	void SetImage(CImage* pImg) { m_pImg = pImg; }

protected:
	VECTOR2_f m_Position;
	CImage::FRAMESPLIT m_Framesplit;
	VECTOR2 m_FrameSize;

	CImage* m_pImg;
};