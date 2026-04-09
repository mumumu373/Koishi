#pragma once
#include "CGameObject/CGameObject.h"	//ゲームオブジェクトクラス

/******************************************************************************************
*		プレイヤーのハートクラス(体力)
*/
class CHeart
	:public CGameObject
{
public:
	//状態
	enum enState
	{
		Living,	//生存中
		Danger,	//危険な状態
		Dead,	//インスタンスを消す
	};

	int m_State = enState::Living;

	//属性
	enum enColor
	{
		NoColor = 0,//無属性
		Red,		//赤
		Yellow,		//黄
		Green,		//緑
		Blue,		//青
	};

	int m_Color = enColor::NoColor;
public:
	CHeart();
	~CHeart();

	virtual void Update() override;
	void Draw();
	virtual void Draw(std::unique_ptr<CCamera>& pCamera) override {};

	//プレイヤーの属性を取得して属性を変える
	void ChangeHeartColor(int Color);
private:
	void Animation();
private:
	int m_Alpha;				//透明度

	VECTOR2 m_FrameSize;		//切り抜く画像のサイズ
};