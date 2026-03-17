#pragma once
#include "CGameObject/CGameObject.h"	//ゲームオブジェクトクラス

/**************************************************************
*		バレットクラス(親)
*/
class CBullet
	:public CGameObject
{
public:
	//バレットの状態
	enum enState
	{
		Set = 0,//本動作をする前の準備
		Living,	//生存中
		Dying,	//死亡中アニメーション用
		Dead,	//インスタンスを消す
	};

	int m_State = enState::Set;

	//自分が何の陣営なのか
	enum enMyCamp
	{
		PlayerCamp = 0,	//プレイヤー
		EnemyCamp,		//エネミー
	};

	int m_MyCamp = enMyCamp::PlayerCamp;

	//バレットの属性
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
	CBullet();
	virtual ~CBullet() {};

	virtual void Update() override {};
	virtual void Draw(std::unique_ptr<CCamera>& pCamera) override {};

	virtual void CharacterHit() {};		//ヒットした時のバレットの処理

protected:
	virtual void Animation() {};

protected:
	VECTOR2_f m_Speed;	//速度

	VECTOR2 m_FrameSize;//元画像サイズ
	int m_Alpha;		//透明度
	double m_Delection;	//角度
};