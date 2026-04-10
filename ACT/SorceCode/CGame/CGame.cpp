#include "CGame.h"

#include <time.h>	//time関数を使うための宣言
#include <iostream>	

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
, m_hMemDC					(nullptr)
, m_MasterWorkDC			(nullptr)
, m_MasterWorkBmp			(nullptr)
, m_hWorkDC					(nullptr)
, m_hWorkBmp				(nullptr)
, m_hWorkDC2				(nullptr)
, m_hWorkBmp2				(nullptr)
, m_hFont					(nullptr)
, m_upCollisionDetection	(nullptr)
, m_upStageManager			(nullptr)
, m_upPlayer				(nullptr)
, m_upCamera				(nullptr)
{
	for (int i = 0; i < m_upEnemy.size(); i++) {
		m_upEnemy[i] = nullptr;
	}
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
	m_upCollisionDetection = std::make_unique<CCollisionDetection>();

	//当たり判定はインスタンス生成だけで良い

	//-------------------------動的に作って消したい者達-----------------------------
	//プレイヤーのインスタンス生成		プレイヤーはゲームが始まった時に作りたい
	m_upPlayer = std::make_unique<CPlayer>();

	m_pWire = std::make_unique<CWire>();


	//エネミーのインスタンス生成
	//エネミーを作るタイミングで良い

	//エネミーを作っている
	VECTOR2_f SetEnemy = { 200,200 };
	m_upEnemy.push_back(CEnemyFactory::CreateKedama(CKedama::enColor::Blue, SetEnemy));
	
	//----------------------------------------------------------------------------

	//ステージマネージャーのインスタンス生成
	m_upStageManager = std::make_unique<CStageManager>();
	m_upStageManager->Create();

	if (NoCreateInstance != true) {
		//カメラのインスタンス生成
		m_upCamera = std::make_unique<CCamera>();
		//ステージの幅と高さをセットする<w.h>
		std::pair<float, float> MapSize = m_upStageManager->GetMapSize();
		m_upCamera->SetStageSize(MapSize.first,MapSize.second);
	}

	//エネミーをポリモーで動かそうとすると、ここのセットクラスはいらないかも
	SetClass();


	//初期設定
	CMouseInput::InitialSettings(m_pGameWnd->hWnd); 
	m_pCWirepoint.push_back(std::make_unique<CWirepoint>(VECTOR2_f{ 100, 400 }));
	m_pCWirepoint.push_back(std::make_unique<CWirepoint>(VECTOR2_f{ 500, 400 }));
	m_pCWirepoint.push_back(std::make_unique<CWirepoint>(VECTOR2_f{ 900, 400 }));
	m_pCWirepoint.push_back(std::make_unique<CWirepoint>(VECTOR2_f{ 1500, 100 }));


	Nega =std::make_unique<NEGA>(); 
	m_upWireActionSupporter = std::make_unique<CWireActionSupporter>();
	return true;
}

//破棄関数
void CGame::Destroy()
{

	//stdの破棄用の関数をまた追加する

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
	//仮置き
	CMouseInput::Update();


	m_pWire->Update();

	//プレイヤーの動作
	//ワイヤーを撃てるかセット
	m_upPlayer->SetWireShotCan(m_pWire->canShot());
	m_upPlayer->WireShotStato(m_pWire->GetplayWire());
	m_upPlayer->Update();

	//当たり判定
	bool test = m_upStageManager->IsHit(*m_upPlayer);

	//エネミーの動作
	//ある分回す
	for (int i = 0; i < m_upEnemy.size(); i++) {
		m_upEnemy[i]->Update();
	}

	for (int i = 0; i < m_pCWirepoint.size(); i++) {
		m_pCWirepoint[i]->Update();
	}
	m_upWireActionSupporter->Update();

	//マウスとエネミーの当たり判定処理
	m_upCollisionDetection->MouseToEnemyCollision(m_upEnemy, m_upCamera);

	//プレイヤーとエネミーの当たり判定処理
	m_upCollisionDetection->PlayerToEnemyCollision(m_upPlayer, m_upEnemy);

	//ワイヤーとワイヤーポイントの当たり判定処理
	m_upCollisionDetection->WireToWirepointCollision(m_pCWirepoint, m_pWire);

	if (m_pWire->GetRock()) {
		m_upWireActionSupporter->StartWireAction(m_upPlayer.get(), m_pWire.get(), m_pWire->GetCatchPoint());
	}

	//ステージの更新
	m_upStageManager->Update();


	//ワイヤーとワイヤーポイント
	Collision();



	//プレイヤーにカメラが付くようにする
	m_upCamera->SetPosition(m_upPlayer->GetCenterPosition());
	m_upCamera->Update();
	//ワイヤーを撃つ処理
	if (m_upPlayer->GetWireShot()) {
		m_pWire->Shot(m_upPlayer, CMouseInput::GetMousePosCamera(m_upCamera));
	}

	//インスタンスを破棄する関数
	DeleteInstance();
}

//描画関数(画像の表示処理を行う)
void CGame::Draw()
{

	//ステージの描画
	m_upStageManager->Draw(m_upCamera);
	//ワイヤーの描画
	m_pWire->Draw(m_upCamera);

	//プレイヤーの描画
	m_upPlayer->Draw(m_upCamera);

	//エネミー描画
	for (int i = 0; i < m_upEnemy.size(); i++) {
		m_upEnemy[i]->Draw(m_upCamera);
	}

	for (int i = 0; i < m_pCWirepoint.size(); i++) {
		m_pCWirepoint[i]->Draw(m_upCamera);
	}
	if (CMouseInput::GetMouseLeft(true,false)) {
		//
		Nega->Draw(m_pGameWnd->hScreenDC);
	
	}
	
	//仮置き
	CMouseInput::Draw();
}

void CGame::Collision()
{
	
}

void CGame::SetClass()
{
}

void CGame::DeleteInstance()
{
}