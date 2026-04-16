#include "CNazrin.h"

CNazrin::CNazrin()
	: m_BulletShot(false)
	, m_BulletShotCo(0)
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

	//地面に立っているか
	m_GroundStand = false;
	//落下速度の初期化
	m_FallingSpeed = 0;

	//カウント初期化
	m_AttackMoveChangeCo = 0;
	//バレット撃った回数を初期化
	m_HowShotBullet = 0;
}

void CNazrin::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	//CImageManagerがシングルトン化しているので、サウンドのように使える
	CImageManager::SelectImg(CImageManager::enImgList::IMG_Nazrin)->TransAlBlendRotation(
		DispPos.x,				//表示位置x座標
		DispPos.y,				//表示位置y座標
		m_Framesplit.w,			//画像幅
		m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		m_Alpha, m_Delection);					//透明度、角度
}

void CNazrin::Update(std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	m_OldPosition = m_Position;

	//バレットを撃ちだす動作
	if (m_BulletShot == false) {
		m_BulletShot = true;
		m_BulletShotCo = 0;
		//周りをまわる弾を生成
		upBullet.push_back(CBulletFactory::CreateRotateBullet(m_MyCamp, GetCenterPosition(), Blue, 3, 100, 100, 180, 64, 240));
	}
	else if (m_BulletShotCo >= 60) {
		m_BulletShot = false;
	}

	switch (m_AttackMove) {
	case enAttackMove::Standby:
		//攻撃パターンを変えるとき
		if (m_AttackMoveChangeCo >= 300) {
			//パターン1に指定
			m_AttackMove = enAttackMove::Move_01;
		}
		else {
			m_AttackMoveChangeCo++;
		}
		break;
	case enAttackMove::Move_01:
		m_BulletShotCo++;
		if (m_BulletShot == false) {

		}
		else if (m_BulletShotCo >= 15) {
			m_BulletShot = false;
		}

		break;
	case enAttackMove::Move_02:
		break;
	}

	//落下速度を入れる
	m_Position.y += m_FallingSpeed;

	StageCollision(44, 44);
}

void CNazrin::BossBattleFlag(VECTOR2_f PlayerPos)
{
	//ナズーリンの配置
	m_Position = PlayerPos;
	m_Position.x += WND_W - 300;

	//スタンバイ状態を早めに解いておく
	m_AttackMoveChangeCo = 240;

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
	m_AnimetionCo++;

	if (m_AnimetionCo >= 15) {
		m_Framesplit.x += 64;
		if (m_Framesplit.x > 64*3) {
			m_Framesplit.x = 0;
		}
		m_AnimetionCo = 0;
	}
}

void CNazrin::StageCollision(double OffsetPos_X, double OffsetPos_Y)
{
	VECTOR2_f offsetPos = { OffsetPos_X, OffsetPos_Y };

	//ブロックに触れていないなら			判定のサイズをナズーリンの大きさに合わせる
	if (CStageCollision::GetInstance()->IsHit(m_Position.x, m_Position.y, 60, 100, offsetPos) != true) {
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
							m_FallingSpeed = 0;
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
