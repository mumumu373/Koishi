#pragma once
#include "CGameObject/CGameObject.h"	//ゲームオブジェクトクラス

/***********************************************************************************
*		クリアイメージクラス
*/
class CClearImage
{
public:
	bool FadeCompleted = false;		//フェードが完了したか

	bool DrawClearImage = false;	//クリアイメージを描画したか
public:
	CClearImage();
	~CClearImage();

	void Update();
	void Draw();

	//フェードアウトを始める
	void FadeOutStart() { m_Fade = false; }

private:
	int m_Alpha;		//透明度

	bool m_Fade;		//フェード中か
};