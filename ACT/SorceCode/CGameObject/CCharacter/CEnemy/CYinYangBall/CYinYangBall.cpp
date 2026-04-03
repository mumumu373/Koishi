#include "CYinYangBall.h"

CYinYangBall::CYinYangBall(int Kinds, VECTOR2_f SetPos)
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

	StartSetting();

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
}

void CYinYangBall::StartSetting()
{
	m_State = enState::Living;

	switch (m_Color) {
		//NoColorはないが、もしなっていたら赤にする
	case enColor::NoColor:
	case enColor::Red:
		m_Framesplit = { 0,64,80,80 };
		m_Speed = { 0,0 };
		//装飾の数
		m_AmountDeco = 3;
		//打ち出すバレットの量
		m_AmountBullet = 5;
		//打ち出すタイミングを設定
		m_BulletShotTiming = 60;
		break;
	case enColor::Yellow:
		m_Framesplit = { 64,64,100,100 };
		m_Speed = { 0,0 };

		m_AmountDeco = 7;

		m_AmountBullet = 4;

		m_BulletShotTiming = 60;
		break;
	case enColor::Green:
		m_Framesplit = { 128,64,100,100 };
		m_Speed = { 0,0 };

		m_AmountDeco = 5;

		m_AmountBullet = 3;

		m_BulletShotTiming = 60;
		break;
	case enColor::Blue:
		m_Framesplit = { 192,64,200,200 };
		m_Speed = { 0,0 };

		m_AmountDeco = 4;

		m_AmountBullet = 7;

		m_BulletShotTiming = 10;
		break;
	}
	//元画像サイズ
	m_FrameSize = { 64,64 };

	//当たり判定
	m_RealFrameSplit = { m_Framesplit.w,m_Framesplit.h };

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

}

void CYinYangBall::Update(std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	m_OldPosition = m_Position;

	//バレットを撃ちだす動作
	m_BulletShotCo++;
	if (m_BulletShot == true) {
		m_BulletShot = false;
		m_BulletShotCo = 0;
		for (int i = 0; i < m_AmountBullet; i++) {
			upBullet.push_back(CBulletFactory::CreateCircularBullet(m_MyCamp, GetCenterPosition(), m_Color, 6, i * (360 / m_AmountBullet), m_ShotBulletAngle, 64, 64));
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

	//こっちで装飾の動作をする
	for (int i = 0; i < m_upYinYangDeco.size(); i++) {
		m_upYinYangDeco[i]->Update(GetCenterPosition());
	}
}

void CYinYangBall::Animation()
{
	//まわり続けるように
	m_Delection += 7;
}
