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
, m_upCollisionDetection(nullptr)
, m_upPlayer(nullptr)
, m_upBoss(nullptr)
, m_upStage(nullptr)
, m_upCamera(nullptr)
{
	for (int i = 0; i < m_upEnemy.size(); i++) {
		m_upEnemy[i] = nullptr;
	}
	for (int i = 0; i < m_upBullet.size(); i++) {
		m_upBullet[i] = nullptr;
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

	//ボスを作る
	m_upBoss = CBossFactory::CreateNazrin();

	//エネミーのインスタンス生成
	//エネミーを作るタイミングで良い

	//エネミーを作っている
	VECTOR2_f SetEnemy = { 200,400 };
	m_upEnemy.push_back(CEnemyFactory::CreateKedama(CKedama::enColor::Blue, SetEnemy));
	SetEnemy.y -= 200;
	m_upEnemy.push_back(CEnemyFactory::CreateFairy(CFairy::enColor::NoColor, SetEnemy));
	SetEnemy.x += 300;
	m_upEnemy.push_back(CEnemyFactory::CreateYinYangBall(CYinYangBall::enColor::Blue, SetEnemy));
	//----------------------------------------------------------------------------

	m_upEnemy.push_back(CEnemySet::otamesi());

	//ステージのインスタンス生成
	m_upStage = std::make_unique<CStage>();
	//マップデータの読み込み
	if (m_upStage->LoadData("Data\\MapData\\Map01.csv") == false)return false;

	if (NoCreateInstance != true) {
		//カメラのインスタンス生成
		m_upCamera = std::make_unique<CCamera>();
		//ステージの幅と高さをセットする
		m_upCamera->SetStageSize(m_upStage->GetWidth(), m_upStage->GetHeight());
	}

	//エネミーをポリモーで動かそうとすると、ここのセットクラスはいらないかも
	SetClass();

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
	CSoundManager::PlayLoop(CSoundManager::enSingleSoundList::BGM_Stage1);

	//プレイヤーの動作
	m_upPlayer->Update(m_upBullet);

	//エネミーの動作
	//ある分回す
	for (int i = 0; i < m_upEnemy.size(); i++) {
		m_upEnemy[i]->Update(m_upBullet);
	}

	//ボスの動作
	if (m_upBoss != nullptr) {
		m_upBoss->Update(m_upBullet);
	}

	//バレットの動作
	for (int i = 0; i < m_upBullet.size(); i++) {
		m_upBullet[i]->Update();
	}

	//プレイヤーとエネミーの当たり判定処理
	m_upCollisionDetection->PlayerToEnemyCollision(m_upPlayer, m_upEnemy);

	m_upStage->Update();

	//バレットを消す処理を行う
	DeleteBullet();

	//プレイヤーにカメラが付くようにする
	m_upCamera->SetPosition(m_upPlayer->GetCenterPosition());
	m_upCamera->Update();

	//インスタンスを破棄する関数
	DeleteInstance();
}

//描画関数(画像の表示処理を行う)
void CGame::Draw()
{
	//ステージの描画
	m_upStage->Draw(m_upCamera);

	//プレイヤーの描画
	m_upPlayer->Draw(m_upCamera);

	//エネミー描画
	for (int i = 0; i < m_upEnemy.size(); i++) {
		m_upEnemy[i]->Draw(m_upCamera);
	}

	//ボスの描画
	if (m_upBoss != nullptr) {
		m_upBoss->Draw(m_upCamera);
	}

	//バレットの描画
	for (int i = 0; i < m_upBullet.size(); i++) {
		m_upBullet[i]->Draw(m_upCamera);
	}
}

void CGame::SetClass()
{
}

void CGame::DeleteInstance()
{
}

void CGame::DeleteBullet()
{
	//バレットのインスタンスを消す
	for (int i = 0; i < m_upBullet.size(); i++) {
		//バレットが死んだら
		if (m_upBullet[i]->m_State == CBullet::enState::Dead) {
			//erase は、ここから後ろの物はすべて消しますというものとなる	引数に remove_if があるということは？
			m_upBullet.erase(
				//remove_ifだけでは削除はしない	begin と end はそれぞれ、先頭、末尾を見てください。というものになる
				std::remove_if(m_upBullet.begin(), m_upBullet.end(),
					//[]->ラムダ式を開始します ()
					[](const std::unique_ptr<CBullet>& Bullet) {
						//この場合の条件は、バレットがデッド状態なら末尾に追いやる
						return Bullet->m_State == CBullet::enState::Dead;
					}),
				m_upBullet.end()
			);
		}
	}
}
