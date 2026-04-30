#include "CYinYangBall.h"

CYinYangBall::CYinYangBall(int Kinds, VECTOR2_f SetPos, int Size)
{
	//ステージに配置する
	m_Position = SetPos;

	//色の種類
	m_Color = Kinds;
	//自分は陰陽玉キャラクターです
	m_MyCharacter = enMyCharacter::YinYangBall;
	//左に進みます
	m_MoveState = enMoveState::MoveLeft;
	//エネミー陣営です
	m_MyCamp = enMyCamp::EnemyCamp;

	m_BulletShot = false;
	m_BulletShotCo = 0;
	m_ShotBulletAngle = 0;

	//攻撃を受けたときに使う変数の初期化
	m_HitBack = false;
	m_HitBackCo = 0;

	//ベクトル初期化
	m_Vector = { 0.f,0.f };

	//ヒットバックする速度
	m_HitBackSpeed = { 15,15 };

	StartSetting();

	//サイズを入れる
	m_Framesplit.w = Size;
	m_Framesplit.h = Size;

	//当たり判定
	m_RealFrameSplit = { m_Framesplit.w,m_Framesplit.h };

	//装飾クラスを作る
	//それぞれの個数装飾を作る
	for (int i = 0; i < m_AmountDeco; i++) {
		m_upYinYangDeco.push_back(std::make_unique<CYinYangDeco>(
			//ポジション			スピード				距離					初期角度			
			GetCenterPosition(), -18, -18, m_Framesplit.w, m_Framesplit.h, i*(360 / m_AmountDeco)));
	}
}

CYinYangBall::~CYinYangBall()
{
	//ここに装飾クラスの解放を作る
}

void CYinYangBall::StartSetting()
{
	m_State = enState::Living;

	switch (m_Color) {
		//NoColorはないが、もしなっていたら赤にする
	case enColor::NoColor:
	case enColor::Red:
		m_Framesplit = { 0,64,0,0 };
		m_Speed = { 0,0 };
		//装飾の数
		m_AmountDeco = 3;
		//打ち出すバレットの量
		m_AmountBullet = 5;
		//打ち出すタイミングを設定
		m_BulletShotTiming = 60;
		//バレットの持続時間を設定
		m_ShotReleaseTime = 60;
		break;
	case enColor::Yellow:
		m_Framesplit = { 64,64,0,0 };
		m_Speed = { 0,0 };

		m_AmountDeco = 7;

		m_AmountBullet = 4;

		m_BulletShotTiming = 60;

		m_ShotReleaseTime = 60;
		break;
	case enColor::Green:
		m_Framesplit = { 128,64,0,0 };
		m_Speed = { 0,0 };

		m_AmountDeco = 5;

		m_AmountBullet = 3;

		m_BulletShotTiming = 60;

		m_ShotReleaseTime = 60;
		break;
	case enColor::Blue:
		m_Framesplit = { 192,64,0,0 };
		m_Speed = { 0,0 };

		m_AmountDeco = 4;

		m_AmountBullet = 7;

		m_BulletShotTiming = 10;

		m_ShotReleaseTime = 60;
		break;
	}
	//元画像サイズ
	m_FrameSize = { 64,64 };

	m_OldPosition = m_Position;
}

void CYinYangBall::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//装飾も一緒に映す + 先に描画
	for (int i = 0; i < m_upYinYangDeco.size(); i++) {
		m_upYinYangDeco[i]->Draw(pCamera);
	}

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

void CYinYangBall::Update(std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	m_OldPosition = m_Position;

	if (m_AttackHit == false) {
		//バレットを撃ちだす動作
		m_BulletShotCo++;
		if (m_BulletShot == true) {
			m_BulletShot = false;
			m_BulletShotCo = 0;
			//360°方向に打てるバレットを作る
			for (int i = 0; i < m_AmountBullet; i++) {
				upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 5, i * (360 / m_AmountBullet), m_ShotBulletAngle, 64, 180, 0, false));
			}
		}
		if (m_BulletShotCo >= m_BulletShotTiming) {
			m_BulletShot = true;
		}

		switch (m_Color) {
		case enColor::NoColor:
			break;
		case enColor::Red:
			//撃つたびに角度を変える
			if (m_BulletShot == true) {
				m_ShotBulletAngle += 45;
			}
			break;
		case enColor::Yellow:
			if (m_BulletShot == true) {
				m_ShotBulletAngle += 20;
			}
			break;
		case enColor::Green:
			if (m_BulletShot == true) {
				m_ShotBulletAngle += 60;
			}
			break;
		case enColor::Blue:
			if (m_BulletShot == true) {
				m_ShotBulletAngle += 10;
			}
			break;
		}
	}

	//攻撃を受けたとき
	if (m_AttackHit == true) {
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
		}
		else {
			//ベクトルに変えたので、攻撃された反対の方向に飛ぶ
			m_Position.x += m_Vector.x;
			m_Position.y += m_Vector.y;

			m_HitBackCo++;
		}
	}

	//まわり続けるように
	m_Delection.z += 7;
}

void CYinYangBall::PlayerAttackHit(int Damage, int Color)
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
	}
	else {
		//HPを減らす
		HP -= Damage;

		//普通のスピード
		Speed = m_HitBackSpeed.x;
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

void CYinYangBall::Animation()
{
	//こっちで装飾の動作をする
	for (int i = 0; i < m_upYinYangDeco.size(); i++) {
		m_upYinYangDeco[i]->Update(GetCenterPosition());
	}
}
