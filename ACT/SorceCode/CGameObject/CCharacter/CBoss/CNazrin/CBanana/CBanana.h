#pragma once
#include "CGameObject/CGameObject.h"	//ゲームオブジェクトクラス

/******************************************************************************************************
*		ボスが踏んで滑るバナナクラス
*/
class CBanana
{
public:
	//呼び出された時に場所をセットする
	CBanana(VECTOR2_f SetPos);
	~CBanana();

	void Update();
	void Draw(std::unique_ptr<CCamera>& pCamera);

	//吹き飛ばす準備
	void SetBlowOffBanana();
	//バナナを吹き飛ばす
	void BlowOffBanana();

private:
	VECTOR2_f m_Position;
	CImage::FRAMESPLIT m_Framesplit;

	int m_Delection;		//角度

	double m_FallingSpeed;	//落下速度
};