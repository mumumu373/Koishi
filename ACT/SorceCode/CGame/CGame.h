#pragma once
#include <Windows.h>
#include "Global.h"
#include "CImage/CImageManager.h"							//イメージクラス
#include "CCamera/CCamera.h"								//カメラクラス
#include "CSound/CSoundManager.h"							//サウンドマネージャークラス

#include "CGame/CTitleImage/CTitleImage.h"					//タイトルイメージクラス
#include "CGame/CClearImage/CClearImage.h"					//クリアイメージクラス
#include "CGame/CSceneChange/CSceneChange.h"				//シーンチェンジクラス
#include "CGame/CMovieScene/CMovieScene.h"					//ムービーシーン動作クラス
#include "CGame/CCollisionDetection/CCollisionDetection.h"	//当たり判定クラス
//----------------------------------------------------------
#include "CGame/Factory/CEnemyFactory/CEnemyFactory.h"		//エネミーを作るクラス
#include "CGame/Factory/CBossFactory/CBossFactory.h"		//ボスを作るクラス
#include "CGame/Factory/CBulletFactory/CBulletFactory.h"	//バレットを作るクラス
//----------------------------------------------------------この3つはまた別のところで呼び出す
#include "CGameObject/CCharacter/CPlayer/CPlayer.h"			//プレイヤークラス
#include "CGameObject/CCharacter/CEnemy/CEnemy.h"			//エネミークラス
#include "CGameObject/CCharacter/CBoss/CBoss.h"				//ボスクラス
#include "CGameObject/CBullet/CBullet.h"					//バレットクラス

#include "CEnemySet/CEnemySet.h"
#include "CGameObject/CStage/CStageManager/CStageManager.h"		//ステージクラス

#include "CMouseInput//CMouseInput.h"//マウス入力クラス
#include "CWire/CWire.h"	///ワイヤークラス
#include "CGameObject/CCharacter/CWirepoint/CWirepoint.h"	///ワイヤーポイントクラス

#include "NEGA/NEGA.h"
#include "CWireActionSupporter/CWireActionSupporter.h"

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
	GameMain,	//ゲーム中
	Movie,		//ムービーシーン
	BossBattle,	//ボスバトルシーン(カメラの制御などを変更するので専用のシーンを用意する)
	PlayerDeath,//デス中
	Clear,		//ゲームクリアシーン
};

int m_Scene;	//ゲームシーン

//世界の状態	(時を止めたりする)
enum enWorldState
{
	Normal,		//通常
	StopWorld,	//ゲームメインで動作する物を一時的に止める
};

int m_WorldState = enWorldState::Normal;

//プレイヤーがどのマップにいるかを見る
enum enInMap
{
	Map01,
	Map02,
	MapBoss,
};

	int m_PlayerInStage = 0;					//プレイヤーはどのマップにいるか
public:
	bool NoCreateInstance = false;	//作りたくないものは作らない
	bool NoDeleteInstance = false;	//消したくないものは消さない
public:
	int m_MovieSceneCameraMoveCo = 0;;			//ムービーシーンのカメラが動くタイミングをカウント
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

	//インスタンスを破棄する関数
	void DeleteInstance();
	//バレットを削除する関数
	void DeleteBullet();
	//エネミーを削除する関数
	void DeleteEnemy(); 

	//バレットのインスタンスを強制削除
	void DeleteInstance_Bullet();
	//エネミーのインスタンスを強制削除
	void DeleteInstance_Enemy();

	//当たり判定をまとめる関数
	void CollisionUpdate();

	//タイトルからゲームメインに移行する関数
	void TitleToGameMain();

	//ゲームを終了する関数
	void GameEnd();

	//カーソルを動かす関数
	void MoveCursor();

	//タイトルの色々を設定する関数
	void SetTitleInfo();	

	//タイトルの画像関連を描画する関数
	void DrawTitleImg();
private:
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
	//----------------------タイトルイメージ----------------
	std::unique_ptr<CTitleImage> m_upTitleImage;

	//----------------------クリアイメージ------------------
	std::unique_ptr< CClearImage> m_upClearImage;

	//----------------------シーンチェンジクラス-------------
	std::unique_ptr<CSceneChange> m_upSceneChange;

	//----------------------ムービーシーンクラス-------------
	std::unique_ptr<CMovieScene> m_upMovieScene;
	
	//----------------------当たり判定----------------------
	std::unique_ptr<CCollisionDetection> m_upCollisionDetection;	//当たり判定クラス

	//														-ゲームオブジェクト系-
	//----------------------プレイヤー-------------------
	std::unique_ptr<CPlayer> m_upPlayer; 
	std::unique_ptr<CWire >m_pWire;
	std::vector<std::unique_ptr<CWirepoint>> m_pCWirepoint;

	//----------------------エネミー---------------------
	std::vector<std::unique_ptr<CEnemy>> m_upEnemy;		//エネミークラスを継承した敵を作っていく

	//----------------------ボス------------------------
	std::unique_ptr<CBoss> m_upBoss;

	//----------------------バレット---------------------
	std::vector<::std::unique_ptr<CBullet>> m_upBullet;	

	//----------------------ステージ--------------------
	std::unique_ptr<CStageManager> m_upStageManager;


	//----------------------カメラ-----------------------
	std::unique_ptr<CCamera> m_upCamera;

	std::unique_ptr<NEGA> Nega;	//ネガポジ反転クラス
	std::unique_ptr<CWireActionSupporter> m_upWireActionSupporter;	//ワイヤーアクションをサポートするクラス

	//選択肢
	std::vector<std::function<void()>>	m_Action;
	std::vector<VECTOR2_f>	m_CursorPosition;
	int m_CursorAction;	//選択肢のどれを選んでいるか

	bool m_OnePush;		//一回だけ押させる

	bool m_TitleSceneSet;	//タイトルシーンの切り替えタイミングなどのセットを行うか
	int m_GameStartCo;		//ゲームが始まる時間をカウント

	int m_ClearCo;			//ゲームをクリアした時の遷移の時間を図る
};