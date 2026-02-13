#pragma once
#include "CGameObject.h"	//ゲームオブジェクトクラス

/***********************************************
*			キャラクタークラス
*/
class CCharacter
	:public CGameObject		//ゲームオブジェクトを継承
{
public:
	//キャラの状態
	enum enState
	{
		Set = 0,//本動作をする前の準備
		Living,	//生存中
		Dying,	//死亡中アニメーション用
		Dead,	//インスタンスを消す
	};

	int m_State = enState::Set;

	//キャラの属性状態
	enum enColorState
	{
		NoColor = 0,//無属性
		Red,		//赤
		Yellow,		//黄
		Green,		//緑
		Blue,		//青
	};

	int m_ColorState = enColorState::NoColor;

	//キャラの動きの状態
	enum enMoveState
	{
		Wait = 0,		//待機状態
		MoveLeft,		//左移動
		MoveRight,		//右移動
	};

	int m_MoveState = enMoveState::Wait;
public:
	CCharacter();
	virtual ~CCharacter(){}

	virtual void StartSetting() = 0;		//初期設定

	virtual void Update() override{}		// {}←定義

	virtual void Draw(CCamera* pCamera) override {}
protected:
	virtual void Animation() = 0;			//アニメーション関数
protected:
	int m_Alpha;				//透明度
	double m_Delection;			//角度

	int m_NoDamageCo;			//無敵時間をはかる
	bool m_NoDamageTime;		//無敵時間中

	VECTOR2_f m_Speed;			//移動速度

	VECTOR2_f m_OldPosition;	//移動前の位置

	VECTOR2 m_FrameSize;		//切り抜く画像のサイズ
};