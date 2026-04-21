#include "CFairy.h"

CFairy::CFairy(int Kinds, VECTOR2_f SetPos, VECTOR2_f Speed, int MoveType, int MoveOp_1, int MoveOp_2)
{
	//ステージに配置する
	m_Position = SetPos;

	//色の種類
	m_Color = Kinds;
	//自分は妖精キャラクターです
	m_MyCharacter = enMyCharacter::Fairy;
	//左に進みます
	m_MoveState = enMoveState::MoveLeft;
	//エネミー陣営です
	m_MyCamp = enMyCamp::EnemyCamp;

	//スピードをセット
	m_Speed = Speed;

	//バレットの初期設定
	m_BulletShot = false;
	m_BulletShotCo = 0;

	StartSetting();

	//呼び出された位置を記憶する
	m_MasterPosition = m_Position;

	//ムーブタイプを決める
	m_MoveType = MoveType;

	//メインムーブの設定項目を入れる
	m_MoveTypeOption_1 = MoveOp_1;
	m_MoveTypeOption_2 = MoveOp_2;

	//ムーブ関連の初期設定
	m_MoveCo = 0;
	m_MovingSpeed = { 0,0 };
	m_Vector = { 0,0 };
}

CFairy::~CFairy()
{
}

void CFairy::StartSetting()
{
	m_State = enState::Living;

	switch (m_Color) {
	case enColor::NoColor:
		m_Framesplit = { 0,32,32,32 };

		//打ち出すタイミングを設定
		m_BulletShotTiming = 2;
		//バレットの持続時間を設定
		m_ShotReleaseTime = 60;
		break;
	case enColor::Red:
		m_Framesplit = { 0,32,100,100 };

		m_BulletShotTiming = 60;

		m_ShotReleaseTime = 60;
		break;
	case enColor::Yellow:
		m_Framesplit = { 32,32,32,32 };

		m_BulletShotTiming = 60;

		m_ShotReleaseTime = 60;
		break;
	case enColor::Green:
		m_Framesplit = { 32,32,100,100 };

		m_BulletShotTiming = 60;

		m_ShotReleaseTime = 60;
		break;
	case enColor::Blue:
		m_Framesplit = { 0,32,200,200 };

		m_BulletShotTiming = 60;

		m_ShotReleaseTime = 60;
		break;
	}
	//元画像サイズ
	m_FrameSize = { 32,32 };

	//当たり判定
	m_RealFrameSplit = { m_Framesplit.w / 2,m_Framesplit.h / 2};

	m_OldPosition = m_Position;
}

void CFairy::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	CImageManager::SelectImg(CImageManager::enImgList::IMG_Enemy)->TransAlBlendRotation3(
		DispPos.x,				//表示位置x座標
		DispPos.y,				//表示位置y座標
		m_Framesplit.w,			//画像幅
		m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		m_Alpha,
		m_Delection.x, m_Delection.y, m_Delection.z);	//透明度、角度
}

void CFairy::Update(std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	m_OldPosition = m_Position;

	//属性によって撃つバレットを変える
	switch (m_Color) {
	case enColor::NoColor:
		//バレットを撃ちだす動作
		m_BulletShotCo++;
		if (m_BulletShot == true) {
			m_BulletShot = false;
			m_BulletShotCo = 0;
			//プレイヤーを狙うバレット
			upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 24, m_PlayerPos, 64, 60, true));
		}
		if (m_BulletShotCo >= m_BulletShotTiming) {
			m_BulletShot = true;
		}
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

	//ムーブの動作関数
	MoveControl();

}

void CFairy::Animation()
{
	//プレイヤーの方に向く
	if (m_PlayerPos.x >= m_Position.x) {
		m_Delection.y = 180;
	}
	else {
		m_Delection.y = 0;
	}
}

void CFairy::MoveControl()
{
	//動かない設定にしているなら
	if (m_MoveType != enMoveType::Stop) {

		//ムーブタイプで動き方を変える
		switch (m_MoveType) {
			//回転
		case enMoveType::Rotation:

			//回転方向を変えれるようにする
			switch (m_MoveState) {
			case enMoveState::MoveLeft:
				//スピードを入れる
				m_MovingSpeed.x += m_Speed.x / 100;
				m_MovingSpeed.y += m_Speed.y / 100;
				break;
			case enMoveState::MoveRight:
				//スピードを入れる
				m_MovingSpeed.x -= m_Speed.x / 100;
				m_MovingSpeed.y -= m_Speed.y / 100;
				break;
			}

			//まわるように動く
			m_Position.x = m_MasterPosition.x + m_MoveTypeOption_1 * cos(m_MovingSpeed.x);
			m_Position.y = m_MasterPosition.y + m_MoveTypeOption_1 * sin(m_MovingSpeed.y);

			//もしオプション2の値が0じゃないなら動作するようにする
			if (m_MoveTypeOption_2 != 0) {
				if (m_MoveCo >= m_MoveTypeOption_2) {
					m_MoveCo = 0;

					//ムーブ方向を変更
					m_MoveState++;

					//右方向から左へ
					if (m_MoveState > enMoveState::MoveRight) {
						m_MoveState = enMoveState::MoveLeft;
					}
				}
				else {
					m_MoveCo++;
				}
			}
			break;
			//反復移動や平行移動
		case enMoveType::MovingAround:

			//ここではオプション1を使わない

			switch (m_MoveState) {
			case enMoveState::MoveLeft:
				//スピード分移動する
				m_MasterPosition.x += m_Speed.x;
				m_MasterPosition.y += m_Speed.y;
				break;
			case enMoveState::MoveRight:
				//スピード分移動する
				m_MasterPosition.x -= m_Speed.x;
				m_MasterPosition.y -= m_Speed.y;
				break;
			}

			m_Position.x = m_MasterPosition.x;
			m_Position.y = m_MasterPosition.y;

			//もしオプション2の値が0じゃないなら動作するようにする
			if (m_MoveTypeOption_2 != 0) {
				if (m_MoveCo >= m_MoveTypeOption_2) {
					m_MoveCo = 0;

					//ムーブ方向を変更
					m_MoveState++;

					//右方向から左へ
					if (m_MoveState > enMoveState::MoveRight) {
						m_MoveState = enMoveState::MoveLeft;
					}
				}
				else {
					m_MoveCo++;
				}
			}
			break;
			//突進
		case enMoveType::ChargengTarget:
			//突進するタイミングを図る
			if (m_MoveCo >= m_MoveTypeOption_2) {
				m_MoveCo = 0;

				//バレットを撃ちだす角度を計算
				m_Vector.x = cos(GetDelectionVect(m_PlayerPos, GetCenterPosition())) * m_MoveTypeOption_1;
				m_Vector.y = sin(GetDelectionVect(m_PlayerPos, GetCenterPosition())) * m_MoveTypeOption_1;
			}
			else {
				m_MoveCo++;
			}

			//ベクトルを代入
			m_MasterPosition.x += m_Vector.x;
			m_MasterPosition.y += m_Vector.y;

			//スピードが遅くなりすぎたら X方向
			if (m_Vector.x <= 0.5 && m_Vector.x > -0.5) {
				//速度を0に
				m_Vector.x = 0;
			}
			else {
				if (m_Vector.x < 0) {
					//だんだん遅くする
					m_Vector.x += m_Speed.x / 10;
				}
				else {
					m_Vector.x -= m_Speed.x / 10;
				}
			}
			//Y方向
			if (m_Vector.y <= 0.5 && m_Vector.y > -0.5) {
				m_Vector.y = 0;
			}
			else {
				if (m_Vector.y < 0) {
					//だんだん遅くする
					m_Vector.y += m_Speed.y / 10;
				}
				else {
					m_Vector.y -= m_Speed.y / 10;
				}
			}

			m_Position.x = m_MasterPosition.x;
			m_Position.y = m_MasterPosition.y;
			break;
		}
	}
	else {
		//動きを止める
		m_MoveState = enMoveState::Wait;
	}
}
