#include "CNazrin.h"

CNazrin::CNazrin()
	: m_BulletShot(false)
	, m_BulletShotCo(0)
	, m_AttackAnimCo(0)
	, m_AttackAnimTime(false)
{
	StartSetting();
}

CNazrin::~CNazrin()
{
	//バナナのメモリを開放
	m_upBanana.reset();
}

void CNazrin::StartSetting()
{
	//見えないように
	m_Position = { -400,-400 };

	//デフォルトにしておく
	m_Color = enColor::NoColor;
	//ナズーリン
	m_MyCharacter = enMyCharacter::Nazrin;
	//敵陣営
	m_MyCamp = enMyCamp::EnemyCamp;

	//生存中にする
	m_State = enState::Living;

	//スタンバイ中
	m_AttackMove = enAttackMove::Standby;

	//フェーズ1のHPをセット
	HP = Phase1_Max_HP;

	//元画像サイズ
	m_FrameSize = { 64,64 };
	m_Framesplit = { 0,0,144,144 };

	//当たり判定
	m_RealFrameSplit = { 104,104 };

	m_OldPosition = m_Position;

	//ジャンプしているか
	m_Jumping = false;
	//ジャンプするタイミングをカウント
	m_JumpingCo = 0;
	//ジャンプ力は都度決める
	m_JumpPower = 0;

	//地面に立っているか
	m_GroundStand = false;
	//落下速度の初期化
	m_FallingSpeed = 0;

	//カウント初期化
	m_AttackMoveChangeCo = 0;
	//バレット撃った回数を初期化
	m_HowShotBullet = 0;
	//攻撃の動作をカウントする
	m_AttackMoveCo = 0;

	//バトルが始まった位置初期化
	m_BattleStartPos = { 0,0 };

	//フェーズを変える変数初期化
	m_PhaseChangeCo = 0;
	m_NextPhaseSetting = false;
	//アニメーション
	m_PhaseChangeAniTime = false;

	//アタックムーブを戻す
	m_ReturnMove = false;
	//Yの位置をいじる用
	m_SetYPosition = false;
	//配置場所に訪れた回数をカウント
	m_SetBlockPosCo = 0;

	//減ったHPを見る
	m_DecreaseHP = 0;

	//ベクトル変数
	m_Vector = { 0,0 };

	//リンゴをゲットできるまでのカウント
	m_GetAppleCo = 0;

	//元居た場所を記憶する
	m_MemoryPos = m_Position;

	//次のブロックまでを初期化
	NextSetPosBlock = false;

	//リンゴをとれるか初期化
	GetingApple = false;

	//バレット関連初期化
	m_BulletShot = false;
	m_BulletShotCo = 0;
	//攻撃アニメーション初期化
	m_AttackAnimCo = 0;
	m_AttackAnimTime = false;

	//スタンバイから始まるように
	m_AttackMove = enAttackMove::Standby;
	//フェーズ1から始まるように
	m_BossPhase = enBossPhase::Phase_1;

	//無敵初期化
	NoHit = false;
	//無敵状態のカウント
	NoHitAttackCo = 0;

	//ボスの死亡演出初期化
	BossDeadEffect = false;
	//死亡アニメーション初期化
	m_DeadAnimCo = 0;

	//不透明に
	m_Alpha = 255;
}

void CNazrin::Draw(std::unique_ptr<CCamera>& pCamera)
{
	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	if (m_State != enState::Dead) {
		//アニメーション処理
		Animation();

		//CImageManagerがシングルトン化しているので、サウンドのように使える
		CImageManager::SelectImg(CImageManager::enImgList::IMG_Nazrin)->TransAlBlendRotation3(
			DispPos.x,				//表示位置x座標
			DispPos.y,				//表示位置y座標
			m_Framesplit.w,			//画像幅
			m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
			m_Framesplit.x,			//元画像x座標
			m_Framesplit.y,			//元画像y座標
			m_FrameSize.x,			//元画像xサイズ		
			m_FrameSize.y,			//元画像yサイズ
			m_Alpha,
			m_Delection.x, m_Delection.y, m_Delection.z);					//透明度、角度
	}
	//死んだとき
	else {
		//リンゴを表示するようにする
		CImageManager::SelectImg(CImageManager::enImgList::IMG_Apple)->TransAlBlendRotation(
			DispPos.x,				//表示位置x座標
			DispPos.y,				//表示位置y座標
			m_Framesplit.w,			//画像幅
			m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
			m_Framesplit.x,			//元画像x座標
			m_Framesplit.y,			//元画像y座標
			m_FrameSize.x,			//元画像xサイズ		
			m_FrameSize.y,			//元画像yサイズ
			m_Alpha, 0);			//透明度、角度
	}

	//ボスの登場回数が規定なら
	if (m_SetBlockPosCo >= BossAppears) {
		if (m_upBanana != nullptr) {
			//バナナを描画
			m_upBanana->Draw(pCamera);
		}
	}
}

void CNazrin::Update(std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	m_OldPosition = m_Position;

	switch (m_State) {
	case enState::Living:
		//次のフェーズの準備を行う
		if (m_NextPhaseSetting == true) {
			//準備完了まで無敵にする
			NoHit = true;

			//フェーズチェンジを完了する時間
			if (m_PhaseChangeCo < 300) {
				//フェーズを変えるカウント
				m_PhaseChangeCo++;
				if (m_PhaseChangeCo >= 240) {
					if (m_Jumping == false) {
						m_JumpPower = 30;

						//ジャンプするように
						m_FallingSpeed = -m_JumpPower;
						//ジャンプ中にする
						m_Jumping = true;

						//地面から離れた
						m_GroundStand = false;
					}

					m_Position.x += 30;
				}
				else if (m_PhaseChangeCo >= 80) {
					//ノーマル状態に戻す
					m_PhaseChangeAniTime = false;
				}
			}
			//フェーズチェンジ完了
			else {
				//ポジションセット
				m_Position.x = m_CameraPos.x + (WND_W / 2) + 150;
				m_Position.y = 0;

				//落下しないように
				m_FallingSpeed = 0;

				//フェーズ2へ移行する
				m_BossPhase = enBossPhase::Phase_2;

				//準備完了
				m_NextPhaseSetting = false;

				//向きをもとに戻す
				m_Delection.y = 0;

				//スタンバイに指定
				m_AttackMove = enAttackMove::Standby;

				//もう飛んでません
				m_Jumping = false;

				//攻撃回数カウントをリセット
				m_AttackMoveCo = 0;
				//撃った回数をリセットさせる
				m_HowShotBullet = 0;
				//カウントをリセットしておく
				m_AttackMoveChangeCo = 0;
				//バレットを撃つカウントをリセット
				m_BulletShotCo = 0;
				//バレットを撃たないように
				m_BulletShot = false;
			}

			//点滅を直すために実行
			if (m_AttackHit == true) {
				//攻撃が当たらない時間を過ぎたら
				if (NoHitAttackCo >= NoHitAttackTime) {
					//カウントが増えないようにする
					NoHitAttackCo = 0;
					
					m_AttackHit = false;
					//表示する
					m_Alpha = 255;
				}
				else {
					NoHitAttackCo++;

					//点滅するようにする
					if (NoHitAttackCo % 7 == 0) {
						if (m_Alpha == 0) {
							//半透明にする
							m_Alpha = 200;
						}
						else {
							m_Alpha = 0;
						}
					}
				}
			}
		}
		else {
			//ボスのフェーズごとの動き
			switch (m_BossPhase) {
				//フェーズ1
			case enBossPhase::Phase_1:
				//攻撃のパターン
				switch (m_AttackMove) {
				case enAttackMove::Standby:
					//攻撃パターンを変えるとき
					if (m_AttackMoveChangeCo >= 180) {
						m_AttackMoveChangeCo = 0;

						//パターン1に指定
						m_AttackMove = enAttackMove::Move_01;

						//ジャンプ力(ムーブ1用)をセット
						m_JumpPower = 30;
					}
					else {
						m_AttackMoveChangeCo++;
					}
					break;
				case enAttackMove::Move_01:

					//ムーブ1の動作を実行(フェーズ違いでも同じ関数が使えるように)
					BossMove_1Update(m_BossPhase, upBullet);

					break;
				case enAttackMove::Move_02:

					//ムーブ2の動作を実行
					BossMove_2Update(m_BossPhase, upBullet);

					break;
				}
				break;
				//フェーズ2
			case enBossPhase::Phase_2:
				//ムーブを戻す処理が完了してから動かす
				if (m_ReturnMove == false) {
					//右にプレイヤー
					if (m_PlayerPos.x >= GetCenterPosition().x) {
						m_Delection.y = 180;
					}
					//左にプレイヤー
					else {
						m_Delection.y = 0;
					}

					//攻撃のパターン
					switch (m_AttackMove) {
					case enAttackMove::Standby:
						//ボスステージでの、ボスの配置場所に行っていないなら
						if (NextSetPosBlock == false) {
							//カメラの横について直線移動する
							m_Position.x = m_CameraPos.x + (WND_W / 2);
							m_Position.y = 0;

							//落下しないように
							m_FallingSpeed = 0;

							//攻撃が当たらないように
							NoHit = true;

							//配置場所までは初期化
							m_SetYPosition = false;
						}
						else {
							//配置する場所が決まって、Y座標をいじっていないなら
							if (m_SetYPosition == false) {
								//ボスの登場回数が規定になったら
								if (m_SetBlockPosCo >= BossAppears) {
									//自分の下にバナナを配置する	位置は都度変える
									m_upBanana = std::make_unique<CBanana>(m_Position);
								}
								//上から来るようにする
								m_Position.y -= 700;
								m_SetYPosition = true;

								//落下速度リセット
								m_FallingSpeed = 0;
							}

							//カメラで見える場所まで来たら
							if (m_CameraPos.x + (WND_W / 3) >= m_Position.x) {
								//ランダムで、ムーブ1か2を選ぶ
								m_AttackMove = (rand() & 1) + 1;

								//カラーをランダムに変更する 0~4
								m_Color = rand() / 8191;
							}

							//攻撃が当たるように
							NoHit = false;
						}
						break;
					case enAttackMove::Move_01:

						//ムーブ1の動作を実行	(フェーズ2用の動作)
						BossMove_1Update(m_BossPhase, upBullet);

						break;
					case enAttackMove::Move_02:

						//ムーブ2の動作を実行
						BossMove_2Update(m_BossPhase, upBullet);

						break;
					}
				}
				else {
					//ムーブを戻す動作
					RetuanMoveSet();
				}

				//ボスの登場回数が規定になったら
				if (m_SetBlockPosCo >= BossAppears) {
					//地面についたら
					if (m_GroundStand == true) {
						//保険
						if (m_upBanana != nullptr) {
							//バナナを吹っ飛ばす準備
							m_upBanana->SetBlowOffBanana();

							//死亡中にする
							m_State = enState::Dying;
							//被弾アニメーション
							m_PhaseChangeAniTime = true;

							//飛び上がるようにする
							m_JumpPower = 30;
							m_FallingSpeed = -m_JumpPower;

							//地上から離れるように
							m_GroundStand = false;
						}
					}
					//ボスがまだ地面についていないなら
					else {
						//保険
						if (m_upBanana != nullptr) {
							//バナナを地面に設置する
							m_upBanana->Update();
						}
					}
				}
				break;
			}

			//攻撃を受けたとき
			if (m_AttackHit == true) {
				//攻撃が当たらない時間を過ぎたら
				if (NoHitAttackCo >= NoHitAttackTime) {
					//カウント初期化
					NoHitAttackCo = 0;
					m_AttackHit = false;

					//表示する
					m_Alpha = 255;
				}
				else {
					NoHitAttackCo++;

					//点滅するようにする
					if (NoHitAttackCo % 7 == 0) {
						if (m_Alpha == 0) {
							//半透明にする
							m_Alpha = 200;
						}
						else {
							m_Alpha = 0;
						}
					}
				}
			}
		}
		break;
	case enState::Dying:
		//回転する
		m_Delection.z += 10;

		//ボスの登場回数が規定なら
		if (m_SetBlockPosCo >= BossAppears) {
			//保険
			if (m_upBanana != nullptr) {
				//バナナを吹っ飛ばすようにする
				m_upBanana->BlowOffBanana();
			}
		}

		//飛び上がって地面に激突したら
		if (m_GroundStand == true) {
			//死
			m_State = enState::Dead;

			//落ちないように
			m_FallingSpeed = 0;

			//リンゴを描画するようにする
			m_FrameSize = { 32,32 };
			m_Framesplit = { 0,0,32,32 };
			//角度を初期化
			m_Delection = { 0,0,0 };

			//不透明に
			m_Alpha = 255;

			//少し浮かせて配置する
			m_Position.y += 0;

			//ベクトルを初期化
			m_Vector = { 0,0 };

			//Y方向にだけ動かす
			m_Speed.x = 0;
			m_Speed.y = 4;

			//地上にいないようにする
			m_GroundStand = false;

			//真ん中からリンゴが出るようにする
			m_Position = GetCenterPosition();

			//倒された位置を記憶する
			m_MemoryPos = m_Position;

			//バナナのメモリを開放
			m_upBanana.reset();

			//ボス撃破音をだす
			CSoundManager::PlaySE_NoDuplication(CSoundManager::enSingleSoundList::SE_BossDead);
		}
		break;
	case enState::Dead:
		m_Vector.y += m_Speed.y / 100;

		m_Position.y = m_MemoryPos.y + 40 * sin(m_Vector.y);

		m_FallingSpeed = 0;

		//ボスを倒してからリンゴが取れるまでの時間
		if (m_GetAppleCo >= 100) {
			GetingApple = true;
		}
		else {
			m_GetAppleCo++;
		}
		break;
	}

	if (m_State != enState::Dead) {
		//落下速度を入れる
		m_Position.y += m_FallingSpeed;

		StageCollision(44, 44);
	}
}

void CNazrin::PlayerAttackHit(int Damage)
{
	//攻撃が当たった
	m_AttackHit = true;
	//HPを減らす
	HP -= Damage;
	//攻撃が当たらない時間のカウントをセット
	NoHitAttackCo = 0;

	//半透明にする
	m_Alpha = 200;

	if (m_BossPhase == enBossPhase::Phase_2) {
		m_DecreaseHP += Damage;

		//フェーズ２の時に減ったHPが規定に達したら
		if (m_DecreaseHP >= Phase2_HP_Decrease) {
			//ムーブを戻すようにする
			m_ReturnMove = true;

			//リセットしておく
			m_DecreaseHP = 0;

			//被弾アニメーション
			m_PhaseChangeAniTime = true;
		}
	}

	//HPがなくなったら
	if (HP <= 0) {
		//フェーズが1なら
		if (m_BossPhase == enBossPhase::Phase_1) {
			//フェーズ2に移行する
			m_NextPhaseSetting = true;
			//フェーズチェンジ時のアニメーションをする
			m_PhaseChangeAniTime = true;

			//攻撃を中断させる
			m_AttackAnimTime = false;

			//もしジャンプしていたなら初期化する
			m_Jumping = false;

			//HPを入れる
			HP = Phase2_MAX_HP;

			//即落下するようにする
			m_FallingSpeed = 0;
		}
		//フェーズが2なら
		else if (m_BossPhase == enBossPhase::Phase_2) {
			//死亡中にする
			m_State = enState::Dying;
			//被弾アニメーション
			m_PhaseChangeAniTime = true;

			//飛び上がるようにする
			m_JumpPower = 30;
			m_FallingSpeed = -m_JumpPower;

			//地上から離れるように
			m_GroundStand = false;

			//演出で一瞬止める
			Sleep(180);
		}
	}

	//ヒット音をだす
	CSoundManager::PlaySE(CSoundManager::enMultiSoundList::SE_AttackHit);
}

void CNazrin::ThrowEnemyHit(int Damage)
{
	//処理に違いはないので呼ぶ出すようにする
	PlayerAttackHit(Damage);
}

//ボスバトルが始まった時の関数
void CNazrin::BossBattleFlag(VECTOR2_f PlayerPos)
{
	//ナズーリンの配置
	m_Position = PlayerPos;
	m_Position.x += WND_W - 300;
	//埋まり対策
	m_Position.y -= 1;

	//バトルが始まった時の位置を保存
	m_BattleStartPos = m_Position;

	//スタンバイ状態を早めに解いておく
	m_AttackMoveChangeCo = 160;

	//左に向くようにする
	m_Delection.y = 0;

	//青色の状態から始まるようにする
	m_Color = enColor::Blue;
}

void CNazrin::MovieSceneUpdate()
{
	m_OldPosition = m_Position;
	//落下速度を入れる
	m_Position.y += m_FallingSpeed;

	StageCollision(44, 44);
}

void CNazrin::Animation()
{
	//攻撃アニメーション
	if (m_AttackAnimTime == true) {
		//ほかのアニメーションのカウントリセット
		m_AnimetionCo = 0;

		m_Framesplit.x = 0;
		m_Framesplit.y = 128;
		//15フレームたったら
		if (m_AttackAnimCo >= 15) {
			//アニメーションを変える
			m_Framesplit.x = 64;

			m_AttackAnimCo = 15;
		}
		else {
			m_AttackAnimCo++;
		}
	}
	//ジャンプアニメーション
	else if (m_GroundStand == false) {
		m_Framesplit.x = 0;
		m_Framesplit.y = 64;

		//ほかのアニメーションのカウントリセット
		m_AnimetionCo = 0;
		m_AttackAnimCo = 0;
	}
	//待機アニメーション
	else {
		//ほかのアニメーションのカウントリセット
		m_AttackAnimCo = 0;

		m_Framesplit.y = 0;

		m_AnimetionCo++;
		if (m_AnimetionCo >= 15) {
			m_Framesplit.x += 64;
			//最後のアニメーションなら
			if (m_Framesplit.x > 64 * 3) {
				m_Framesplit.x = 0;
			}
			m_AnimetionCo = 0;
		}
	}

	//優先してする
	//フェーズチェンジ || 被弾アニメーション
	if (m_PhaseChangeAniTime == true) {
		m_Framesplit.x = 0;
		m_Framesplit.y = 192;
	}
}

void CNazrin::StageCollision(double OffsetPos_X, double OffsetPos_Y)
{
	VECTOR2_f offsetPos = { OffsetPos_X, OffsetPos_Y };

	//ブロックに触れていないなら				重力分も入れて判定する	判定のサイズをナズーリンの大きさに合わせる
	if (CStageCollision::GetInstance()->IsHit(m_Position.x, m_Position.y + Gravity, 60, 100, offsetPos) != true) {
		m_GroundStand = false;

		//常に動いているように見えるが、地面や天井の当たり判定の時に、目に見えないレベルで浮いている
		//落下速度を計算
		m_FallingSpeed += Gravity;
	}
	else {
		//動いた距離
		double MoveRangeX = m_Position.x - m_OldPosition.x;
		double MoveRangeY = m_Position.y - m_OldPosition.y;

		//ナズーリンは横の判定がいらない
		//まだ縦に動こうとしているなら
		if (MoveRangeY != 0.0f) {
			while (1) {
				VECTOR2_f checkPos = m_OldPosition;
				checkPos.y += MoveRangeY;
				//															判定のサイズをナズーリンの大きさに合わせる
				if (CStageCollision::GetInstance()->IsHit(m_OldPosition.x, checkPos.y, 60, 100, offsetPos)) {

					if (std::abs(MoveRangeY) <= 1.0f) {
						//地面の判定
						if (MoveRangeY > 0) {
							if (m_GroundStand == false) {
								m_FallingSpeed = 0;

								m_GroundStand = true;
							}
						}
						//天井の判定
						else {
							m_FallingSpeed = 0;
						}

						MoveRangeY = 0; // 1px以下なら移動不可として終了	床や天井に当たったときの感じ
						m_Position.y = m_OldPosition.y;
						break;
					}

					//移動距離を 0 に近づける（後ずさりする）
					//地面判定としてみることもできる
					if (MoveRangeY > 0) {
						//右に動こうとしていたなら、1px 左に戻す
						MoveRangeY = MoveRangeY - 1.0;
					}
					else {
						//左に動こうとしていた（マイナスだった）なら、1px 右に戻す
						//例: -5.0 + 1.0 = -4.0 （0に近づく！）
						MoveRangeY = MoveRangeY + 1.0;
					}

					// 符号を維持したまま、絶対値を1減らす
					// 例: 5.0 -> 4.0 / -5.0 -> -4.0
					//double sign = (MoveRangeY > 0) ? 1.0 : -1.0;
					//MoveRangeY -= sign * 1.0;
				}
				else {
					m_Position.y = checkPos.y;
					break;
				}
			}
		}
	}

	//フェーズ2の時のみ動かす　または　ムーブを戻し終わってから
	if (m_BossPhase == enBossPhase::Phase_2 || m_ReturnMove == true) {
		//セットポスブロックの位置に来たら
		if (CStageCollision::GetInstance()->GetBossSetPos() == true) {
			NextSetPosBlock = true;
		}
	}
}

void CNazrin::BossMove_1Update(int BossPhase, std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	switch (BossPhase) {
		//フェーズ1
	case enBossPhase::Phase_1:
		//5発撃つまで続く
		if (m_HowShotBullet < 5) {
			//撃つ間隔
			if (m_BulletShotCo >= 10) {
				//1回目
				if (m_AttackMoveCo == 0) {
					//バレットを3発同じ角度で出す(スピードは違う)
					for (int i = 1; i <= 5; i++) {
						upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 5 * i, (-20 * m_HowShotBullet), -100, 80, 300, 0, false));
					}
					upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, -5, (-20 * m_HowShotBullet), -100, 80, 60, 0, false));
				}
				//2回目
				else if (m_AttackMoveCo == 1) {
					//バレットを3発同じ角度で出す(2回目は右方向に撃つ)
					for (int i = 1; i <= 5; i++) {
						upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 5 * i, (20 * m_HowShotBullet), -80, 80, 300, 0, false));
					}
					upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, -5, (20 * m_HowShotBullet), -80, 80, 60, 0, false));
				}
				//何発(何回)バレットを撃った動作をしたか
				m_HowShotBullet++;

				m_BulletShotCo = 0;

				//カラーをランダムに変更する 0~4
				m_Color = rand() / 8191;
			}
			else {
				m_BulletShotCo++;
			}

			//攻撃アニメーション中
			m_AttackAnimTime = true;
		}
		//次の行動をする
		else {
			//待機時間を与える
			if (m_AttackMoveChangeCo >= 50) {
				//1回目
				if (m_AttackMoveCo == 0) {
					if (m_Jumping == false) {
						//ジャンプするように
						m_FallingSpeed = -m_JumpPower;
						//ジャンプ中にする
						m_Jumping = true;

						//地面から離れた
						m_GroundStand = false;

						//攻撃アニメーションではなくする
						m_AttackAnimTime = false;
					}

					//地面にいない間だけ移動する
					if (m_GroundStand == false) {
						//左に飛んで移動する
						m_Position.x -= 13;
					}
					//地面についたら
					else {
						//もう一回動かします
						m_AttackMoveCo = 1;

						//撃った回数をリセットさせる
						m_HowShotBullet = 0;

						//カウントをリセットしておく
						m_AttackMoveChangeCo = 0;

						//反対の方向を見るようにする
						m_Delection.y = 180;
					}
				}
				//2回目以降は別の行動にする
				else {
					//次の行動に移す
					m_AttackMove = enAttackMove::Move_02;

					//攻撃回数カウントをリセット
					m_AttackMoveCo = 0;
					//撃った回数をリセットさせる
					m_HowShotBullet = 0;
					//カウントをリセットしておく
					m_AttackMoveChangeCo = 0;
					//バレットを撃つカウントをリセット
					m_BulletShotCo = 0;

					//次に動く時のジャンプ力を設定する
					m_JumpPower = 80;

					//上に飛んでいくようにする
					//ジャンプするように
					m_FallingSpeed = -m_JumpPower;

					//ジャンプ中にする
					m_Jumping = true;
					m_GroundStand = false;

					//ジャンプする前の場所を記憶
					m_MemoryPos = m_Position;

					//攻撃アニメーションではなくする
					m_AttackAnimTime = false;
				}
			}
			else {
				m_AttackMoveChangeCo++;
			}

		}
		break;
		//フェーズ2
	case enBossPhase::Phase_2:
		//攻撃した回数が規定に達したら
		if(m_AttackMoveCo>=3){
			//少し待機
			if (m_AttackMoveChangeCo >= 60) {
				//次の場所に移動するようにする
				m_ReturnMove = true;
				//ムーブを戻す時に早めに戻るようにする
				m_AttackMoveChangeCo = 60;

				//アタック変数リセット
				m_HowShotBullet = 0;
				m_BulletShotCo = 0;
				m_AttackMoveCo = 0;
			}
			else {
				m_AttackMoveChangeCo++;
			}
		}
		//50発撃つまで続ける
		else if (m_HowShotBullet < 50) {
			//撃つ感覚
			if (m_BulletShotCo >= 50) {
				upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 8, (60 * m_HowShotBullet), -70 + 5 * m_HowShotBullet, 80, 180, -6, false));

				//撃ったバレットをカウント
				m_HowShotBullet++;
				//フロー対策
				m_BulletShotCo = 50;

				//攻撃アニメーション中
				m_AttackAnimTime = true;
			}
			else {
				m_BulletShotCo++;
			}
		}
		else {
			//もう一度撃つようにする
			m_HowShotBullet = 0;
			m_BulletShotCo = 0;

			//攻撃アニメーションを一時解除
			m_AttackAnimTime = false;

			//攻撃した回数をカウント
			m_AttackMoveCo++;
		}
		break;
	}
}

void CNazrin::BossMove_2Update(int BossPhase, std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	switch (BossPhase) {
	case enBossPhase::Phase_1:
		//画面上の外にいったら
		if (m_Position.y <= m_CameraPos.y - (WND_H / 2) - m_Framesplit.h) {
			//落下しないようにする
			m_FallingSpeed = 0;

			//バレットの撃てる準備ができた
			m_BulletShot = true;
		}

		if (m_BulletShot == true) {
			//バレットを撃つタイミング
			if (m_BulletShotCo >= 50) {
				//攻撃した回数が6回に達するまで
				if (m_AttackMoveCo < 6) {
					//攻撃した回数でどこにバレットを撃つかを決める
					switch (m_AttackMoveCo) {
						//左右に弾を放つ
					case 0:
						//左右から迫るバレット
						for (int i = 0; i < 4; i++) {
							switch (i) {
							case 0:
								//画面左端上から出てくるように
								m_Position.x = m_CameraPos.x - (WND_W / 2) - m_Framesplit.w - 150;
								m_Position.y = m_CameraPos.y - (WND_H / 2) + 24;
								upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 2, 0, 0, 150, 240, 3, true));
								break;
							case 1:
								//画面左端下から出てくるように
								m_Position.x = m_CameraPos.x - (WND_W / 2) - m_Framesplit.w - 150;
								m_Position.y = m_CameraPos.y + (WND_H / 2) - m_Framesplit.h;
								upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 2, 0, 0, 150, 240, 3, true));
								break;
							case 2:
								//画面右端上から出てくるように
								m_Position.x = m_CameraPos.x + (WND_W / 2) + 150;
								m_Position.y = m_CameraPos.y - (WND_H / 2) + 24;
								upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 2, 0, 180, 150, 240, 3, true));
								break;
							case 3:
								//画面右端下から出てくるように
								m_Position.x = m_CameraPos.x + (WND_W / 2) + 150;
								m_Position.y = m_CameraPos.y + (WND_H / 2) - m_Framesplit.h;
								upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 2, 0, 180, 150, 240, 3, true));
								break;
							}
						}
						//次のバレット
						m_AttackMoveCo++;
						//クールタイム
						m_BulletShotCo = 0;
						break;
					case 1:
						//四隅から斜めにバレットを飛ばす
						for (int i = 0; i < 4; i++) {
							switch (i) {
							case 0:
								//画面左端上から出てくるように
								m_Position.x = m_CameraPos.x - (WND_W / 2) - m_Framesplit.w - 0;
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h - 50;
								upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 2, 0, 55, 150, 240, 3, true));
								break;
							case 1:
								//画面左端下から出てくるように
								m_Position.x = m_CameraPos.x - (WND_W / 2) - m_Framesplit.w - 0;
								m_Position.y = m_CameraPos.y + (WND_H / 2) + m_Framesplit.h + 0;
								upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 2, 0, -55, 150, 240, 3, true));
								break;
							case 2:
								//画面右端上から出てくるように
								m_Position.x = m_CameraPos.x + (WND_W / 2) + 0;
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h - 50;
								upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 2, 0, 125, 150, 240, 3, true));
								break;
							case 3:
								//画面右端下から出てくるように
								m_Position.x = m_CameraPos.x + (WND_W / 2) + 0;
								m_Position.y = m_CameraPos.y + (WND_H / 2) + m_Framesplit.h + 0;
								upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 2, 0, 235, 150, 240, 3, true));
								break;
							}
						}
						//次のバレット
						m_AttackMoveCo++;
						//クールタイム
						m_BulletShotCo = 0;
						break;
					case 2:
						//上からバレットを飛ばす
						for (int i = 0; i < 3; i++) {
							switch (i) {
							case 0:
								//画面左上から出てくるように
								m_Position.x = m_CameraPos.x - (WND_W / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h - 100;
								upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 2, 0, 90, 150, 240, 3, true));
								break;
							case 1:
								//画面真ん中上から出てくるように
								m_Position.x = m_CameraPos.x - m_Framesplit.w / 2;
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h - 100;
								upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 2, 0, 90, 150, 240, 3, true));
								break;
							case 2:
								//画面右上から出てくるように
								m_Position.x = m_CameraPos.x + (WND_W / 2) - m_Framesplit.w;
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h - 100;
								upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 2, 0, 90, 150, 240, 3, true));
								break;
							}
						}
						//次のバレット
						m_AttackMoveCo++;
						//クールタイム
						m_BulletShotCo = 0;
						break;
					case 3:
						//円形に回るバレットが上下からくる
						for (int i = 0; i < 5; i++) {
							switch (i) {
							case 0:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							case 1:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x - (WND_W / 3) - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								//周りをまわるバレット
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							case 2:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x + (WND_W / 3) - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							case 3:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x - (WND_W / 5) - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								//周りをまわるバレット
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							case 4:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x + (WND_W / 5) - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							}
						}
						//次のバレット
						m_AttackMoveCo++;
						//クールタイム
						m_BulletShotCo = 0;
						break;
					case 4:
						//円形に回るバレットが上下からくる
						for (int i = 0; i < 5; i++) {
							switch (i) {
							case 0:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							case 1:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x - (WND_W / 3) - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								//周りをまわるバレット
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							case 2:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x + (WND_W / 3) - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							case 3:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x - (WND_W / 5) - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								//周りをまわるバレット
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							case 4:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x + (WND_W / 5) - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							}
						}
						//次のバレット
						m_AttackMoveCo++;
						//クールタイム
						m_BulletShotCo = 0;
						break;
					case 5:
						//円形に回るバレットが上下からくる
						for (int i = 0; i < 5; i++) {
							switch (i) {
							case 0:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							case 1:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x - (WND_W / 3) - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								//周りをまわるバレット
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							case 2:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x + (WND_W / 3) - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							case 3:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x - (WND_W / 5) - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								//周りをまわるバレット
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							case 4:
								//画面上から出てくるように
								m_Position.x = m_CameraPos.x + (WND_W / 5) - (m_Framesplit.w / 2);
								m_Position.y = m_CameraPos.y - (WND_H / 2) - m_Framesplit.h;
								upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 12, m_PlayerPos, 90, 300, true));
								break;
							}
						}
						//次のバレット
						m_AttackMoveCo++;
						//クールタイム
						m_BulletShotCo = 0;
						break;
					}
				}
				//攻撃回数が最大になったら
				else {
					//最初の攻撃に移る
					m_AttackMove = enAttackMove::Standby;

					//ムーブ2を繰り返さないように
					m_BulletShot = false;

					//アタック状態リセット
					m_AttackMoveCo = 0;
					//左方向を見るようにする
					m_Delection.y = 0;
					//右に配置する
					m_Position.x = m_CameraPos.x + (WND_W / 2) - 200;
					//ジャンプ中ではない
					m_Jumping = false;
				}
			}
			else {
				m_BulletShotCo++;
			}
		}
		break;
	case enBossPhase::Phase_2:
		//攻撃した回数が規定に達したら
		if (m_AttackMoveCo >= 4) {
			//少し待機
			if (m_AttackMoveChangeCo >= 60) {
				//次の場所に移動するようにする
				m_ReturnMove = true;
				//ムーブを戻す時に早めに戻るようにする
				m_AttackMoveChangeCo = 60;

				//アタック変数リセット
				m_HowShotBullet = 0;
				m_BulletShotCo = 0;
				m_AttackMoveCo = 0;
			}
			else {
				m_AttackMoveChangeCo++;
			}
		}
		//50発撃つまで続ける
		else if (m_HowShotBullet < 4) {
			//撃つ感覚
			if (m_BulletShotCo >= 3) {
				//自機狙いのバレット
				upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 8, m_PlayerPos, 40, 180, true));

				//撃ったバレットをカウント
				m_HowShotBullet++;
				//カウントリセット
				m_BulletShotCo = 0;

				//攻撃アニメーション中
				m_AttackAnimTime = true;
			}
			else {
				m_BulletShotCo++;
			}
		}
		else {
			//感覚を開けて撃つ
			m_BulletShotCo = -50;
			m_HowShotBullet = 0;

			//攻撃アニメーションを一時解除
			m_AttackAnimTime = false;

			//攻撃した回数をカウント
			m_AttackMoveCo++;
		}
		break;
	}
}

void CNazrin::RetuanMoveSet()
{
	//被弾しないようにする
	NoHit = true;

	//次の攻撃する場所に移動する動作
	m_AttackMoveChangeCo++;
	if (m_AttackMoveChangeCo >= 120) {
		//次の場所まで移動するようにする
		m_AttackMove = enAttackMove::Standby;
		//ムーブ戻し完了
		m_ReturnMove = false;

		//ジャンプをリセット
		m_Jumping = false;

		//攻撃回数カウントをリセット
		m_AttackMoveCo = 0;
		//撃った回数をリセットさせる
		m_HowShotBullet = 0;
		//カウントをリセットしておく
		m_AttackMoveChangeCo = 0;
		//バレットを撃つカウントをリセット
		m_BulletShotCo = 0;
		//バレットを撃たないように
		m_BulletShot = false;

		//ある程度移動してから判定するように
		NextSetPosBlock = false;

		//減ったHPの値をリセットしておく
		m_DecreaseHP = 0;

		//アニメーションを戻す
		m_PhaseChangeAniTime = false;
		m_AttackAnimTime = false;

		//ボスの登場した回数をカウント
		m_SetBlockPosCo++;
	}
	else if (m_AttackMoveChangeCo >= 60) {

		//ジャンプして右に移動する動作
		if (m_Jumping == false) {
			m_JumpPower = 50;
			//ジャンプするように
			m_FallingSpeed = -m_JumpPower;
			//ジャンプ中にする
			m_Jumping = true;

			//地面から離れた
			m_GroundStand = false;

			//攻撃アニメーションではなくする
			m_AttackAnimTime = false;
		}

		m_Position.x += 30;
	}
}
