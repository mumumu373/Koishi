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
, m_upBoss					(nullptr)
, m_Action					()
, m_CursorAction			()
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

		CStageCollisionDraw::GetInstance()->SetScreenDC(m_pGameWnd->hScreenDC);	//当たり判定の描画クラスにスクリーンDCをセットする
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

	//ボスを作る
	m_upBoss = CBossFactory::CreateNazrin();

	//エネミーのインスタンス生成
	//エネミーを作るタイミングで良い

	//エネミーを作っている
	VECTOR2_f SetEnemy = { 600,400 };
	VECTOR2_f Speed = { 4,4 };
	m_upEnemy.push_back(CEnemyFactory::CreateKedama(CKedama::enColor::Blue, SetEnemy,60, 5, 30, 120));
	SetEnemy.y += 500;																//作るときにムーブタイプを決めておく
	m_upEnemy.push_back(CEnemyFactory::CreateFairy(CFairy::enColor::NoColor, SetEnemy,80, Speed, CFairy::enMoveType::Rotation, 60, 120));
	SetEnemy.x += 600;
	m_upEnemy.push_back(CEnemyFactory::CreateYinYangBall(CYinYangBall::enColor::Blue, SetEnemy,100));

	//ゲームシーン状態にしておく
	m_Scene = enScene::Title;
	//----------------------------------------------------------------------------


	//ステージマネージャーのインスタンス生成
	m_upStageManager = std::make_unique<CStageManager>();
	m_upStageManager->Create();

	CEnemySet::LoadEnemies(m_upEnemy);

	if (NoCreateInstance != true) {
		//カメラのインスタンス生成
		m_upCamera = std::make_unique<CCamera>();
		//ステージの幅と高さをセットする<w.h>
		std::pair<float, float> MapSize = m_upStageManager->GetMapSize();
		m_upCamera->SetStageSize(MapSize.first,MapSize.second);
		m_upPlayer->SetStegeUnder(MapSize.second);//プレイヤーにステージの下の位置を教える
	}

	//初期設定
	CMouseInput::InitialSettings(m_pGameWnd->hWnd); 
	m_pCWirepoint.push_back(std::make_unique<CWirepoint>(VECTOR2_f{ 300, 800 }));
	m_pCWirepoint.push_back(std::make_unique<CWirepoint>(VECTOR2_f{ 600, 700 }));
	m_pCWirepoint.push_back(std::make_unique<CWirepoint>(VECTOR2_f{ 900, 600 }));
	m_pCWirepoint.push_back(std::make_unique<CWirepoint>(VECTOR2_f{ 1500, 800 }));
	m_pCWirepoint.push_back(std::make_unique<CWirepoint>(VECTOR2_f{ 2000, 600 }));
	m_pCWirepoint.push_back(std::make_unique<CWirepoint>(VECTOR2_f{ 2500, 400 }));


	Nega =std::make_unique<NEGA>(); 
	m_upWireActionSupporter = std::make_unique<CWireActionSupporter>();

	m_upPlayer->SetCamera(m_upCamera.get());

	//タイトルの色々を設定する関数
	SetTitleInfo();

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

	//仮置き
	CMouseInput::Update();
	switch (m_Scene) {
	case enScene::Title:

		MoveCursor();

		if(GetAsyncKeyState(VK_RETURN) & 0x8000) 
		{
			m_Action[m_CursorAction]();
		}
		break;


	case enScene::GameMain:

		m_pWire->Update();

		//プレイヤーの動作
		//ワイヤーを撃てるかセット
		m_upPlayer->SetWireShotCan(m_pWire->canShot());
		m_upPlayer->WireShotStato(m_pWire->GetplayWire());

		//プレイヤーの動作
		m_upPlayer->Update(m_upBullet);

		//エネミーの動作
		//ある分回す
		for (int i = 0; i < m_upEnemy.size(); i++) {
			//生存中になっていれば動かす
			if (m_upEnemy[i]->m_State == CEnemy::enState::Living) {
				//プレイヤーの位置を取得する
				m_upEnemy[i]->SetPlayerPos(m_upPlayer->GetCenterPosition());

				//キャッチされていなければ
				if (m_upEnemy[i]->GetCatchWire() != CEnemy::enCatchWire::Catch) {
					m_upEnemy[i]->Update(m_upBullet);
				}
				else {
					//投げられたら
					if (m_upEnemy[i]->EnemyThrown == true) {
						m_upEnemy[i]->ThrowEnemy();
					}
				}
			}
		}

		//バレットの動作
		for (int i = 0; i < m_upBullet.size(); i++) {
			m_upBullet[i]->Update();
		}

		for (int i = 0; i < m_pCWirepoint.size(); i++) {
			m_pCWirepoint[i]->Update();
		}
		m_upWireActionSupporter->Update();
		
		//当たり判定をまとめた関数
		CollisionUpdate();

		//ステージの更新
		m_upStageManager->Update();

		//プレイヤーにカメラが付くようにする
		m_upCamera->SetPosition(m_upPlayer->GetCenterPosition());
		m_upCamera->Update();

		//ワイヤーを撃つ処理
		if (m_upPlayer->GetWireShot()) {
			m_pWire->Shot(m_upPlayer, CMouseInput::GetMousePosCamera(m_upCamera.get()));
		}

		//プレイヤーがイベントブロックに触れたなら
		if (m_upPlayer->EVENT_HIT == true) {
			//ムービーシーン(ボスとの会話などの)
			m_Scene = enScene::Movie;

			//ボスをプレイヤーの場所を参照して登場させる
			m_upBoss->BossBattleFlag(m_upPlayer->GetPosition());

			//イベントが開始した位置を入れる
			m_upPlayer->EVENT_START_POS = m_upPlayer->GetPosition();
		}

		break;
	case enScene::Movie:

		//プレイヤーのムービーシーン中の動作
		m_upPlayer->MovieSceneUpdate();

		//ボスのムービーシーン中の動作
		m_upBoss->MovieSceneUpdate();

		//プレイヤーが地面についてからうごかしたい
		if (m_MovieSceneCameraMoveCo >= 20 && m_upPlayer->GroundStand == true) {
			//ムービーシーン
			m_upCamera->Update();

			//ナズーリン用のカメラを用意する
			if (m_upBoss->m_MyCharacter == CBoss::enMyCharacter::Nazrin) {
				//カメラをボス戦用にセットする
				m_upCamera->SetBossBattleCamera_Nazrin(m_upPlayer->GetPosition());
			}

			//エンターキーまたはマウスで進行できる
			if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
				//ボスバトルに移行
				m_Scene = enScene::BossBattle;

				m_MovieSceneCameraMoveCo = 0;

				//ボス用のステージに変更する
				m_upStageManager->ChangeStage(CStageManager::enStage::MapBoss);

				//カメラの場所を変更したところと同じ位置になるようにする
				m_upCamera->SetChangeBossStageCamera(m_upPlayer->GetPositionadd(), m_upBoss->GetPositionadd());

				//ステージの幅と高さをセットする<w.h>
				std::pair<float, float> MapSize = m_upStageManager->GetMapSize();
				m_upCamera->SetStageSize(MapSize.first, MapSize.second);
			}
		}
		else {
			m_MovieSceneCameraMoveCo++;
		}
		break;
	case enScene::BossBattle:

		//カメラの動作
		m_upCamera->Update();

		//ワイヤーの動作
		m_pWire->Update();

		//ワイヤーを撃てるかセット
		m_upPlayer->SetWireShotCan(m_pWire->canShot());
		m_upPlayer->WireShotStato(m_pWire->GetplayWire());

		//プレイヤーの動作
		m_upPlayer->Update(m_upBullet);
		//カメラとの当たり判定
		m_upPlayer->CameraCollision(m_upCamera->GetCameraPos(), 44, 44);


		//ボスの動作	ボスバトルのときだけ動作する
		if (m_upBoss != nullptr) {
			//プレイヤーの位置を取得する
			m_upBoss->SetPlayerPos(m_upPlayer->GetCenterPosition());
			//カメラの位置を取得する
			m_upBoss->SetCameraPos(m_upCamera->GetCameraPos());
			//ボスの動作
			m_upBoss->Update(m_upBullet);

			//ボスのフェーズが2になったら
			if (m_upBoss->m_BossPhase == CBoss::enBossPhase::Phase_2) {
				//死亡状態になったらスクロールを止める
				if (m_upBoss->m_State <= CBoss::enState::Dying) {
					//強制スクロールにする
					m_upCamera->BossPhase2Camera();
				}
			}
		}

		//エネミーの動作
		//ある分回す
		for (int i = 0; i < m_upEnemy.size(); i++) {
			//生存中になっていれば動かす
			if (m_upEnemy[i]->m_State == CEnemy::enState::Living) {
				//プレイヤーの位置を取得する
				m_upEnemy[i]->SetPlayerPos(m_upPlayer->GetCenterPosition());

				//キャッチされていなければ
				if (m_upEnemy[i]->GetCatchWire() != CEnemy::enCatchWire::Catch) {
					m_upEnemy[i]->Update(m_upBullet);
				}
				else {
					//投げられたら
					if (m_upEnemy[i]->EnemyThrown == true) {
						m_upEnemy[i]->ThrowEnemy();
					}
				}
			}
		}

		//バレットの動作
		for (int i = 0; i < m_upBullet.size(); i++) {
			m_upBullet[i]->Update();
		}

		//ワイヤーポイントの動作
		for (int i = 0; i < m_pCWirepoint.size(); i++) {
			m_pCWirepoint[i]->Update();
		}
		//ワイヤーのアクションの動作?
		m_upWireActionSupporter->Update();

		//当たり判定の関数
		CollisionUpdate();

		//ワイヤーを撃つ処理
		if (m_upPlayer->GetWireShot()) {
			m_pWire->Shot(m_upPlayer, CMouseInput::GetMousePosCamera(m_upCamera.get()));
		}

		break;
	}
	//インスタンスを破棄する関数
	DeleteInstance();
}

//描画関数(画像の表示処理を行う)
void CGame::Draw()
{
	VECTOR2_f Senter ;
	switch (m_Scene)
	{
	case enScene::Title:
		
		CImageManager::SelectImg(CImageManager::enImgList::IMG_Title)->BBlt(
			0, 0,
			1280, 720,
			0, 0);

		CImageManager::SelectImg(CImageManager::enImgList::IMG_Cursor)->TransAlBlend(
			m_CursorPosition[m_CursorAction].x, m_CursorPosition[m_CursorAction].y,
			128, 128,
			0, 0,
			255);

		break;
	case enScene::GameMain:
	case enScene::Movie:
	case enScene::BossBattle:
		
		//ステージの描画
		m_upStageManager->Draw(m_upCamera);
		//ワイヤーの描画
		m_pWire->Draw(m_upCamera);


		 Senter =m_pWire->GetTopPoint();
		Senter.x += m_pWire->GetSize() / 2;
		Senter.y += m_pWire->GetSize() / 2;
		//プレイヤーの描画
		m_upPlayer->SetWireTopPos(Senter);
		m_upPlayer->Draw(m_upCamera);
		//ワイヤーの手の描画
		m_pWire->WireHandDraw(m_upCamera);

		//エネミー描画
		for (int i = 0; i < m_upEnemy.size(); i++) {
			//エネミーが死亡中になっていなければ
			if (m_upEnemy[i]->m_State != CEnemy::enState::Dying) {
				m_upEnemy[i]->Draw(m_upCamera);
			}
			else {
				//死んだとき用の描画にする
				m_upEnemy[i]->DeadAnimationDraw(m_upCamera);
			}
		}

		//ボスの描画
		if (m_upBoss != nullptr) {
			m_upBoss->Draw(m_upCamera);
		}

		//バレットの描画
		for (int i = 0; i < m_upBullet.size(); i++) {
			m_upBullet[i]->Draw(m_upCamera);
		}

		//プレイヤーのハートを描画する
		m_upPlayer->PlayerHeartDraw();

		for (int i = 0; i < m_pCWirepoint.size(); i++) {
			m_pCWirepoint[i]->Draw(m_upCamera);
		}
		if (CMouseInput::GetMouseLeft(true, false)) {
			//
			//Nega->Draw(m_pGameWnd->hScreenDC);
			int a = 0;
		}
		//m_upPlayer->DrawH(m_pGameWnd->hScreenDC, m_pGameWnd->hWnd, m_upCamera);
		//仮置き
		CMouseInput::Draw();

		break;

	default:
		break;
	}
}

void CGame::DeleteInstance()
{
	//バレットを消す処理を行う
	DeleteBullet();
	//エネミーを消す処理を行う
	DeleteEnemy();
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

void CGame::DeleteEnemy()
{
	//バレットと同じようにエネミーを消す
	for (int i = 0; i < m_upEnemy.size(); i++) {
		//エネミーが死んだら
		if (m_upEnemy[i]->m_State == CEnemy::enState::Dead) {
			m_upEnemy.erase(
				std::remove_if(m_upEnemy.begin(), m_upEnemy.end(),
					[](const std::unique_ptr<CEnemy>& Enemy) {
						return Enemy->m_State == CEnemy::enState::Dead;
					}),
				m_upEnemy.end()
			);
		}
	}
}

void CGame::CollisionUpdate()
{
	//マウスとエネミーの当たり判定処理
	m_upCollisionDetection->MouseToEnemyCollision(m_upEnemy, m_upCamera);
	//マウスとワイヤーポイント
	m_upCollisionDetection->MouseToWirePoint(m_pCWirepoint, m_upCamera);

	//プレイヤーとエネミーの当たり判定処理
	m_upCollisionDetection->PlayerToEnemyCollision(m_upPlayer, m_upEnemy);

	//ワイヤーと敵	
	m_upCollisionDetection->WireToEnemyCollision(m_upEnemy, m_pWire, m_upPlayer, m_upWireActionSupporter);

	//プレイヤーとバレットの当たり判定処理
	m_upCollisionDetection->PlayerToBulletCollision(m_upPlayer, m_upBullet);

	//ワイヤーとワイヤーポイントの当たり判定処理
	m_upCollisionDetection->WireToWirepointCollision(m_pCWirepoint, m_pWire, m_upPlayer, m_upWireActionSupporter);

	//プレイヤーのアタックとエネミーの当たり判定
	m_upCollisionDetection->PlayerAttackToEnemyCollision(m_upPlayer->GetNormalAttack_p(), m_upEnemy);

	//ボスバトルになったら判定するもの
	if (m_Scene == enScene::BossBattle) {
		//プレイヤーとボスの当たり判定
		m_upCollisionDetection->PlayerToBossCollision(m_upPlayer, m_upBoss);

		//プレイヤーのアタックとボスの当たり判定	これはボス戦のみ動かす
		m_upCollisionDetection->PlayerAttackToBossCollision(m_upPlayer->GetNormalAttack_p(), m_upBoss);

		//飛ばされたエネミーとボスの当たり判定
		m_upCollisionDetection->BossToEnemyCollision(m_upBoss, m_upEnemy);
	}
}

//---------タイトルの選択肢関数------------

//タイトルからゲームメインに移行する関数
void CGame::TitleToGameMain()
{
	m_Scene = enScene::GameMain;
}

//ゲームを終了する関数
void CGame::GameEnd()
{
	PostMessage(m_pGameWnd->hWnd, WM_CLOSE, 0, 0);
}

//カーソルを動かす関数
void CGame::MoveCursor()
{
	if (GetAsyncKeyState(VK_UP) & 0x0001)
	{
		m_CursorAction--;
		if (m_CursorAction < 0) m_CursorAction = m_Action.size() - 1;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x0001)
	{
		m_CursorAction++;
		if (m_CursorAction >= m_Action.size()) m_CursorAction = 0;
	}
}

void CGame::SetTitleInfo()
{
	//タイトルでの選択肢
	m_Action =
	{
		[this]() { TitleToGameMain(); },
		[this]() { GameEnd(); }
	};

	//タイトルでのカーソルの位置
	m_CursorPosition.push_back({ 400,300 });
	m_CursorPosition.push_back({ 400,500 });

}
//----------------------------------------
