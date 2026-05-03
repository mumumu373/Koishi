#include "CFairy.h"

CFairy::CFairy(int Kinds, VECTOR2_f SetPos, int Size, VECTOR2_f Speed, int MoveType, int MoveOp_1, int MoveOp_2)
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

	m_Framesplit = { 32 * m_Color,32,Size,Size };

	//スピードをセット
	m_Speed = Speed;

	//バレットの初期設定
	m_BulletShot = false;
	m_BulletShotCo = 0;
	m_BulletCo = 0;

	//攻撃を受けたときに使う変数の初期化
	m_HitBack = false;
	m_HitBackCo = 0;
	//妖精のみ、吹っ飛ぶ速度が速いようにする
	m_HitBackSpeed = { 25,25 };

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
		//打ち出すタイミングを設定
		m_BulletShotTiming = 20;
		//バレットの持続時間を設定
		m_ShotReleaseTime = 120;
		break;
	case enColor::Red:
		m_BulletShotTiming = 30;

		m_ShotReleaseTime = 120;
		break;
	case enColor::Yellow:
		m_BulletShotTiming = 40;

		m_ShotReleaseTime = 120;
		break;
	case enColor::Green:
		m_BulletShotTiming = 50;

		m_ShotReleaseTime = 180;
		break;
	case enColor::Blue:
		m_BulletShotTiming = 90;

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

	//攻撃を受けていなければ
	if (m_AttackHit == false) {
		//属性によって撃つバレットを変える
		switch (m_Color) {
		case enColor::NoColor:
			//バレットを撃ちだす動作
			m_BulletShotCo++;
			if (m_BulletShot == true) {
				m_BulletShot = false;
				m_BulletShotCo = 0;
				//まっすぐ進むバレット
				upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 10, 0, m_Delection.y, 64, m_ShotReleaseTime, 0, false));
			}
			if (m_BulletShotCo >= m_BulletShotTiming) {
				m_BulletShot = true;
			}
			break;
		case enColor::Red:
			//バレットを撃ちだす動作
			m_BulletShotCo++;
			if (m_BulletShot == true) {
				m_BulletShot = false;
				m_BulletShotCo = 0;
				//プレイヤーを狙うバレット
				upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 7, m_PlayerPos, 64, m_ShotReleaseTime, false));
			}
			if (m_BulletShotCo >= m_BulletShotTiming) {
				m_BulletShot = true;
			}
			break;
		case enColor::Yellow:
			//バレットを撃ちだす動作
			m_BulletShotCo++;
			if (m_BulletShot == true) {
				m_BulletShot = false;
				m_BulletShotCo = 0;
				//3方向に打ち出す
				for (int i = 0; i < 3; i++) {
					upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 10, i * (360 / 3), 37 * m_BulletCo, 64, m_ShotReleaseTime, 0, false));
				}

				//撃った回数をカウント
				m_BulletCo++;
			}
			if (m_BulletShotCo >= m_BulletShotTiming) {
				m_BulletShot = true;
			}
			break;
		case enColor::Green:
			//バレットを撃ちだす動作
			m_BulletShotCo++;
			if (m_BulletShot == true) {
				m_BulletShot = false;
				m_BulletShotCo = 0;
				//プレイヤーを狙うバレット
				upBullet.push_back(CBulletFactory::CreateRockOnBullet(m_MyCamp, GetCenterPosition(), m_Color, 3, m_PlayerPos, 150, m_ShotReleaseTime, false));
			}
			if (m_BulletShotCo >= m_BulletShotTiming) {
				m_BulletShot = true;
			}
			break;
		case enColor::Blue:
			//バレットを撃ちだす動作
			m_BulletShotCo++;
			if (m_BulletShot == true) {
				m_BulletShot = false;
				m_BulletShotCo = 0;
				//まっすぐ進むバレット
				for (int i = 0; i < 6; i++) {
					upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 2, i * (360 / 6), 12 * m_BulletCo, 64, m_ShotReleaseTime, 4, false));
				}

				//撃った回数をカウント
				m_BulletCo++;
			}
			if (m_BulletShotCo >= m_BulletShotTiming) {
				m_BulletShot = true;
			}
			break;
		}

		//ムーブの動作関数
		MoveControl();
	}
	//攻撃を受けたとき
	else if (m_AttackHit == true) {
		//攻撃が当たらない時間を過ぎたら
		if (NoHitAttackCo >= NoHitAttackTime) {
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

	//ヒットバック処理
	if (m_HitBack == true) {
		if (m_HitBackCo >= 10) {
			//ヒットバック終了
			m_HitBackCo = 0;
			m_HitBack = false;

			//ヒットバックが終わってから
			//体力がなくなったら
			if (HP <= 0) {
				//エネミーが死んだときの処理
				EnemyIsDead();
			}

			//吹っ飛んだ先で動くようにする
			m_MasterPosition = m_Position;
		}
		else {
			//ベクトルに変えたので、攻撃された反対の方向に飛ぶ
			m_Position.x += m_Vector.x;
			m_Position.y += m_Vector.y;

			m_HitBackCo++;
		}
	}

}

void CFairy::PlayerAttackHit(int Damage, int Color)
{
	double Speed = 0.f;	//スピード

	//攻撃が当たった
	m_AttackHit = true;
	//半透明にする
	m_Alpha = 200;

	//もしプレイヤーの属性と一致していたら
	if (Color == m_Color) {
		//確実な死を贈る
		HP = 0;

		//属性が合っていたらめっちゃぶっ飛ばすように
		Speed = m_HitBackSpeed.x * 5;

		//クリティカルヒット音をだす
		CSoundManager::PlaySE(CSoundManager::enMultiSoundList::SE_AttackCritkal);
	}
	else {
		//HPを減らす
		HP -= Damage;

		//普通のスピード
		Speed = m_HitBackSpeed.x;

		//ヒット音をだす
		CSoundManager::PlaySE(CSoundManager::enMultiSoundList::SE_AttackHit);
	}
	//攻撃が当たらない時間のカウントをセット
	NoHitAttackCo = 0;

	//ヒットバック準備
	m_HitBack = true;
	m_HitBackCo = 0;

	//攻撃した方向と逆に行くように
	m_Vector.x = cos(GetDelectionVect(GetCenterPosition(), m_PlayerPos)) * Speed;
	m_Vector.y = sin(GetDelectionVect(GetCenterPosition(), m_PlayerPos)) * Speed;
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

	if (m_AnimationCo >= m_AnimationTime) {
		m_AnimationCo = 0;

		//アニメーション
		m_Framesplit.x += 160;
		//繰り返す
		if (m_Framesplit.x >= 320) {
			m_Framesplit.x = 32 * m_Color;
		}
	}
	else {
		m_AnimationCo++;
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
