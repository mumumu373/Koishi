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

		MAX,		//属性の最大値
	};

	int m_Color = enColor::NoColor;
public:
	CHeart(int PlayerHP);
	~CHeart();

	virtual void Update() override;
	void Draw();
	virtual void Draw(std::unique_ptr<CCamera>& pCamera) override {};

	//プレイヤーの属性を取得して属性を変える+属性ごとの体力を渡す
	int ChangeHeartColor(int Color, int PlayerHP);

	//プレイヤーがダメージを受けたときに、ダメージを反映させ、現在HPを確認する
	int PlayerHeartDamage(int Color, int PlayerHP);
private:
	void Animation();
private:
	int m_Alpha;				//透明度

	VECTOR2 m_FrameSize;		//切り抜く画像のサイズ

	int m_ColorHP[enColor::MAX];	//属性ごとのHPを持つようにする
	int m_HaveHP;					//HPを保持しておく変数
};