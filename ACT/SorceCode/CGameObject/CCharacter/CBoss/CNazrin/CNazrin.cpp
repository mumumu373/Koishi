#include "CNazrin.h"

CNazrin::CNazrin()
	: m_BulletShot(false)
	, m_BulletShotCo(0)
	, m_AttackAnimCo(0)
	, m_AttackAnimTime(false)
{
	//m_Position = SetPos;
	m_Position = { 400,400 };

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

	//HPをセット
	MAX_HP = 50;
	HP = MAX_HP;

	StartSetting();
}

CNazrin::~CNazrin()
{
}

void CNazrin::StartSetting()
{
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

	//フェーズを変える変数初期化
	m_PhaseChangeCo = 0;
	m_NextPhaseSetting = false;
	//アニメーション
	m_PhaseChangeAniTime = false;

	//無敵状態変数初期化
	m_NoHit = false;

	//元居た場所を記憶する
	m_MemoryPos = m_Position;
}

void CNazrin::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

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

void CNazrin::Update(std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	m_OldPosition = m_Position;

	//次のフェーズの準備を行う
	if (m_NextPhaseSetting == true) {
		//準備完了まで無敵にする
		AttackHit = true;

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
		else {
			//フェーズ2へ移行する
			m_BossPhase = enBossPhase::Phase_2;

			//準備完了
			m_NextPhaseSetting = false;

			//向きをもとに戻す
			m_Delection.y = 0;

			//スタンバイに指定
			m_AttackMove = enAttackMove::Standby;

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
		if (AttackHit == true) {
			//攻撃が当たらない時間を過ぎたら
			if (NoHitAttackCo >= NoHitAttackTime) {
				//カウントが増えないようにする
				NoHitAttackCo = NoHitAttackTime;
				//変身中とか攻撃する非道はいないっしょ
				//AttackHit = false;
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
			//攻撃のパターン
			switch (m_AttackMove) {
			case enAttackMove::Standby:
				//ボスステージでの、ボスの配置場所に行っていないなら
				if (NextSetPosBlock == false) {
					//横に直線移動する
					m_Position.x += 10;
					m_Position.y = 0;
				}
				else {
					//カメラで見える場所まで来たら
					if (m_CameraPos.x + (WND_W / 3) >= m_Position.x) {
						//ランダムで、ムーブ1か2を選ぶ
						//m_AttackMove = (rand() & 1) + 1;
						m_AttackMove = 1;

						//カラーをランダムに変更する 0~4
						m_Color = rand() & 4;
					}
				}
				break;
			case enAttackMove::Move_01:
				//ムーブ1の動作を実行	(フェーズ2用の動作)
				BossMove_1Update(m_BossPhase, upBullet);
				break;
			case enAttackMove::Move_02:
				break;
			}
			break;
		}

		//攻撃を受けたとき
		if (AttackHit == true) {
			//攻撃が当たらない時間を過ぎたら
			if (NoHitAttackCo >= NoHitAttackTime) {
				//カウント初期化
				NoHitAttackCo = 0;
				AttackHit = false;

				//問答無用で無敵状態にする
				if (m_NoHit == true) {
					NoHitAttackCo = NoHitAttackTime;
					AttackHit = true;
				}
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

	//落下速度を入れる
	m_Position.y += m_FallingSpeed;

	StageCollision(44, 44);
}

void CNazrin::PlayerAttackHit(int Damage)
{
	//攻撃が当たった
	AttackHit = true;
	//HPを減らす
	HP -= Damage;
	//攻撃が当たらない時間のカウントをセット
	NoHitAttackCo = 0;

	//半透明にする
	m_Alpha = 200;

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
			MAX_HP = 250;
			HP = MAX_HP;		
		}
	}
}

//ボスバトルが始まった時の関数
void CNazrin::BossBattleFlag(VECTOR2_f PlayerPos)
{
	//ナズーリンの配置
	m_Position = PlayerPos;
	m_Position.x += WND_W - 300;

	//バトルが始まった時の位置を保存
	m_BattleStartPos = m_Position;

	//スタンバイ状態を早めに解いておく
	m_AttackMoveChangeCo = 160;

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

	//セットポスブロックの位置に来たら
	if (CStageCollision::GetInstance()->GetBossSetPos() == true) {
		NextSetPosBlock = true;
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
				m_Color = rand() & 4;
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
		if (MAX_HP - HP >= 50) {
			//被弾アニメーション
			m_PhaseChangeAniTime = true;
			//被弾しないようにする
			m_NoHit = true;

			//次の攻撃する場所に移動する動作
			m_AttackMoveChangeCo++;
			if (m_AttackMoveChangeCo >= 120) {
				m_Position.x += 20;
			}
			//外部からの干渉はないので==で見る
			else if (m_AttackMoveChangeCo == 60) {

				m_JumpPower = 30;
				//ジャンプするように
				m_FallingSpeed = -m_JumpPower;
				//ジャンプ中にする
				m_Jumping = true;

				//地面から離れた
				m_GroundStand = false;

				//攻撃アニメーションではなくする
				m_AttackAnimTime = false;
			}
		}
		else {
			//50発撃つまで続ける
			if (m_HowShotBullet < 50) {
				//撃つ感覚
				if (m_BulletShotCo >= 50) {
					upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 8, (60 * m_HowShotBullet), -70 + 5 * m_HowShotBullet, 80, 180, -6, false));

					//撃ったバレットをカウント
					m_HowShotBullet++;
					//フロー対策
					m_BulletShotCo = 50;
				}
				else {
					m_BulletShotCo++;
				}
			}
			else {
				//もう一度撃つようにする
				m_HowShotBullet = 0;
				m_BulletShotCo = 0;
			}
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
		break;
	}
}
