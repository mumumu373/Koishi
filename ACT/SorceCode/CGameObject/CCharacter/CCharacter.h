#pragma once
#include "CGameObject/CGameObject.h"	//ゲームオブジェクトクラス
#include "CGame/CBulletFactory/CBulletFactory.h"	//バレットを作るクラス

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

	//自分が何のキャラクターなのか
	enum enMyCharacter
	{
		Player = 0,	//プレイヤー
		Kedama,		//毛玉
		Fairy,		//妖精
		//以降敵の名前
		Nazrin,		//ボス
	};

	int m_MyCharacter = enMyCharacter::Player;

	//自分が何の陣営なのか
	enum enMyCamp
	{
		PlayerCamp = 0,	//プレイヤー
		EnemyCamp,		//エネミー
	};

	int m_MyCamp = enMyCamp::PlayerCamp;

	//キャラの属性
	enum enColor
	{
		NoColor = 0,//無属性
		Red,		//赤
		Yellow,		//黄
		Green,		//緑
		Blue,		//青
	};

	int m_Color = enColor::NoColor;

	//キャラの動きの状態
	enum enMoveState
	{
		Wait = 0,		//待機状態
		MoveLeft,		//左移動
		MoveRight,		//右移動
	};

	int m_MoveState = enMoveState::Wait;

	bool m_NoDamageTime;		//無敵時間中

	VECTOR2_f m_Speed;			//移動速度

	VECTOR2 m_RealFrameSplit;	//実際の当たり判定
public:
	CCharacter();
	virtual ~CCharacter(){}

	virtual void StartSetting() = 0;		//初期設定

	virtual void Update() override{}		// {}←定義

	virtual void Draw(std::unique_ptr<CCamera>& pCamera) override {}

	//オーバーロードさせる
	virtual void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) = 0;
protected:
	virtual void Animation() = 0;			//アニメーション関数
protected:
	int m_Alpha;				//透明度
	double m_Delection;			//角度

	int m_NoDamageCo;			//無敵時間をはかる

	VECTOR2_f m_OldPosition;	//移動前の位置

	VECTOR2 m_FrameSize;		//切り抜く画像のサイズ

	int m_AnimetionCo;			//アニメーション切り替えのカウンター
};