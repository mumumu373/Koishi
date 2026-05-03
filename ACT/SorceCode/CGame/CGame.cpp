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
, m_upTitleImage			(nullptr)
, m_upClearImage			(nullptr)
, m_upSceneChange			(nullptr)
, m_upMovieScene			(nullptr)
, m_upCollisionDetection	(nullptr)
, m_upStageManager			(nullptr)
, m_upPlayer				(nullptr)
, m_pWire					(nullptr)
, m_upCamera				(nullptr)
, Nega						(nullptr)
, m_upWireActionSupporter	(nullptr)
, m_upBoss					(nullptr)
, m_Action					()
, m_CursorAction			()
, m_OnePush					(false)
, m_TitleSceneSet			(false)
, m_GameStartCo				(0)
, m_ClearCo					(0)
, TitleStartSound			(false)
, TitleStartSoundCo			(0)
, TitleBGMSwitch			(false)
, TitleBGMSwitchCo			(0)
, StageBGMSwitch			(false)
, StageBGMSwitchCo			(0)
, BossBGMSwitch				(false)
, BossBGMSwitchCo			(0)
{
	for (int i = 0; i < m_upEnemy.size(); i++) {
		m_upEnemy[i] = nullptr;
	}
	for (int i = 0; i < m_upBullet.size(); i++) {
		m_upBullet[i] = nullptr;
	}
	for (int i = 0; i < m_upCWirepoint.size(); i++) {
		m_upCWirepoint[i] = nullptr;
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

	//タイトルイメージのインスタンス生成
	m_upTitleImage = std::make_unique<CTitleImage>();

	//クリアイメージのインスタンス生成
	m_upClearImage = std::make_unique<CClearImage>();

	//シーンチェンジのインスタンス生成
	m_upSceneChange = std::make_unique<CSceneChange>();

	//ムービーシーンのインスタンス生成
	m_upMovieScene = std::make_unique<CMovieScene>();

	//当たり判定のインスタンス生成
	m_upCollisionDetection = std::make_unique<CCollisionDetection>();

	//当たり判定はインスタンス生成だけで良い

	//-------------------------動的に作って消したい者達-----------------------------
	//プレイヤーのインスタンス生成		プレイヤーはゲームが始まった時に作りたい
	m_upPlayer = std::make_unique<CPlayer>();
	
	m_pWire = std::make_unique<CWire>();

	//ボスを作る
	m_upBoss = CBossFactory::CreateNazrin();

	//ゲームシーン状態にしておく
	m_Scene = enScene::Title;
	//----------------------------------------------------------------------------


	//ステージマネージャーのインスタンス生成
	m_upStageManager = std::make_unique<CStageManager>();
	m_upStageManager->Create();

	//初期状態でマップ1から始まる
	m_PlayerInStage = enInMap::Map01;

	//カメラのインスタンス生成
	m_upCamera = std::make_unique<CCamera>();
	//ステージの幅と高さをセットする<w.h>
	std::pair<float, float> MapSize = m_upStageManager->GetMapSize();
	m_upCamera->SetStageSize(MapSize.first,MapSize.second);
	m_upPlayer->SetStegeUnder(MapSize.second);//プレイヤーにステージの下の位置を教える

	//初期設定
	CMouseInput::InitialSettings(m_pGameWnd->hWnd); 

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
	//----------------------タイトルイメージ----------------
	m_upTitleImage.reset();

	//----------------------クリアイメージ------------------
	m_upClearImage.reset();

	//----------------------シーンチェンジクラス------------
	m_upSceneChange.reset();

	//----------------------ムービーシーンクラス-------------
	m_upMovieScene.reset();

	//----------------------当たり判定----------------------
	m_upCollisionDetection.reset();

	//														-ゲームオブジェクト系-
	//----------------------プレイヤー-------------------
	m_upPlayer.reset();
	m_pWire.reset();
	m_upCWirepoint.clear();

	//----------------------エネミー---------------------
	m_upEnemy.clear();

	//----------------------ボス------------------------
	m_upBoss.reset();

	//----------------------バレット---------------------
	m_upBullet.clear();

	//----------------------ステージ--------------------
	m_upStageManager.reset();


	//----------------------カメラ-----------------------
	m_upCamera.reset();

	Nega.reset();	//ネガポジ反転クラス
	m_upWireActionSupporter.reset();	//ワイヤーアクションをサポートするクラス

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

	m_upSceneChange->Update();

	switch (m_Scene) {
	case enScene::Title:

		//タイトルサウンドの動作
		TitleSoundUpdate();

		MoveCursor();

		m_upTitleImage->Update();

		//タイトルシーンの動作を開始したら
		if (m_TitleSceneSet == true) {
			//ゲームが始まるまでの時間になったら
			if (m_GameStartCo >= 60) {
				//シーンチェンジが始まっていないなら
				if (m_upSceneChange->SceneChangeStart == false) {
					//シーンチェンジ
					m_upSceneChange->SetSceneChangeType(CSceneChange::enSceneType::Right, 50, 20, false);
				}
			}

			//ジャンプする動作に切り替える
			m_upPlayer->TitleSceneUpdate();

			m_GameStartCo++;
		}
		else {
			m_upPlayer->TitleSceneUpdate(m_upTitleImage->GetPosition());
		}

		if(GetAsyncKeyState(VK_RETURN) & 0x8000 || CMouseInput::GetMouseLeft(true, true) || CMouseInput::GetMouseRight(true, true) || CMouseInput::GetMouseWheel(true, true))
		{
			if (m_OnePush == false) {
				//タイトルの準備が完了していないなら
				if (m_upTitleImage->SetCompleted == false) {
					//規定の場所にセットする
					m_upTitleImage->SetInitializePos();
				}
				//タイトル準備が完了してから
				else {
					//タイトルシーンの動作を始める
					if (m_TitleSceneSet == false) {
						m_TitleSceneSet = true;
						m_GameStartCo = 0;

						//タイトルシーンのアップデートをするときの準備
						m_upPlayer->TitleSceneSet();

						CSoundManager::Stop(CSoundManager::enSingleSoundList::BGM_TitleStart);
						//BGM1をストップ
						CSoundManager::Stop(CSoundManager::enSingleSoundList::BGM_Title_1);
						//BGM2をストップ
						CSoundManager::Stop(CSoundManager::enSingleSoundList::BGM_Title_2);

						//決定音を鳴らす
						CSoundManager::PlaySE_NoDuplication(CSoundManager::enSingleSoundList::SE_Decision);
					}
				}

				//一回だけ押させる
				m_OnePush = true;
			}
		}
		else {
			//離すとリセット
			m_OnePush = false;
		}

		//シーンが完全に覆いかぶさってから
		if (m_upSceneChange->SceneSetComp == true) {
			//ステージへ
			m_Action[m_CursorAction]();

			//ステージ1のエネミーをセットする
			CEnemySet::LoadEnemies_Stage1(m_upEnemy);
			//ワイヤーポイント
			CWirePointSet::LoadWirePoints_Stage1(m_upCWirepoint);

			//プレイヤーを初期化
			m_upPlayer->Initialization();

			//ステージでのプレイヤーの配置
			m_upPlayer->SetStagePos({ 100, 500 });

			//タイトルサウンドの変数を初期化
			InitializeTitleSound();
			//曲のはじめから流すように
			CSoundManager::SingleSoundSeekToStart(CSoundManager::enSingleSoundList::BGM_TitleStart);
			CSoundManager::SingleSoundSeekToStart(CSoundManager::enSingleSoundList::BGM_Title_1);
			CSoundManager::SingleSoundSeekToStart(CSoundManager::enSingleSoundList::BGM_Title_2);

			//タイトルシーンの動作を完了
			m_TitleSceneSet = false;
		}
		break;

	case enScene::GameMain:

		//ステージサウンドの動作
		StageMainSoundUpdate();

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
			if (m_upEnemy[i] != nullptr) {
				//生存中になっていれば動かす
				if (m_upEnemy[i]->m_State == CEnemy::enState::Living) {
					//プレイヤーの位置を取得する
					m_upEnemy[i]->SetPlayerPos(m_upPlayer->GetCenterPosition());

					//キャッチされていなければ
					if (m_upEnemy[i]->GetCatchWire() != CEnemy::enCatchWire::Catch) {
						//カメラを基準とした範囲内にエネミーが存在しているなら
						if (m_upEnemy[i]->GetCenterPosition().x <= m_upCamera->GetCameraPos().x + (WND_W * 2)) {
							if (m_upEnemy[i]->GetCenterPosition().x >= m_upCamera->GetCameraPos().x - (WND_W * 2)) {
								//横の距離だけ見て判断
								m_upEnemy[i]->Update(m_upBullet);
							}
						}
					}
					else {
						//投げられたら
						if (m_upEnemy[i]->EnemyThrown == true) {
							m_upEnemy[i]->ThrowEnemy();
						}
					}
				}
			}
		}

		//バレットの動作
		for (int i = 0; i < m_upBullet.size(); i++) {
			if (m_upBullet[i] != nullptr) {
				m_upBullet[i]->Update();
			}
		}

		for (int i = 0; i < m_upCWirepoint.size(); i++) {
			if (m_upCWirepoint[i] != nullptr) {
				m_upCWirepoint[i]->Update();
			}
		}
		m_upWireActionSupporter->Update(m_upCamera->GetCameraPos());

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

		//プレイヤーがステージチェンジブロックに触れたなら
		if (m_upPlayer->STAGE_CHANGE_HIT == true) {
			if (m_upPlayer->StageChangeTime == false) {
				//ステージチェンジの遷移を行う
				m_upSceneChange->SetSceneChangeType(CSceneChange::enSceneType::Left, 40, 40, false);

				//ステージチェンジ中
				m_upPlayer->StageChangeTime = true;
			}
		}
		//プレイヤーがイベントブロックに触れたなら
		else if (m_upPlayer->EVENT_HIT == true) {
			//ムービーシーン(ボスとの会話などの)
			m_Scene = enScene::Movie;

			//ボスをプレイヤーの場所を参照して登場させる
			m_upBoss->BossBattleFlag(m_upPlayer->GetPosition());

			//イベントが開始した位置を入れる
			m_upPlayer->EVENT_START_POS = m_upPlayer->GetPosition();
		
		}

		//ステージチェンジ中なら
		if (m_upPlayer->StageChangeTime == true) {
			//シーンが完全に覆いかぶさってから
			if (m_upSceneChange->SceneSetComp == true) {
				//ステージ2に移行
				m_upStageManager->ChangeStage(CStageManager::enStage::Map02);

				//ステージ切り替え時のプレイヤーの配置
				m_upPlayer->SetStagePos({ 100,200 });

				//現在存在するバレットとエネミーとワイヤーポイントのインスタンスをすべて空ける
				DeleteInstance_Bullet();
				DeleteInstance_Enemy();
				DeleteInstance_WirePoint();

				//ステージ2の時のエネミーを配置する
				CEnemySet::LoadEnemies_Stage2(m_upEnemy);
				//ワイヤーポイント
				CWirePointSet::LoadWirePoints_Stage2(m_upCWirepoint);

				//プレイヤーがいるところを更新
				m_PlayerInStage = enInMap::Map02;

				//ステージの幅と高さをセットする<w.h>
				std::pair<float, float> MapSize = m_upStageManager->GetMapSize();
				m_upCamera->SetStageSize(MapSize.first, MapSize.second);
				m_upPlayer->SetStegeUnder(MapSize.second);//プレイヤーにステージの下の位置を教える

				//ステージチェンジ終了
				m_upPlayer->StageChangeTime = false;
			}
		}

		break;
	case enScene::Movie:
		//ステージサウンドの動作
		StageMainSoundUpdate();

		m_upWireActionSupporter->WireEnd();
		//ワイヤーの動作
		m_pWire->Update();

		//プレイヤーのムービーシーン中の動作
		m_upPlayer->MovieSceneUpdate();

		//ボスのムービーシーン中の動作
		m_upBoss->MovieSceneUpdate();

		//プレイヤーが地面についてからうごかしたい
		if (m_MovieSceneCameraMoveCo >= 20 && m_upPlayer->GroundStand == true) {
			//ボスとプレイヤーの位置を渡す
			m_upMovieScene->SetPlayerPos(m_upPlayer->GetPosition());
			m_upMovieScene->SetBossPos(m_upBoss->GetPosition());

			//ムービーシーンをスタートさせる
			if (m_upMovieScene->StartMovie == false) {
				//ボスのムービーシーンスタート
				m_upMovieScene->BossMovieSceneStart();
			}
			//ムービーシーンが始まったら
			else {
				//ムービーシーン(ボス用ムービー)の動作
				m_upMovieScene->BossMovieSceneUpdate();
			}
			//ムービーシーン
			m_upCamera->Update();

			//ナズーリン用のカメラを用意する
			if (m_upBoss->m_MyCharacter == CBoss::enMyCharacter::Nazrin) {
				//カメラをボス戦用にセットする
				m_upCamera->SetBossBattleCamera_Nazrin(m_upPlayer->GetPosition());
			}

			//エンターキーまたはマウスで進行できる
			if (GetAsyncKeyState(VK_RETURN) & 0x8000 || CMouseInput::GetMouseLeft(true, true)|| CMouseInput::GetMouseRight(true, true)|| CMouseInput::GetMouseWheel(true, true)) {
				if (m_OnePush == false) {
					//シーンのセットが完了したら
					if (m_upMovieScene->SceneSetComp == true) {
						//シーンチェンジが始まっていないなら
						if (m_upSceneChange->SceneChangeStart == false) {
							//シーンチェンジ
							m_upSceneChange->SetSceneChangeType(CSceneChange::enSceneType::Right, 80, 10, false);

							//ステージBGM1をストップ
							CSoundManager::Stop(CSoundManager::enSingleSoundList::BGM_Stage1_1);
							//ステージBGM2をストップ
							CSoundManager::Stop(CSoundManager::enSingleSoundList::BGM_Stage1_2);						
						}					
					}
					else {
						//次のシーンに
						m_upMovieScene->NextMessage();
					}
				}

				m_OnePush = true;
			}
			else {
				m_OnePush = false;
			}

			//画面を覆ったらボス戦へ
			if (m_upSceneChange->SceneSetComp == true) {
				//ボスバトルに移行
				m_Scene = enScene::BossBattle;

				m_MovieSceneCameraMoveCo = 0;

				//ボス用のステージに変更する
				m_upStageManager->ChangeStage(CStageManager::enStage::MapBoss);

				//現在存在するバレットとエネミーとワイヤーポイントのインスタンスをすべて空ける
				DeleteInstance_Bullet();
				DeleteInstance_Enemy();
				DeleteInstance_WirePoint();

				//ボスの時のエネミーを配置する
				CEnemySet::LoadEnemies_Boss(m_upEnemy);
				//ワイヤーポイント
				CWirePointSet::LoadWirePoints_Boss(m_upCWirepoint);

				//プレイヤーがいるところを更新
				m_PlayerInStage = enInMap::MapBoss;

				//カメラの場所を変更したところと同じ位置になるようにする
				m_upCamera->SetChangeBossStageCamera(m_upPlayer->GetPositionadd(), m_upBoss->GetPositionadd());

				//ステージの幅と高さをセットする<w.h>
				std::pair<float, float> MapSize = m_upStageManager->GetMapSize();
				m_upCamera->SetStageSize(MapSize.first, MapSize.second);
				m_upPlayer->SetStegeUnder(MapSize.second);//プレイヤーにステージの下の位置を教える

				//ステージメインのサウンド変数初期化
				InitializeStageMainSound();

				//曲のはじめから流すように
				CSoundManager::SingleSoundSeekToStart(CSoundManager::enSingleSoundList::BGM_Stage1_1);
				CSoundManager::SingleSoundSeekToStart(CSoundManager::enSingleSoundList::BGM_Stage1_2);
			}
		}
		else {
			m_MovieSceneCameraMoveCo++;
		}
		break;
	case enScene::BossBattle:
		//ボスが死亡したときの演出入っていないなら
		if (m_upBoss->BossDeadEffect == false) {
			//ボスのサウンド動作
			BossSoundUpdate();
		}

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

			//ボスが死亡したら
			if (m_upBoss->m_State == CBoss::enState::Dead) {
				if (m_upSceneChange->SceneChangeStart == false) {
					if (m_upBoss->BossDeadEffect == false) {
						//ピカっと光るようにする
						m_upSceneChange->SetSceneChangeType(CSceneChange::enSceneType::FadeFinish, 20, 10, true);

						//ボスが死亡したときの演出入りました
						m_upBoss->BossDeadEffect = true;

						m_upCamera->SnakeCamera(50, 50);

						//曲を止める
						CSoundManager::Stop(CSoundManager::BGM_Boss1_1);
						CSoundManager::Stop(CSoundManager::BGM_Boss1_2);
					}
				}
			}
		}

		//エネミーの動作
		//ボスフェーズ2から動くようにする
		if (m_upBoss->m_BossPhase == CBoss::enBossPhase::Phase_2) {
			//ある分回す
			for (int i = 0; i < m_upEnemy.size(); i++) {
				//生存中になっていれば動かす
				if (m_upEnemy[i]->m_State == CEnemy::enState::Living) {
					//プレイヤーの位置を取得する
					m_upEnemy[i]->SetPlayerPos(m_upPlayer->GetCenterPosition());

					//キャッチされていなければ
					if (m_upEnemy[i]->GetCatchWire() != CEnemy::enCatchWire::Catch) {
						//カメラを基準とした範囲内にエネミーが存在しているなら
						if (m_upEnemy[i]->GetCenterPosition().x <= m_upCamera->GetCameraPos().x + (WND_W * 2)) {
							if (m_upEnemy[i]->GetCenterPosition().x >= m_upCamera->GetCameraPos().x - (WND_W * 2)) {
								//横の距離だけ見て判断
								m_upEnemy[i]->Update(m_upBullet);
							}
						}
					}
					else {
						//投げられたら
						if (m_upEnemy[i]->EnemyThrown == true) {
							m_upEnemy[i]->ThrowEnemy();
						}
					}
				}
			}
		}

		//バレットの動作
		for (int i = 0; i < m_upBullet.size(); i++) {
			m_upBullet[i]->Update();
		}

		//ワイヤーポイントの動作
		for (int i = 0; i < m_upCWirepoint.size(); i++) {
			m_upCWirepoint[i]->Update();
		}
		//ワイヤーのアクションの動作?
		m_upWireActionSupporter->Update(m_upCamera->GetCameraPos());

		//当たり判定の関数
		CollisionUpdate();


		//ワイヤーを撃つ処理
		if (m_upPlayer->GetWireShot()) {
			m_pWire->Shot(m_upPlayer, CMouseInput::GetMousePosCamera(m_upCamera.get()));
		}

		if (m_upPlayer->ClearGame == true) {
			if (m_ClearCo >= 180) {
				m_ClearCo = 180;

				if (m_upSceneChange->SceneChangeStart == false) {
					//シーンチェンジ
					m_upSceneChange->SetSceneChangeType(CSceneChange::enSceneType::FadeStart, 4, 60, false);
					//次の入力があるまで黒画面で待機するようにする
					m_upSceneChange->StopScene(true);

					//ボスサウンドの初期化
					InitializeBossSound();
					//曲のはじめから流すように
					CSoundManager::SingleSoundSeekToStart(CSoundManager::enSingleSoundList::BGM_Boss1_1);
					CSoundManager::SingleSoundSeekToStart(CSoundManager::enSingleSoundList::BGM_Boss1_2);
				}
			}
			else {
				m_ClearCo++;
			}

			//シーンが完全に覆いかぶさってから
			if (m_upSceneChange->SceneSetComp == true) {
				//シーンクリアに移動する
				m_Scene = enScene::Clear;

				//カウントを初期化
				m_ClearCo = 0;
			}
		}

		break;
	case enScene::PlayerDeath:
		//プレイヤーのみ動作させる
		m_upPlayer->Update(m_upBullet);
		m_pWire->DayEnd();
		
		//こいしが画面外にでたら
		if (m_upPlayer->m_State == CPlayer::enState::Dead) {
			if (m_upSceneChange->SceneChangeStart == false) {
				//フェード開始
				m_upSceneChange->SetSceneChangeType(CSceneChange::enSceneType::FadeStart, 5, 60, false);
			}
		}

		//シーンが完全に覆いかぶさってから
		if (m_upSceneChange->SceneSetComp == true) {
			//復活処理

			//現在存在するバレットとエネミーとワイヤーポイントのインスタンスをすべて空ける
			DeleteInstance_Bullet();
			DeleteInstance_Enemy();
			DeleteInstance_WirePoint();

			//プレイヤーの初期化
			m_upPlayer->HaveInstanceDelete();	//ハートなどの持っているインスタンスを削除
			m_upPlayer->StartSetting();
			m_upPlayer->Initialization();

			//どのマップにいたかを見る
			switch (m_PlayerInStage) {
			case enInMap::Map01:
				//ステージ1の最初からプレイヤーを配置する
				m_upPlayer->SetStagePos({ 100, 500 });

				//ステージ1のエネミーをセットする
				CEnemySet::LoadEnemies_Stage1(m_upEnemy);
				//ワイヤーポイント
				CWirePointSet::LoadWirePoints_Stage1(m_upCWirepoint);
				break;
			case enInMap::Map02:
				//ステージ2の最初からプレイヤーを配置する
				m_upPlayer->SetStagePos({ 100,200 });

				//ステージ2の時のエネミーを配置する
				CEnemySet::LoadEnemies_Stage2(m_upEnemy);
				//ワイヤーポイント
				CWirePointSet::LoadWirePoints_Stage2(m_upCWirepoint);
				break;
			case enInMap::MapBoss:
				//ステージ2に戻すようにする
				m_upStageManager->ChangeStage(CStageManager::enStage::Map02);

				//ステージの幅と高さをセットする<w.h>
				std::pair<float, float> MapSize = m_upStageManager->GetMapSize();
				m_upCamera->SetStageSize(MapSize.first, MapSize.second);
				m_upPlayer->SetStegeUnder(MapSize.second);//プレイヤーにステージの下の位置を教える

				//ボスは、ボスの1つ前のステージの直前からはじまるようにする
				m_upPlayer->SetStagePos({ 100,200 });

				//ボスも初期化する
				m_upBoss->StartSetting();

				//ステージ2の時のエネミーを配置する
				CEnemySet::LoadEnemies_Stage2(m_upEnemy);
				//ワイヤーポイント
				CWirePointSet::LoadWirePoints_Stage2(m_upCWirepoint);

				//ボスサウンドの初期化
				InitializeBossSound();
				//曲のはじめから流すように
				CSoundManager::SingleSoundSeekToStart(CSoundManager::enSingleSoundList::BGM_Boss1_1);
				CSoundManager::SingleSoundSeekToStart(CSoundManager::enSingleSoundList::BGM_Boss1_2);
				break;
			}

			//ムービーシーンの初期化
			m_upMovieScene->StartSetting();

			//ステージサウンドの変数初期化
			InitializeStageMainSound();

			//曲のはじめから流すように
			CSoundManager::SingleSoundSeekToStart(CSoundManager::enSingleSoundList::BGM_Stage1_1);
			CSoundManager::SingleSoundSeekToStart(CSoundManager::enSingleSoundList::BGM_Stage1_2);

			//ボスバトルには入らないのでメインでOK
			m_Scene = enScene::GameMain;
		}
		break;
	case enScene::Clear:
		if (m_upClearImage->FadeCompleted == false) {
			m_upClearImage->Update();

			//1回だけ押せるように
			if (GetAsyncKeyState(VK_RETURN) & 0x8000 || CMouseInput::GetMouseLeft(true, true) || CMouseInput::GetMouseRight(true, true) || CMouseInput::GetMouseWheel(true, true))
			{
				if (m_OnePush == false) {
					//クリア画面が表示されたら
					if (m_upClearImage->DrawClearImage == true) {
						//フェードアウトを開始する
						m_upClearImage->FadeOutStart();
					}

					//一回だけ押させる
					m_OnePush = true;
				}
			}
			else {
				//離すとリセット
				m_OnePush = false;
			}
		}
		//フェードが終わったら
		else {
			//タイトルへ
			m_Scene = enScene::Title;

			NoCreateInstance = true;	//インスタンスを作らないようにする
			NoDeleteInstance = true;	//インスタンスを消さないようにする

			Destroy();		//一度メモリを解放する

			Create();		//インスタンスを作る

			NoCreateInstance = false;	//初期化
			NoDeleteInstance = false;
		}
		break;
	}

	//スイッチの外にプレイヤーが死亡中になったかを入れる
	if (m_upPlayer->m_State == CPlayer::enState::Dying) {
		//プレイヤーが倒された動作をする
		m_Scene = enScene::PlayerDeath;

		//ステージBGM1をストップ
		CSoundManager::Stop(CSoundManager::enSingleSoundList::BGM_Stage1_1);
		//ステージBGM2をストップ
		CSoundManager::Stop(CSoundManager::enSingleSoundList::BGM_Stage1_2);

		//曲を止める
		CSoundManager::Stop(CSoundManager::BGM_Boss1_1);
		CSoundManager::Stop(CSoundManager::BGM_Boss1_2);
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

		m_upTitleImage->Draw();

		//タイトルシーンようの描画
		m_upPlayer->TitleSceneDraw();
		
		//タイトル関連の画像を描画
		DrawTitleImg();

		break;
	case enScene::GameMain:
	case enScene::Movie:
	case enScene::BossBattle:
	case enScene::PlayerDeath:
		
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
		m_pWire->WireHandDraw(m_upCamera, m_upPlayer.get());

		//エネミー描画
		for (int i = 0; i < m_upEnemy.size(); i++) {
			if (m_upEnemy[i] != nullptr) {
				//エネミーが死亡中になっていなければ
				if (m_upEnemy[i]->m_State != CEnemy::enState::Dying) {
					m_upEnemy[i]->Draw(m_upCamera);
				}
				else {
					//死んだとき用の描画にする
					m_upEnemy[i]->DeadAnimationDraw(m_upCamera);
				}
			}
		}

		//ボスの描画
		if (m_upBoss != nullptr) {
			m_upBoss->Draw(m_upCamera);
		}

		//バレットの描画
		for (int i = 0; i < m_upBullet.size(); i++) {
			if (m_upBullet[i] != nullptr) {
				m_upBullet[i]->Draw(m_upCamera);
			}
		}

		//ムービーシーン中の時だけ描画
		if (m_upMovieScene->StartMovie == true) {
			m_upMovieScene->Draw(m_upCamera);
		}

		for (int i = 0; i < m_upCWirepoint.size(); i++) {
			m_upCWirepoint[i]->Draw(m_upCamera);
		}

		//プレイヤーのハートを描画する
		m_upPlayer->PlayerHeartDraw();
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

	m_upSceneChange->Draw();
	//クリアシーンだけ映す
	if (m_Scene == enScene::Clear) {
		m_upClearImage->Draw();
	}
}

void CGame::InitializeTitleSound()
{
	//初期化
	TitleStartSound = false;
	TitleStartSoundCo = 0;

	TitleBGMSwitch = false;
	TitleBGMSwitchCo = 0;
}

void CGame::InitializeStageMainSound()
{
	//初期化
	StageBGMSwitch = false;
	StageBGMSwitchCo = 0;
}

void CGame::InitializeBossSound()
{
	//初期化
	BossBGMSwitch = false;
	BossBGMSwitchCo = 0;
}

void CGame::TitleSoundUpdate()
{
	//タイトルのBGMの開始BGMを流していないなら
	if (TitleStartSound == false) {
		CSoundManager::Play(CSoundManager::enSingleSoundList::BGM_TitleStart, true);

		TitleStartSound = true;

		TitleStartSoundCo = 0;
	}
	else {
		//タイトルシーンがまだ開始していないなら
		if (m_TitleSceneSet == false) {
			//規定のフレームになったら
			if (TitleStartSoundCo >= 120) {
				//BGMをループして、最初から流したなら
				if (TitleBGMSwitchCo <= 0) {
					//1度目
					if (TitleBGMSwitch == false) {
						//BGM1を再生	
						CSoundManager::Play(CSoundManager::enSingleSoundList::BGM_Title_1, true);

						//タイトルBGM1を流したので次は2を流す
						TitleBGMSwitch = true;
					}
					//2度目	以降繰り返し
					else if (TitleBGMSwitch == true) {
						//BGM2を再生
						CSoundManager::Play(CSoundManager::enSingleSoundList::BGM_Title_2, true);

						//タイトルBGM1を流したので次は2を流す
						TitleBGMSwitch = false;
					}

					//ループするタイミングをセット
					TitleBGMSwitchCo = 60 * 15 + 38;
				}
				else {
					//最初に通ってから次のやつを流すようにしたいので--
					TitleBGMSwitchCo--;
				}
			}
			else {
				//タイトルカウント
				TitleStartSoundCo++;
			}
		}
	}
}

void CGame::StageMainSoundUpdate()
{
	//BGMをループして、最初から流したなら
	if (StageBGMSwitchCo <= 0) {
		//1度目
		if (StageBGMSwitch == false) {
			//BGM1を再生	
			CSoundManager::Play(CSoundManager::enSingleSoundList::BGM_Stage1_1, true);

			//ステージBGM1を流したので次は2を流す
			StageBGMSwitch = true;
		}
		//2度目	以降繰り返し
		else if (StageBGMSwitch == true) {
			//ステージBGM2を再生
			CSoundManager::Play(CSoundManager::enSingleSoundList::BGM_Stage1_2, true);

			//タイトルBGM1を流したので次は2を流す
			StageBGMSwitch = false;
		}

		//ループするタイミングをセット
		StageBGMSwitchCo = 60 * 111 - 40;
	}
	else {
		//最初に通ってから次のやつを流すようにしたいので--
		StageBGMSwitchCo--;
	}
}

void CGame::BossSoundUpdate()
{
	//BGMをループして、最初から流したなら
	if (BossBGMSwitchCo <= 0) {
		//1度目
		if (BossBGMSwitch == false) {
			//ボスBGM1を再生	
			CSoundManager::Play(CSoundManager::enSingleSoundList::BGM_Boss1_1, true);

			//ステージBGM1を流したので次は2を流す
			BossBGMSwitch = true;
		}
		//2度目	以降繰り返し
		else if (BossBGMSwitch == true) {
			//ボスBGM2を再生
			CSoundManager::Play(CSoundManager::enSingleSoundList::BGM_Boss1_2, true);

			//タイトルBGM1を流したので次は2を流す
			BossBGMSwitch = false;
		}

		//ループするタイミングをセット
		BossBGMSwitchCo = 60 * 39;
	}
	else {
		//最初に通ってから次のやつを流すようにしたいので--
		BossBGMSwitchCo--;
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
		if (m_upBullet[i] != nullptr) {
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
}

void CGame::DeleteEnemy()
{
	//バレットと同じようにエネミーを消す
	for (int i = 0; i < m_upEnemy.size(); i++) {
		if (m_upEnemy[i] != nullptr) {
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
}

void CGame::DeleteInstance_Bullet()
{
	//バレットのインスタンスを消す
	//
	for (int i = 0; i < m_upBullet.size(); i++) {
		if (m_upBullet[i] != nullptr) {
			m_upBullet.erase(
				std::remove_if(m_upBullet.begin(), m_upBullet.end(),
					[](const std::unique_ptr<CBullet>& Bullet) {
						return Bullet != nullptr;
					}),
				m_upBullet.end()
			);
		}
	}
}

void CGame::DeleteInstance_Enemy()
{
	//すべてのエネミーのインスタンスを削除
	//インスタンスがあるエネミーのメモリを空ける
	for (int i = 0; i < m_upEnemy.size(); i++) {
		if (m_upEnemy[i] != nullptr) {
			m_upEnemy.erase(
				std::remove_if(m_upEnemy.begin(), m_upEnemy.end(),
					[](const std::unique_ptr<CEnemy>& Enemy) {
						return Enemy != nullptr;
					}),
				m_upEnemy.end()
			);
		}
	}
}

void CGame::DeleteInstance_WirePoint()
{
	//すべてのワイヤーポイントのインスタンスを削除
	//インスタンスがあるワイヤーポイントのメモリを空ける
	for (int i = 0; i < m_upCWirepoint.size(); i++) {
		if (m_upCWirepoint[i] != nullptr) {
			m_upCWirepoint.erase(
				std::remove_if(m_upCWirepoint.begin(), m_upCWirepoint.end(),
					[](const std::unique_ptr<CWirepoint>& upCWirepoint) {
						return upCWirepoint != nullptr;
					}),
				m_upCWirepoint.end()
			);
		}
	}
}

void CGame::CollisionUpdate()
{
	//マウスとエネミーの当たり判定処理
	m_upCollisionDetection->MouseToEnemyCollision(m_upEnemy, m_upCamera);
	//マウスとワイヤーポイント
	m_upCollisionDetection->MouseToWirePoint(m_upCWirepoint, m_upCamera);

	//プレイヤーとエネミーの当たり判定処理
	m_upCollisionDetection->PlayerToEnemyCollision(m_upPlayer, m_upEnemy);

	//ワイヤーと敵	
	m_upCollisionDetection->WireToEnemyCollision(m_upEnemy, m_pWire, m_upPlayer, m_upWireActionSupporter);

	//プレイヤーとバレットの当たり判定処理
	m_upCollisionDetection->PlayerToBulletCollision(m_upPlayer, m_upBullet);

	//ワイヤーとワイヤーポイントの当たり判定処理
	m_upCollisionDetection->WireToWirepointCollision(m_upCWirepoint, m_pWire, m_upPlayer, m_upWireActionSupporter);

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

		//タイトルセレクト音を出す
		CSoundManager::PlaySE(CSoundManager::enMultiSoundList::SE_TitleSelect);
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x0001)
	{
		m_CursorAction++;
		if (m_CursorAction >= m_Action.size()) m_CursorAction = 0;

		//タイトルセレクト音を出す
		CSoundManager::PlaySE(CSoundManager::enMultiSoundList::SE_TitleSelect);
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
	m_CursorPosition.push_back({ 150, 430, });
	m_CursorPosition.push_back({ 150, 555, });

}

void CGame::DrawTitleImg()
{
	VECTOR2_f StagePos = m_upTitleImage->GetPosition();

	CImageManager::SelectImg(CImageManager::enImgList::IMG_Cursor)->TransAlBlend(
		StagePos.x + m_CursorPosition[m_CursorAction].x, m_CursorPosition[m_CursorAction].y,
		128, 128,
		0, 0,
		255);

	CImageManager::SelectImg(CImageManager::enImgList::IMG_Makasero)->TransAlBlend(
		StagePos.x + 305, 457,
		307, 75,
		0, 0,
		255);

	CImageManager::SelectImg(CImageManager::enImgList::IMG_Kaerimasu)->TransAlBlend(
		StagePos.x + 300, 585,
		340, 75,
		0, 0,
		255);

	//====================タイトル=======================
	CImageManager::SelectImg(CImageManager::enImgList::IMG_Makasero)->TransAlBlend(
		StagePos.x + 800, 100,
		307, 75,
		0, 0,
		255);
	//====================================================
}
//----------------------------------------
