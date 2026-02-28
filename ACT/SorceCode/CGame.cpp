#include "CGame.h"
#include <time.h>	//time関数を使うための宣言

//コンストラクタ(引数なし)
CGame::CGame()
	:	CGame(nullptr)
{
}

//コンストラクタ(引数あり)
CGame::CGame(GameWindow* pGameWnd)

//↓	Class型の初期化リスト			※順番はCGame.hの順番
: m_Scene(enScene::Title)
, m_pGameWnd(pGameWnd)
, m_hMemDC(nullptr)
, m_MasterWorkDC(nullptr)
, m_MasterWorkBmp(nullptr)
, m_hWorkDC(nullptr)
, m_hWorkBmp(nullptr)
, m_hWorkDC2(nullptr)
, m_hWorkBmp2(nullptr)
, m_hFont(nullptr)
, m_pCollisionDetection(nullptr)
, m_pPlayer(nullptr)
, m_pEnemy(nullptr)
, m_pStage(nullptr)
, m_pCamera(nullptr)
{
}



//デストラクタ
CGame::~CGame()
{

}

//初期化(リセット)関数
void CGame::InitializeGame()
{
}

//構築関数
bool CGame::Create()
{
	//乱数の初期化
	srand((unsigned int)time(nullptr));

	if (NoCreateInstance != true) {
		//メモリDCの作成.
		m_hMemDC = CreateCompatibleDC(nullptr);

		//作業用DCの作成
		m_hWorkDC = CreateCompatibleDC(m_pGameWnd->hScreenDC);		//作業用DCの作成
		m_hWorkBmp = CreateCompatibleBitmap(m_pGameWnd->hScreenDC, WND_W, WND_H);//作業用BITMAPの作成
		SelectObject(m_hWorkDC, m_hWorkBmp);		//DCとBITMAPを紐づけ

		//作業用DC2の作成
		m_hWorkDC2 = CreateCompatibleDC(m_pGameWnd->hScreenDC);						//作業用DC2の作成
		m_hWorkBmp2 = CreateCompatibleBitmap(m_pGameWnd->hScreenDC, WND_W, WND_H);	//作業用BITMAP2の作成
		SelectObject(m_hWorkDC2, m_hWorkBmp2);										//DC2とBITMAP2を紐づけ

		m_MasterWorkDC = CreateCompatibleDC(m_pGameWnd->hScreenDC);						//作業用DCマスターの作成
		m_MasterWorkBmp = CreateCompatibleBitmap(m_pGameWnd->hScreenDC, WND_W, WND_H);	//作業用BITMAPマスターの作成
		SelectObject(m_MasterWorkDC, m_MasterWorkBmp);									//DCマスターとBITMAPマスターを紐づけ
	}

	if (NoCreateInstance != true) {
		//サウンドデータを読み込む処理
		if (CSoundManager::GetInstance()->Load(m_pGameWnd->hWnd) == false)return false;
	}

	if (NoCreateInstance != true) {
		//イメージを読み込む
		if (CImageManager::GetInstance()->Load(m_pGameWnd->hWnd, m_pGameWnd->hScreenDC, m_hMemDC, m_hWorkDC, m_hWorkDC2) == false)return false;
	}

	//当たり判定のインスタンス生成
	m_pCollisionDetection = new CCollisionDetection();
	//当たり判定はインスタンス生成だけで良い

	//-------------------------動的に作って消したい者達-----------------------------
	//プレイヤーのインスタンス生成		プレイヤーはゲームが始まった時に作りたい
	m_pPlayer = new CPlayer();
	m_pPlayer->StartSetting();

	//エネミーのインスタンス生成		ステージごとに配置するようにしたい
	m_pEnemy = new CEnemy();
	m_pEnemy->StartSetting();
	//----------------------------------------------------------------------------

	//ステージのインスタンス生成
	m_pStage = new CStage();
	//マップデータの読み込み
	if (m_pStage->LoadData("Data\\MapData\\Map01.csv") == false)return false;

	if (NoCreateInstance != true) {
		//カメラのインスタンス生成
		m_pCamera = new CCamera();
		//ステージの幅と高さをセットする
		m_pCamera->SetStageSize(m_pStage->GetWidth(), m_pStage->GetHeight());
	}

	//エネミーをポリモーで動かそうとすると、ここのセットクラスはいらないかも
	SetClass();

	return true;
}

//破棄関数
void CGame::Destroy()
{
	//カメラ
	if (NoDeleteInstance != true) {
		SAFE_DELETE(m_pCamera);
	}

	SAFE_DELETE(m_pPlayer);

	SAFE_DELETE(m_pEnemy);

	SAFE_DELETE(m_pStage);

	SAFE_DELETE(m_pCollisionDetection);

	//BITMAPの解放.--------------------------------------------------重要---------------------------

	if (NoDeleteInstance != true) {

		//フォントの解放.
		DeleteObject(m_hFont);

		//作業用BITMAPの解放
		DeleteObject(m_hWorkBmp);
		//作業用DCの解放
		DeleteDC(m_hWorkDC);

		//作業用BITMAP2の解放
		DeleteObject(m_hWorkBmp2);
		//作業用DC2の解放
		DeleteDC(m_hWorkDC2);

		//メモリDCの解放.
		DeleteDC(m_hMemDC);
	}

}

//更新関数(キー入力や動作処理を行う)
void CGame::Update()
{
	m_pCollisionDetection->Update();

	//プレイヤーの動作
	m_pPlayer->Update();

	//エネミーの動作
	m_pEnemy->Update();

	m_pStage->Update();

	//プレイヤーにカメラが付くようにする
	m_pCamera->SetPosition(m_pPlayer->GetCenterPosition());
	m_pCamera->Update();

	//インスタンスを破棄する関数
	DeleteInstance();
}

//描画関数(画像の表示処理を行う)
void CGame::Draw()
{
	//ステージの描画
	m_pStage->Draw(m_pCamera);

	//プレイヤーの描画
	m_pPlayer->Draw(m_pCamera);

	//エネミー描画
	m_pEnemy->Draw(m_pCamera);
}

void CGame::SetClass()
{
	m_pCollisionDetection->SetCharacter(m_pPlayer, m_pEnemy);
}

void CGame::DeleteInstance()
{
}