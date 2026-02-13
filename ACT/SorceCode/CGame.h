#pragma once
#include <Windows.h>
#include "Global.h"
#include "CSound.h"		//サウンドクラス
#include "CImageManager.h"		//イメージクラス
#include "CStage.h"		//ステージクラス
#include "CCamera.h"	//カメラクラス
#include "CSoundManager.h"		//サウンドマネージャークラス
#include "CCollisionDetection.h"//当たり判定クラス
#include "CPlayer.h"	//プレイヤークラス
#include "CEnemy.h"		//エネミーマネージャークラス

/************************************************************
*			ゲームクラス
*/
class CGame
{
public:
//---------------------------------------
//	列挙型宣言.
//---------------------------------------
//シーン列挙型
enum enScene
{
	Title,		//タイトル
	TitleToGameMain,	//タイトルからゲームメインへ
	GameMain,	//ゲーム中
	GameMainToResult,	//ゲームメインからリザルトへ
	Result,		//リザルト
	ResultToTitle,		//リザルトからタイトルへ
};

int m_Scene;	//ゲームシーン

//世界の状態	(時を止めたりする)
enum enWorldState
{
	Normal,		//通常
	StopWorld,	//ゲームメインで動作する物を一時的に止める
	PlayerDeath,//どちらかのプレイヤーが死亡中になったらこの状態になる	(演出を行うため)
};

int m_WorldState = enWorldState::Normal;

public:
	bool NoCreateInstance = false;	//作りたくないものは作らない
	bool NoDeleteInstance = false;	//消したくないものは消さない
public:
	CGame();		//コンストラクタ
	CGame(GameWindow* pGameWnd);	//コンストラクタ(引数あり)
	~CGame();		//デストラクタ

	//初期化(リセット)関数
	void InitializeGame();

	//構築関数
	bool Create();

	//破棄関数
	void Destroy();

	//更新関数(キー入力や動作処理を行う)
	void Update();

	//描画関数(画像の表示処理を行う)
	void Draw();

	//ウィンドウを閉じているか確認
	BOOL IsWindowClose() const { return m_pGameWnd->isWndClose; }
	//FPSを取得
	DWORD GetFPS() const { return m_pGameWnd->dwFPS; }
	//バックバッファを取得
	HDC GetScreenDC() const { return m_pGameWnd->hScreenDC; }
	//ウィンドウハンドルを取得
	HWND GetWnd() const { return m_pGameWnd->hWnd; }

private:
	//クラスをセットする関数
	void SetClass();

	//インスタンスを破棄する関数
	void DeleteInstance();

	GameWindow* m_pGameWnd;	//ゲームウィンドウ構造体.

	HDC m_hMemDC;  //メモリデバイスコンテキスト.
	HDC m_MasterWorkDC;				//作業用DCの元
	HBITMAP m_MasterWorkBmp;		//作業用BITMAPの元
	//この辺にゲーム用DCとBMPがほしい------------------------------------------
	HDC m_hWorkDC;					//作業用DC
	HBITMAP m_hWorkBmp;				//作業用BITMAP
	HDC m_hWorkDC2;					//作業用DC2
	HBITMAP m_hWorkBmp2;			//作業用BITMAP2
	HFONT m_hFont;	//フォントハンドル.

	//														-ゲームシステム面のもの-
	//----------------------当たり判定----------------------
	CCollisionDetection* m_pCollisionDetection;	//当たり判定クラス

	//														-ゲームシーン系-
	//----------------------ブラックフェード-------------
	 
	//----------------------ゲームタイトル---------------
	//CGameTitle* m_pGameTitle;

	//----------------------リザルト--------------------
	//CResult* m_pResult;

	//														-ゲームUI系-
	//----------------------ゲージ-----------------------
	//CGaugeManager* m_pGaugeManager;

	//----------------------プレイヤーUI-----------------
	//CPlayerUIManager* m_pPlayerUIManager;

	//														-ゲームオブジェクト系-
	//----------------------プレイヤー-------------------
	CPlayer* m_pPlayer;

	//----------------------エネミー---------------------
	CEnemy* m_pEnemy;	//後でマネージャーを呼び出す

	//----------------------バレット---------------------

	//----------------------ステージ--------------------
	CStage* m_pStage;

	//----------------------カメラ-----------------------
	CCamera* m_pCamera;
};