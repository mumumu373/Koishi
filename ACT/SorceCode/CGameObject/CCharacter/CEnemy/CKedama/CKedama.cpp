#include "CKedama.h"

CKedama::CKedama(int Kinds, VECTOR2_f SetPos, double MoveSpeed, double JumpPower, int ChangeMoveTime, int JumpingTime)
{
	//ステージに配置する
	m_Position = SetPos;

	//色の種類
	m_Color = Kinds;
	//自分は毛玉キャラクターです
	m_MyCharacter = enMyCharacter::Kedama;
	//左に進みます
	m_MoveState = enMoveState::MoveLeft;
	//エネミー陣営です
	m_MyCamp = enMyCamp::EnemyCamp;

	//スピード
	m_Speed = { MoveSpeed, MoveSpeed };

	//ジャンプ力
	m_JumpPower = JumpPower;
	//ジャンプしているか
	m_Jumping = false;
	//ジャンプするタイミング
	m_JumpingTime = JumpingTime;
	m_JumpingCo = 0;

	//地面に立っているか
	m_GroundStand = false;
	//落下速度の初期化
	m_FallingSpeed = 0;

	StartSetting();

	//ムーブを変えるタイミングを決める
	m_ChangeMoveTime = ChangeMoveTime;
	m_ChangeMoveCo = 0;
}

CKedama::~CKedama()
{
}

void CKedama::StartSetting()
{
	m_State = enState::Living;

	switch (m_Color) {
	case enColor::NoColor:
		m_Framesplit = { 0,0,80,80 };
		break;
	case enColor::Red:
		m_Framesplit = { 32,0,100,100 };
		break;
	case enColor::Yellow:
		m_Framesplit = { 32,0,100,100 };
		break;
	case enColor::Green:
		m_Framesplit = { 32,0,100,100 };
		break;
	case enColor::Blue:
		m_Framesplit = { 0,0,200,200 };
		break;
	}
	//元画像サイズ
	m_FrameSize = { 32,32 };

	//当たり判定
	m_RealFrameSplit = { m_Framesplit.w,m_Framesplit.h };

	m_OldPosition = m_Position;
}

void CKedama::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	CImageManager::SelectImg(CImageManager::enImgList::IMG_Enemy)->TransAlBlendRotation(
		DispPos.x,				//表示位置x座標
		DispPos.y,				//表示位置y座標
		m_Framesplit.w,			//画像幅
		m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		m_Alpha, m_Delection);	//透明度、角度

	RECT rect;
	rect.left = DispPos.x;
	rect.top = DispPos.y ;
	rect.right = DispPos.x + m_RealFrameSplit.x ;
	rect.bottom = DispPos.y + m_RealFrameSplit.y;
	CStageCollisionDraw::GetInstance()->CollisionDraw(rect);

}

void CKedama::Update(std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	m_OldPosition = m_Position;

	switch (m_Color) {
	case enColor::NoColor:
		break;
	case enColor::Red:
		break;
	case enColor::Yellow:
		break;
	case enColor::Green:
		break;
	case enColor::Blue:
		break;
	}

	switch (m_MoveState) {
	case enMoveState::Wait:
		break;
	case enMoveState::MoveLeft:
		//ステージに対してその場所に行けるか判定
		m_Position.x -= m_Speed.x;
		break;
	case enMoveState::MoveRight:
		m_Position.x += m_Speed.x;
		break;
	}

	//ジャンプをさせないなら
	if (m_JumpingTime != 0) {
		//地面に立っているなら
		if (m_GroundStand == true) {
			//ジャンプするタイミングなら
			if (m_JumpingCo > m_JumpingTime) {
				//ジャンプする
				m_FallingSpeed = -m_JumpPower;
				m_JumpingCo = 0;

				//地面から離れた
				m_GroundStand = false;
			}
			else {
				m_JumpingCo++;
			}
		}
	}

	//落下するようにする
	m_Position.y += m_FallingSpeed;

	//ステージとの当たり判定を行う
	StageCollision(0,0);
}

void CKedama::Animation()
{
}

//ステージとの判定を見る
void CKedama::StageCollision(double OffsetPos_X, double OffsetPos_Y)
{
	VECTOR2_f offsetPos = { OffsetPos_X, OffsetPos_Y };

	//ブロックに触れていないなら
	if (CStageCollision::GetInstance()->IsHit(m_Position.x, m_Position.y, m_Framesplit.w, m_Framesplit.h, offsetPos) != true) {
		m_GroundStand = false;

		//常に動いているように見えるが、地面や天井の当たり判定の時に、目に見えないレベルで浮いている
		//落下速度を計算
		if (m_FallingSpeed >= MAX_FALLING_SPEED) {
			m_FallingSpeed = MAX_FALLING_SPEED;
		}
		else {
			m_FallingSpeed += Gravity;
		}
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

				//X方向に動くことができるか
				if (CStageCollision::GetInstance()->IsHit(checkPos.x, m_OldPosition.y, m_Framesplit.w, m_Framesplit.h, offsetPos) == true) {
					//std::abs(数値)：絶対値（Absolute）
					//これを使うことで、どれだけ移動するかの絶対値を見ることができるので、+や-の区分無く判定することができる
					if (std::abs(MoveRangeX) <= 1.0f) {
						MoveRangeX = 0; // 1px以下なら移動不可として終了	壁に張り付いている感じ
						m_Position.x = m_OldPosition.x;

						//ムーブ方向を変更
						m_MoveState++;

						//右方向から左へ
						if (m_MoveState > enMoveState::MoveRight) {
							m_MoveState = enMoveState::MoveLeft;
						}
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

		//まだ縦に動こうとしているなら
		if (MoveRangeY != 0.0f) {
			while (1) {
				VECTOR2_f checkPos = m_OldPosition;
				checkPos.y += MoveRangeY;

				if (CStageCollision::GetInstance()->IsHit(m_OldPosition.x, checkPos.y, m_Framesplit.w, m_Framesplit.h, offsetPos)) {

					if (std::abs(MoveRangeY) <= 1.0f) {
						//地面の判定
						if (MoveRangeY > 0) {
							if (m_GroundStand != true) {
								m_GroundStand = true;
								m_Jumping = false;
								m_FallingSpeed = 0;
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
}
