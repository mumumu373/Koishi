#include "CPlayer.h"
#include "CMouseInput//CMouseInput.h"
#include<iostream>

constexpr float PlayerCollisionW = 60.f;
constexpr float PlayerCollisionH = 100.0f;


void CPlayer::Turnaround(VECTOR2_f Pos)
{
	if (m_Position.x+m_Framesplit.w/2 < Pos.x)
	{
		m_Delection.y = 180;
	}
	else {
		m_Delection.y = 0;
	}

}

void CPlayer::Initialization()
{
	m_Position = { 0,0 };
	//初期設定でデフォルトにする
	m_Color = enColor::NoColor;
	NormalAttack = nullptr;
	NormalAttack = std::make_unique<CNormalAttack>();
	//プレイヤーのカラーをセットする
	NormalAttack->SetPlayerColor(m_Color);
	//HP
	HP = MAX_HP;
	//ハートクラス作成
	m_upHeart = nullptr;
	m_upHeart = std::make_unique<CHeart>(HP);

	enActionState = enActionState::None;
	m_MoveState = enMoveState::Wait;
	m_State=enState::Living;
	m_Delection = { 0,0,0 };
	m_JumpAcc = 0;
	m_Acceleration = {0,0};

	AvoidanceCount = -1;//回避状態を終わらせる
	AvoidanceCoolCount = AvoidancecoolTime;//回避のクールタイムを開始する

	NoHitAttackCo = 0;
	m_AttackHit = false;
	m_HitBack = false;
	m_HitBackBack = false;
	m_Delection.y = 180;
	m_WallHit = false;
	ClearGame = false;
}




CPlayer::CPlayer()
	: m_Jumping(false)
	, m_JumpPower(JUMP_POWER)
	, m_JumpAcc(0)
	, m_JumpRemove(false)
	, m_JumpRemoveCo(0)
	, m_WireShot(false)
	, m_WireShotCan(false)
	, m_Acceleration({ 0,0 })
	, AvoidanceCount(0)
	, AvoidanceCoolCount(0)
	, m_AnimationCount(0)
	, m_OldSteta(0)
	, m_MoveSpeed(0)
	, WireTopPos(0, 0)
	, OldenActionState(false)
	, AvoidanceCanCount(AvoidanceCan)
	, m_HitBack(0)
	, m_HitBackCo(0)
	, m_HitBackSpeed({ 0,0 })
	, m_HitBackBack(false)
	, m_HitBackBackCount(0)
	, m_DeathRotation(0)
	, m_DeathStop(0)
{
	StartSetting();

}

CPlayer::~CPlayer()
{
	m_upHeart.reset();
	NormalAttack.reset();
}

void CPlayer::StartWirePointCatch()
{
	enActionState = enActionState::WirePointCatch;
	
	m_JumpAcc = 0;
	m_Acceleration = { 0,0 };
	m_MoveState = enMoveState::Wait;
}

void CPlayer::StartSetting()
{
	m_State = enState::Living;

	m_FrameSize = { 64,64 };
	m_Framesplit = { 0,0,144,144 };
	m_Position = { 50,50 };

	//実際の当たり判定
	m_RealFrameSplit = { 80,80 };

	m_OldPosition = m_Position;

	//初期設定でデフォルトにする
	m_Color = enColor::NoColor;
	//自分はプレイヤーです
	m_MyCharacter = enMyCharacter::Player;
	//プレイヤー陣営です
	m_MyCamp = enMyCamp::PlayerCamp;
	//動きの状態
	m_MoveState = enMoveState::Wait;


	{
		m_leftkey[0] = false;
		m_rightkey[0] = false;
		m_leftkey[1] = false;
		m_rightkey[1] = false;
		m_Ldashcount = 0;
		m_Rdashcount = 0;
		m_Ldash = false;
		m_Rdash = false;
	}
	NormalAttack = std::make_unique<CNormalAttack>();
	//プレイヤーのカラーをセットする
	NormalAttack->SetPlayerColor(m_Color);

	m_Delection.y = 180;

	//最大HP
	MAX_HP = 200;
	//HP
	HP = MAX_HP;

	//ハートクラス作成
	m_upHeart = std::make_unique<CHeart>(HP);
	m_WallHit = false;

	//属性を変えたかを確認
	m_ChangeColor = false;

	//ステージチェンジに触れたか
	STAGE_CHANGE_HIT = false;
	//イベントに触れたか
	EVENT_HIT = false;
	//初期宣言だけはしておく
	EVENT_START_POS = m_Position;

	//ダメージ毛玉に触れたか
	DAMAGE_KEDAMA_HIT = false;
}

void CPlayer::HaveInstanceDelete()
{
	m_upHeart.reset();
	NormalAttack.reset();
}

void CPlayer::Attackmove()
{
	if (enActionState== enActionState::Attack) {
		if (NormalAttack->GetAttack() == false) {
			enActionState = enActionState::None;
		}

	}
	else {
		if (NormalAttack->GetAttack() == false) {
			if (CMouseInput::GetMouseLeft(true, true)) {
				////**音	 攻撃
				NormalAttack->Strat(GetDelectionVect(GetCenterPosition(), CMouseInput::GetMousePosCamera(m_pCamera)), GetCenterPosition());
				enActionState = enActionState::Attack;
			}

		}
	}

		
}

void CPlayer::Draw(std::unique_ptr<CCamera>& pCamera)
{

	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	CImageManager::SelectImg(CImageManager::enImgList::IMG_Koishi)->TransAlBlendRotation3(
		DispPos.x,				//表示位置x座標
		DispPos.y,				//表示位置y座標
		m_Framesplit.h,			//画像幅
		m_Framesplit.w,			//高さ	<-拡大して表示するサイズ		
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		m_Alpha, m_Delection.x, m_Delection.y, m_Delection.z);					//透明度、角度

	NormalAttack->Draw(pCamera);

	//ハートを変えたときの演出用描画
	if (m_upHeart->ChangeHeartEnd == false) {
		m_upHeart->HeartChangeDraw(pCamera, { GetPosition().x,GetPosition().y + 10 });
	}
}

void CPlayer::WireEnd(VECTOR2_f Spead)
{
	enActionState = enActionState::WireShot;
	m_Acceleration = Spead;
	m_Jumping = true;

}

void CPlayer::WireEndEnemi()
{
	enActionState = enActionState::WireShot;
	m_Jumping = true;
}

double CPlayer::GetWireStartSpeed()
{
	double Speed = m_Position.y  - m_OldPosition.y;

	if(Speed>0){
		return Speed;
	}
	return 0;
}

void CPlayer::Update(std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	if (m_State == enState::Living) {
		//ダメージ毛玉に触れたなら
		if (DAMAGE_KEDAMA_HIT == true && m_AttackHit == false && AvoidanceCount < 0) {
			PlayerDamegEriaHit(15);

			return;
		}
		else {
			DAMAGE_KEDAMA_HIT = false;
		}

		m_MoveSpeed = NoSpeed;
		if (enActionState == enActionState::WireObjectCatch) {
			m_MoveSpeed = cathiSpeed;
		}

		//過去の自分
		m_OldPosition = m_Position;
		OldGroundStand = GroundStand;

		NormalAttack->Update();

		m_WireShot = false;
		if (ClearGame == false) {
			//常にfalseにする
			GroundStand = false;
			if (enActionState != enActionState::WirePointCatch) {

				if (m_HitBack != true && m_HitBackBack != true) {
					//ワイヤーポイントを掴んでいないなら

						//プレイヤーのジャンプの制御
					JumpPlayer();
					if (enActionState != enActionState::WireObjectCatch) {
						Attackmove();//攻撃
					}


					if (AvoidanceCoolCount > 0) {

						AvoidanceCoolCount--;
					}
					else {
						if ((enActionState != enActionState::Avoidance&& enActionState != enActionState::AirAvoidance) && OldGroundStand == true) {
							KyeInput();
						}
						else {
							if ((enActionState != enActionState::AirAvoidance&& enActionState != enActionState::Avoidance) && OldGroundStand == false) {
								AirKeyInput();
							}
						}

					}

					//Attackmove();
					Dash();
					if (OldGroundStand == true) {
						m_Acceleration = { 0,0 };
						//プレイヤーの動きの制御
						MovePlayer();
					}
					if (OldGroundStand == false) {
						MovePlayerJump();
					}
					else {
						MovePlayerGround();
					}
				}
				else {
					//空中回避状態なら
					if (enActionState != enActionState::AirAvoidance) {
						//最大落下速度
						if (m_JumpAcc > MAX_FALLING_SPEED) {
							m_Position.y = MAX_FALLING_SPEED;
						}
						else {
							if (m_HitBack == true) {
								m_JumpAcc -= Gravity;
							}
							else {
								m_JumpAcc -= PlayerGrobtyi;
							}

							m_Position.y -= m_JumpAcc;
						}
					}
					m_MoveState = enMoveState::Wait;
					MovePlayerJump();
					if (m_HitBackBack) {
						if (m_HitBackBackCount <= 0) {
							m_HitBackBack = false;
							m_Acceleration = { 0,0 };
						}
						else {
							m_HitBackBackCount--;
						}

					}
					else {
						if (m_Position.y > m_OldPosition.y) {
							m_HitBack = false;
							m_Acceleration.x = 0;

						}
					}





				}
			}

			if (m_AttackHit == true) {
				//攻撃が当たらない時間を過ぎたら
				if (NoHitAttackCo == 0) {
					NoHitAttackCo = -1;
					m_AttackHit = false;
					//表示する
				}
				else {
					if (NoHitAttackCo > 0) {
						NoHitAttackCo--;
					}
				}
			}


			AvoidanceEnd();

			//プレイヤーの属性変更制御
			PlayerColorChange();

			m_WallHit = false;
			//ステージとの判定
			StageCollision(44, 44);
		}
		else {
			if (m_JumpAcc > MAX_FALLING_SPEED) {
				m_Position.y = MAX_FALLING_SPEED;
			}
			else {
				m_JumpAcc -= PlayerGrobtyi;

				m_Position.y -= m_JumpAcc;
			}
			StageCollision(44, 44);
		}
	}
	else if (m_State == enState::Dying) {
		m_WireShot = false;
		Sleep(m_DeathStop);
		m_DeathStop = 0;
		//最大落下速度
		if (m_JumpAcc > MAX_FALLING_SPEED) {
			m_Position.y = MAX_FALLING_SPEED;
		}
		else {
				m_JumpAcc -= PlayerGrobtyi;

			m_Position.y -= m_JumpAcc;
		}
		if (m_StegeUnder < m_Position.y) {//ステージの下に落ちたら
			m_State = enState::Dead;

		}
	}

}

void CPlayer::TitleSceneSet()
{
	m_JumpAcc = m_JumpPower;

	GroundStand = false;
}

void CPlayer::TitleSceneUpdate(VECTOR2_f GroundPos)
{
	//地面に合わせる
	m_Position.x = GroundPos.x + 300;
	m_Position.y = GroundPos.y + 180;

	//地面にいるぞ
	GroundStand = true;
}

void CPlayer::TitleSceneUpdate()
{
	m_OldPosition = m_Position;

	m_Position.x += 5;
	m_Position.y += -m_JumpAcc;

	m_JumpAcc -= PlayerGrobtyi;
}

void CPlayer::TitleSceneDraw()
{
	//アニメーション処理
	Animation();

	CImageManager::SelectImg(CImageManager::enImgList::IMG_Koishi)->TransAlBlendRotation3(
		m_Position.x,				//表示位置x座標
		m_Position.y,				//表示位置y座標
		m_Framesplit.h + 40,		//画像幅	少し大きめに描画する
		m_Framesplit.w + 40,		//高さ	<-拡大して表示するサイズ		
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		m_Alpha, m_Delection.x, m_Delection.y, m_Delection.z);					//透明度、角度
}

void CPlayer::MovieSceneUpdate()
{
	m_OldPosition = m_Position;

	m_AttackHit = false;	
	NoHitAttackCo =  0;

	//落下だけするように
	//最大落下速度
	if (m_JumpAcc > MAX_FALLING_SPEED) {
		m_Position.y = MAX_FALLING_SPEED;
	}
	else {
		m_JumpAcc -= Gravity;
		m_Position.y -= m_JumpAcc;
	}

	//目的位置よりも手前にいるなら
	if (m_Position.x <= EVENT_START_POS.x + 100) {
		//目的地まで移動
		m_MoveState = enMoveState::MoveRight;
		AvoidanceCount = -1;//回避状態を終わらせる
		enActionState = enActionState::None;
		m_Position.x += m_MoveSpeed;
	}
	else {
		{

		
			m_MoveState = enMoveState::Wait;
			AvoidanceCoolCount = 0;
			//m_JumpAcc = 0;
			m_Acceleration = { 0,0 };//空中の加速度をリセットする

		}
	}

	//ステージとの判定
	StageCollision(44, 44);
}

void CPlayer::StaratEnemiWire()
{
	enActionState = enActionState::WireObjectCatch;
}

void CPlayer::EndEnemiWire()
{
	enActionState = enActionState::None;
}

void CPlayer::Animation()
{
	m_Alpha = 255;
	if (m_State == enState::Living) {



		m_Framesplit.y = 0;
		if (m_OldSteta != m_MoveState || OldenActionState != enActionState) {

			m_Framesplit.x = 0;
			if ((m_MoveState == enMoveState::MoveLeft || m_MoveState == enMoveState::MoveRight) && (m_Rdash == true || m_Ldash == true)) { m_Framesplit.x = ImageSize * 4; }
			m_AnimationCount = 0;
		}
		OldenActionState = enActionState;
		m_OldSteta = m_MoveState;
		m_AnimationCount++;
		if (ClearGame==true) 
		{
			m_Framesplit.x = 0;
			m_Framesplit.y = 0;
			if (m_AnimationCount > AnimationSpeed*2) {
				
				m_Framesplit.y = ImageSize*7;
				if (m_AnimationCount > AnimationSpeed*4) {
					m_AnimationCount = 0;
				
		}
			}
			return;

		}




		if (enActionState != enActionState::WireObjectCatch) {
			switch (m_MoveState) {
			case enMoveState::Wait:
				if (m_AnimationCount > AnimationSpeed) {
					m_AnimationCount = 0;
					m_Framesplit.x += ImageSize;
					if (m_Framesplit.x > ImageSize * 3) {
						m_Framesplit.x = 0;
					}
				}

				break;
			case enMoveState::MoveLeft:
				m_Delection.y = 0;
				m_Framesplit.y = ImageSize;
				if (m_Ldash == true) {
					if (m_AnimationCount > AnimationSpeed) {
						m_AnimationCount = 0;
						m_Framesplit.x += ImageSize;
						if (m_Framesplit.x > ImageSize * 7) {
							m_Framesplit.x = ImageSize * 4;
						}

					}
				}
				else {
					if (m_AnimationCount > AnimationSpeed) {
						m_AnimationCount = 0;
						m_Framesplit.x += ImageSize;
						if (m_Framesplit.x > ImageSize * 3) {
							m_Framesplit.x = ImageSize;
						}

					}
				}

				break;
			case enMoveState::MoveRight:
				m_Delection.y = 180;
				m_Framesplit.y = ImageSize;
				if (m_Rdash == true) {
					if (m_AnimationCount > AnimationSpeed) {
						m_AnimationCount = 0;
						m_Framesplit.x += ImageSize;
						if (m_Framesplit.x > ImageSize * 7) {
							m_Framesplit.x = ImageSize * 4;
						}
					}
				}
				else {
					if (m_AnimationCount > AnimationSpeed) {
						m_AnimationCount = 0;
						m_Framesplit.x += ImageSize;
						if (m_Framesplit.x > ImageSize * 3) {
							m_Framesplit.x = 0;
						}
					}
				}
				break;
			}
		}


		if (GroundStand == false && enActionState != enActionState::WireObjectCatch) {
			m_Framesplit.y = ImageSize * 2;
			if (m_Position.y < m_OldPosition.y) {
				m_Framesplit.x = 0;
			}
			else {
				m_Framesplit.x = ImageSize;

			}
		}
		switch (enActionState) {
		case enActionState::WireShot:
		case enActionState::WirePointCatch:
			m_Framesplit.y = ImageSize * 4;
			m_Framesplit.x = 0;

			if (m_Position.x < WireTopPos.x) {
				m_Delection.y = 180;
			}
			else {
				m_Delection.y = 0;
			}
			break;
		case enActionState::Attack:
			m_Framesplit.y = ImageSize * 3;
			m_Framesplit.x = 0;
			break;
		case enActionState::WireObjectCatch:
			m_Framesplit.y = ImageSize * 3;
			if (m_Framesplit.x == 0) {
				m_Framesplit.x = ImageSize * 2;
			}

			switch (m_MoveState) {
			case enMoveState::Wait:
				m_Framesplit.x = ImageSize * 3;

				break;
			case enMoveState::MoveLeft:
				if (m_AnimationCount > AnimationSpeed) {
					m_AnimationCount = 0;
					m_Framesplit.x += ImageSize;
					if (m_Framesplit.x > ImageSize * 4) {
						m_Framesplit.x = ImageSize * 2;
					}
				}
				break;
			case enMoveState::MoveRight:
				if (m_AnimationCount > AnimationSpeed) {
					m_AnimationCount = 0;
					m_Framesplit.x += ImageSize;
					if (m_Framesplit.x > ImageSize * 4) {
						m_Framesplit.x = ImageSize * 2;
					}
				}
				break;
			}
			break;
		case enActionState::Avoidance:
			m_Framesplit.y = ImageSize * 6;
			m_Framesplit.x = 0;
			m_Alpha = 100;
			break;
		case enActionState::AirAvoidance:
			m_Framesplit.y = ImageSize * 6;
			m_Framesplit.x = 0;
			if (AirAvoidanceVECT.x == 0 && AirAvoidanceVECT.y == 0) {
				m_Alpha = 10;
			}
			else {
				m_Alpha = 100;
			}


			break;
		}
		if (m_HitBack || m_HitBackBack) {
			m_Framesplit.y = ImageSize * 5;
			m_Framesplit.x = 0;
		}
		//点滅するようにする
		if (m_AttackHit == true) {
			if (NoHitAttackCo % 28 <= 28 / 2) {
				//半透明にする
				m_Alpha = 255;
			}
			else {
				m_Alpha = 0;
			}
		}
	}
	else {
		if (m_State == enState::Dying) {
			//死んだときの回転
			m_Framesplit.y = ImageSize * 5;
			m_Framesplit.x = 0;
			m_Delection.z += DeathRotationSpeed;
			//m_Delection.z += m_DeathRotation;
			//m_DeathRotation += DeathRotationSpeed;


			if (m_DeathRotation >= DeathRotationSpeedMAX) {
				m_DeathRotation = DeathRotationSpeedMAX;
			}
		}
	}
}


//ステージとの判定を見る
void CPlayer::StageCollision(double OffsetPos_X, double OffsetPos_Y)
{
	VECTOR2_f offsetPos = { OffsetPos_X, OffsetPos_Y };

	//ブロックに触れていないなら						プレイヤーやキャラは画像の位置が少しずれているので指定して直す
	if (CStageCollision::GetInstance()->IsHit(m_Position.x, m_Position.y, 60, 100, offsetPos) != true) {
		GroundStand = false;
	}
	else {
		//動いた距離
		double MoveRangeX = m_Position.x - m_OldPosition.x;
		double MoveRangeY = m_Position.y - m_OldPosition.y;

		//まだ横に動こうとしているなら
		if (MoveRangeX != 0.0f) {
			//その場所に行けるかどうかを確認し、いけないのであれば、移動距離を減らしていってを繰り返す
			while (1) {
				VECTOR2_f checkPos = m_OldPosition;
				checkPos.x += MoveRangeX;

				//X方向に動くことができるか						プレイヤーやキャラは画像の位置が少しずれているので指定して直す
				if (CStageCollision::GetInstance()->IsHit(checkPos.x, m_OldPosition.y, 60, 100, offsetPos) == true) {
	
					//std::abs(数値)：絶対値（Absolute）
					//これを使うことで、どれだけ移動するかの絶対値を見ることができるので、+や-の区分無く判定することができる
					if (std::abs(MoveRangeX) <= 1.0f) {
						MoveRangeX = 0; // 1px以下なら移動不可として終了	壁に張り付いている感じ
						m_Position.x = m_OldPosition.x;
						m_Acceleration.x = 0;
						break;
					}

					//移動距離を 0 に近づける（後ずさりする）
					if (MoveRangeX > 0) {
						//右に動こうとしていたなら、1px 左に戻す
						MoveRangeX = MoveRangeX - 1.0;
					}
					else {
						//左に動こうとしていた（マイナスだった）なら、1px 右に戻す
						//例: -5.0 + 1.0 = -4.0 （0に近づく！）
						MoveRangeX = MoveRangeX + 1.0;
					}
					//		↓こんな方法も
					// 符号を維持したまま、絶対値を1減らす
					// 例: 5.0 -> 4.0 / -5.0 -> -4.0
					//double sign = (MoveRangeX > 0) ? 1.0 : -1.0;
					//MoveRangeX -= sign * 1.0;
				}
				//動けるなら
				else {
					m_Position.x = checkPos.x;
					break;
				}
			}
		}
		else {

			if (CStageCollision::GetInstance()->IsHit(m_Position.x, m_OldPosition.y, 60, 100, offsetPos) == true) {
				m_WallHit = true;
			}
		}

		//まだ縦に動こうとしているなら
		if (MoveRangeY != 0.0f) {
			while (1) {
				VECTOR2_f checkPos = m_OldPosition;
				checkPos.y += MoveRangeY;
				//Y方向に動けるか									プレイヤーやキャラは画像の位置が少しずれているので指定して直す
				if (CStageCollision::GetInstance()->IsHit(m_OldPosition.x, checkPos.y, 60, 100, offsetPos)) {

					if (std::abs(MoveRangeY) <= 1.0f) {
						//地面の判定
						if (MoveRangeY > 0) {
							if (GroundStand != true) {
								GroundStand = true;
								if (m_Jumping==true&&m_MoveState != enMoveState::Wait) {
									if (m_Rdash == true || m_Ldash == true) { m_Framesplit.x = ImageSize * 4; }
								}
								m_Jumping = false;
								AvoidanceCanCount = AvoidanceCan;
								m_JumpRemove = false;
								m_JumpAcc = 0;
								m_Acceleration.y = 0;
							}
						}
						//天井の判定
						else {
							m_Acceleration.y = 0;
							m_JumpAcc = 0;
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

	//イベントブロックを初期化
	EVENT_HIT = false;
	STAGE_CHANGE_HIT = false;

	//イベントブロック(ステージチェンジイベント)に触れていたら
	if (CStageCollision::GetInstance()->GetHitStageChange() == true) {
		STAGE_CHANGE_HIT = true;
	}
	//イベントブロック(ボス戦突入イベント)に触れていたら
	else if (CStageCollision::GetInstance()->GetHitEvent() == true) {
		EVENT_HIT = true;
	}
	//ダメージ毛玉に触れていたら
	else if (CStageCollision::GetInstance()->GetHitDamageKedama() == true) {
		DAMAGE_KEDAMA_HIT = true;
	}
}

void CPlayer::GetApple(VECTOR2_f Centerpos)
{
	
	m_Position.x = Centerpos.x - m_Framesplit.w / 2;
	if (ClearGame==false) {
		ClearGame = true;
		{
			//**音	クリア


			enActionState = enActionState::None;
			m_MoveState = enMoveState::Wait;
			m_State = enState::Living;
			m_Delection = { 0,0,0 };
			m_JumpAcc = 0;
			m_Acceleration = { 0,0 };

			AvoidanceCount = -1;//回避状態を終わらせる
			AvoidanceCoolCount = AvoidancecoolTime;//回避のクールタイムを開始する

			NoHitAttackCo = 0;
			m_AttackHit = false;
			m_HitBack = false;
			m_HitBackBack = false;
			m_Delection.y = 180;
			m_WallHit = false;
			
		}
	}

	
}


void CPlayer::EnemyHit(VECTOR2_f Pos, int Damage)
{
	HP -= Damage;

	//減ったHPの属性を入れる
	m_upHeart->PlayerHeartDamage(m_Color, HP);

	m_Acceleration = { 0,0 };//空中の加速度をリセットする

	//当たった場所を見る
	PlayerMyHit(Pos);
	//死んでいるなら上に飛ぶ
	Death();
}

void CPlayer::BulletHit(VECTOR2_f Pos, int Color, int Damage, bool NazrinBullet)
{
	//属性が違うならまたはナズーリンのバレットなら
	if (m_Color != Color || NazrinBullet == true) {
		//ダメージを受ける
		HP -= Damage;

		//減ったHPの属性を入れる
		m_upHeart->PlayerHeartDamage(m_Color, HP);

		//当たった場所を見る
		PlayerMyHit(Pos);
		//死んでいるなら上に飛ぶ
		Death();
	}
	//属性が一緒ならスルー出来る
}

void CPlayer::CameraCollision(VECTOR2_f CameraPos, double OffsetPos_X, double OffsetPos_Y)
{
	//判定の位置調整
	VECTOR2_f offsetPos = { OffsetPos_X, OffsetPos_Y };

	//カメラの左側の枠外に出ようとしたら
	if (m_Position.x < CameraPos.x - (WND_W / 2) - offsetPos.x) {
		m_Position.x = CameraPos.x - (WND_W / 2) - offsetPos.x;
		if (m_WallHit==true) {
			HP = 0;
			Death();
		}
	}
	//カメラの右側の枠外に出ようとしたら
	else if (m_Position.x > CameraPos.x + (WND_W / 2) - m_Framesplit.w + offsetPos.x) {
		m_Position.x = CameraPos.x + (WND_W / 2) - m_Framesplit.w + offsetPos.x;
	}
}

void CPlayer::SetStagePos(VECTOR2_f SetPos)
{
	Initialization();
	m_Position = SetPos;
	
}

void CPlayer::EventMoov(enMoveState Moov)
{


}

void CPlayer::AvoidanceEnd()
{
	if (AvoidanceCount > 0) {
		AvoidanceCount -= 1;
		
	}
	else {
		if (AvoidanceCount == 0) {
			AvoidanceCount = -1;//回避状態を終わらせる
			enActionState = enActionState::None;
			m_MoveState = enMoveState::Wait;
			AvoidanceCoolCount = AvoidancecoolTime;//回避のクールタイムを開始する
			m_JumpAcc = 0;
			m_Acceleration = { 0,0 };//空中の加速度をリセットする

			if (enActionState != enActionState::WirePointCatch) {
				enActionState = enActionState::None;
			}
		}
	}
}

void CPlayer::KyeInput()
{
	m_leftkey[1] = m_leftkey[0];
	m_leftkey[0] = GetAsyncKeyState('A') & 0x8000;

	m_rightkey[1] = m_rightkey[0];
	m_rightkey[0] = GetAsyncKeyState('D') & 0x8000;


	if (m_leftkey[0])
	{
	
		m_MoveState = enMoveState::MoveLeft;
		//シフトキーを押しているなら回避状態にする
		if (enActionState != enActionState::WireShot && enActionState != enActionState::WirePointCatch&& enActionState != enActionState::WireObjectCatch) {
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {			
				enActionState = enActionState::Avoidance;
				AvoidanceCount = AvoidanceTime;
				//**音	回避
			}
		}
		if (m_leftkey[1] == false) {
			if (m_Ldashcount>0) {
				m_Ldash = true;
			}
			else {
				m_Ldashcount = DashcountMAX;
				
			}
		}
	}
	else if (m_rightkey[0])
	{
		m_MoveState = enMoveState::MoveRight;

		//シフトキーを押しているなら回避状態にする
		if (enActionState != enActionState::WireShot && enActionState != enActionState::WirePointCatch&& enActionState != enActionState::WireObjectCatch) {
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {		
				enActionState = enActionState::Avoidance;
				AvoidanceCount = AvoidanceTime;
				//**音	回避
			}
		}
		if (m_rightkey[1] ==false) {
		
			if (m_Rdashcount > 0) {
				m_Rdash = true;
			}
			else {
				m_Rdashcount = DashcountMAX;
			}
		}
	}
	//無操作状態
	else {
		m_MoveState = enMoveState::Wait;
	}
	//ワイヤー発射指示
	if (CMouseInput::GetMouseRight(true, false)&& m_WireShotCan) {
		m_WireShot = true;
	}

}

void CPlayer::AirKeyInput()
{
	m_leftkey[1] = m_leftkey[0];
	m_leftkey[0] = GetAsyncKeyState('A') & 0x8000;

	m_rightkey[1] = m_rightkey[0];
	m_rightkey[0] = GetAsyncKeyState('D') & 0x8000;
	if (enActionState != enActionState::WireShot&& enActionState != enActionState::WirePointCatch&&enActionState != enActionState::WireObjectCatch) {
	
		if (m_leftkey[0])
		{
			m_MoveState = enMoveState::MoveLeft;
			//シフトキーを押しているなら回避状態にする

		}
		else if (m_rightkey[0])
		{
			m_MoveState = enMoveState::MoveRight;

		}
		//無操作状態
		else {
			m_MoveState = enMoveState::Wait;
		}
		if (AvoidanceCanCount>0) {
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
				//**音	回避
				AvoidanceCanCount--;
				enActionState = enActionState::AirAvoidance;
				AirAvoidanceVECTSet();
				if (AirAvoidanceVECT.x == 0&& AirAvoidanceVECT.y==0) {
					AvoidanceCount = AvoidanceTime*3;
				}
				else {
					AvoidanceCount = AvoidanceTime;
			
				}

			}
			
		}


	}
	//ワイヤー発射指示
	if (CMouseInput::GetMouseRight(true, false) && m_WireShotCan) {
		m_WireShot = true;
	}
}

void CPlayer::MovePlayer()
{
	switch (m_MoveState) {
	case enMoveState::Wait:
		if (m_Rdash==true|| m_Ldash==true) {
			m_Rdashcount = 0;
			m_Rdash = false;

			m_Ldash = false;
			m_Ldashcount = 0;
		}
		break;
	case enMoveState::MoveLeft:

		if (m_Ldash == true) {
			m_Acceleration.x -= m_MoveSpeed*2;
		}
		else {
			m_Acceleration.x -= m_MoveSpeed;
		}
		break;
	case enMoveState::MoveRight:
		if (m_Rdash == true) {
			m_Acceleration.x += m_MoveSpeed *2;
		}
		else {
			m_Acceleration.x += m_MoveSpeed;
		}

		break;
	}
}

void CPlayer::MovePlayerJump()
{
	
	switch (m_MoveState) {
	case enMoveState::Wait:
		break;
	case enMoveState::MoveLeft:
			m_Acceleration.x -= m_MoveSpeed *AirPower;
			if (-AirSpeedMAX > m_Acceleration.x) {
				m_Acceleration.x = -AirSpeedMAX;
			}
		break;
	case enMoveState::MoveRight:
			m_Acceleration.x += m_MoveSpeed * AirPower;
			if (AirSpeedMAX< m_Acceleration.x) {
				m_Acceleration.x = AirSpeedMAX;
			}
		break;
	}

	if(enActionState == enActionState::AirAvoidance) {
		VECTOR2_f SPin;
		SPin.x = AirAvoidanceVECT.x * AvoidanceDistance / AvoidanceTime;
		SPin.y = AirAvoidanceVECT.y * AvoidanceDistance / AvoidanceTime;
		m_Position.x += SPin.x;
		m_Position.y += SPin.y;
		
	}
	else {
		m_Position.x += m_Acceleration.x;
		m_Position.y += m_Acceleration.y;
		m_Acceleration.x *= 0.999;
		if (enActionState == enActionState::WireObjectCatch) {
			m_Acceleration.x *= 0.8;
		}


	}
}

void CPlayer::MovePlayerGround()
{
	if (enActionState == enActionState::Avoidance) {
		if (m_Acceleration.x <= 0) {
			//左回避状態なら
			float moveX = -(AvoidanceDistance / AvoidanceTime);
			m_Position.x += moveX;
		}
		else {
			float moveX = (AvoidanceDistance / AvoidanceTime);
			m_Position.x += moveX;
		}
		m_Position.y -= m_Acceleration.y;
	}
	else {
		m_Position.x += m_Acceleration.x;
		m_Position.y += m_Acceleration.y;
	}
}

void CPlayer::JumpPlayer()
{
	//ジャンプボタンをまだ押していなければ
	if (m_JumpRemove == false) {
		if (enActionState != enActionState::Avoidance) {
			if (GetAsyncKeyState('W') & 0x8000) {
				m_Jumping = true;	//ジャンプ中

				if (enActionState == enActionState::WireObjectCatch) {
					m_JumpAcc = m_JumpPower/2;
				}
				else {
					m_JumpAcc = m_JumpPower;
				}
			
				m_Position.y -= m_JumpAcc;


				//押している時間を図る
				if (m_JumpRemoveCo >= 10) {
					//**音	ジャンプ

					m_JumpRemove = true;	//強制的にジャンプボタンを離すようにする
					m_JumpRemoveCo = 0;
				}
				else {
					m_JumpRemoveCo++;
				}
			}
			//ジャンプボタンを離したなら
			else {
				//**音	ジャンプ


				m_JumpRemove = true;
				m_JumpRemoveCo = 0;
			}
		}

	}
	
	//空中回避状態なら
	if (enActionState != enActionState::AirAvoidance) {
		//最大落下速度
		if (m_JumpAcc > MAX_FALLING_SPEED) {
			m_Position.y = MAX_FALLING_SPEED;
		}
		else {
			if (m_HitBack == true) {
				m_JumpAcc -= Gravity;
			}
			else {
				m_JumpAcc -= PlayerGrobtyi;
			}

			m_Position.y -= m_JumpAcc;
		}
	}
}

void CPlayer::Dash()
{
	//keyinputの次に動く
	if (m_Ldashcount > 0) {
		m_Ldashcount--;

	}
	if (m_Rdashcount > 0) {
		m_Rdashcount--;

	}
	switch (m_MoveState) {
	case enMoveState::Wait:
		break;
	case enMoveState::MoveLeft:
		m_Rdashcount = 0;
		m_Rdash = false;
		break;
	case enMoveState::MoveRight:
		m_Ldashcount = 0;
		m_Ldash = false;
		break;
	}
}

void CPlayer::Death()
{
	if (HP<=0) {
		if (m_State == enState::Living) {
			//**音	死

			m_State = enState::Dying;
			m_JumpAcc = DeathSpeed;
			m_DeathRotation = 0;
			m_Delection.z = 0;
			m_DeathStop = 700;
		}
	}

}

void CPlayer::PlayerMyHit(VECTOR2_f Pos)
{
	m_JumpRemove = true;
	if (Pos.x > GetCenterPosition().x) {
		m_Acceleration.x = -10;
	}
	else {
		m_Acceleration.x = 10;
	}
	//**音	被弾（球）



	//攻撃が当たった
	m_AttackHit = true;
	//攻撃が当たらない時間のカウントをセット
	NoHitAttackCo = NoHitAttackTime;

	m_JumpAcc = 0;
	//ヒットバック準備
	m_HitBackBack = true;
	m_HitBackBackCount = 10;

	m_JumpAcc = 2;

}



void CPlayer::SetWireTopPos(VECTOR2_f TopPos)
{
	WireTopPos = TopPos;
}

void CPlayer::PlayerDamegEriaHit(int Damage)
{
	//**音	被弾（地面）

	m_JumpRemove = true;
	//攻撃が当たった
	m_AttackHit = true;
	//攻撃が当たらない時間のカウントをセット
	NoHitAttackCo = NoHitAttackTime;

	m_JumpAcc = 0;
	//ヒットバック準備
	m_HitBack = true;
	m_JumpAcc += m_HitBackCoPware;
	m_Acceleration = { 0,0 };

	HP -= Damage;
	Death();
}

void CPlayer::PlayerColorChange()
{
	//属性を変更する
	if (GetAsyncKeyState('Q') & 0x8000) {
		//色が変わっていたらそれ以降は変えない
		if (m_ChangeColor == false) {
			m_ChangeColor = true;

			m_Color--;

			//変えたときNoColorなら
			if (m_Color < enColor::NoColor) {
				//Blue(最後の列挙)にする
				m_Color = enColor::Blue;
			}

			//ハートのほうの属性も変える
			HP = m_upHeart->ChangeHeartColor(m_Color, HP);

			//攻撃の属性も変える
			NormalAttack->SetPlayerColor(m_Color);
		}
	}
	else if (GetAsyncKeyState('E') & 0x8000) {
		if (m_ChangeColor == false) {
			m_ChangeColor = true;

			m_Color++;

			if (m_Color > enColor::Blue) {
				m_Color = enColor::NoColor;
			}

			//変更後の属性ごとのHPを受け取る
			HP = m_upHeart->ChangeHeartColor(m_Color, HP);

			//攻撃の属性も変える
			NormalAttack->SetPlayerColor(m_Color);
		}
	}
	//離したら
	else {
		m_ChangeColor = false;
	}
}

void CPlayer::AirAvoidanceVECTSet()
{
	AirAvoidanceVECT = { 0,0 };

	if (GetAsyncKeyState('A') & 0x8000) {
		AirAvoidanceVECT.x = -1;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		AirAvoidanceVECT.x = 1;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		AirAvoidanceVECT.y = 1;
	}
	if (GetAsyncKeyState('W') & 0x8000) {
		AirAvoidanceVECT.y = -1;
	}
	AirAvoidanceVECT = NormalVector(AirAvoidanceVECT);

}
bool CPlayer::MoveSafeWrier(VECTOR2_f pos)
{
	VECTOR2_f offsetPos = { 40.f, 40.f };
	bool Hit = false;
	// X軸移動
	//if (moveX != 0.0f) 
	{
		VECTOR2_f nextPosX = pos;
		nextPosX.y = m_Position.y;
		if (!CStageCollision::GetInstance()->IsHit(nextPosX.x, nextPosX.y, 60, 100, offsetPos))
		{
			m_Position.x = nextPosX.x;
		}
		else
		{
			Hit = true;
			//m_Acceleration.x = 0; // 壁に当たったら速度を殺す
		}
	}

	// Y軸移動
	//if (moveY != 0.0f) 
	{
		VECTOR2_f nextPosY = pos;
		nextPosY.x = m_Position.x;
		if (!CStageCollision::GetInstance()->IsHit(nextPosY.x, nextPosY.y, 60,100, offsetPos))
		{
			m_Position.y = nextPosY.y;
		}
		else
		{
			Hit = true;
			//m_Acceleration.y = 0;
		}
	}
	return Hit;
}