#pragma once
#include "Global.h"		//なんでも屋さん
#include "CCamera/CCamera.h"	//カメラクラス

/*************************************************************
*		ゲームオブジェクトクラス
*		継承を前提にしているクラス
*/
class CGameObject
{
public:
	CGameObject();
	virtual ~CGameObject(){}

	virtual void Update() = 0;

	//参照でなければエラーとなる
	virtual void Draw(std::unique_ptr<CCamera>& pCamera) = 0;

	void SetImage(CImage* pImg) { m_pImg = pImg; }

	//ポジションを渡す
	VECTOR2_f GetPosition() { return m_Position; }
	//ポジションを渡す(真ん中から)
	VECTOR2_f GetCenterPosition();
	//ポジションのアドレスを渡す
	VECTOR2_f* GetPositionadd() { return &m_Position; }
	//フレームを渡す
	CImage::FRAMESPLIT GetFrameSplit() { return m_Framesplit; }
protected:
	CImage* m_pImg;
	
	VECTOR2_f m_Position;
	CImage::FRAMESPLIT m_Framesplit;
};
