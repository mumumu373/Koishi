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
	m_BulletShotCo++;
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
		if (m_AttackMoveChangeCo >= 60) {
			//パターン1に指定
			m_AttackMove = enAttackMove::Move_01;
		}
		else {
			m_AttackMoveChangeCo++;
		}
		break;
	case enAttackMove::Move_01:
		break;
	case enAttackMove::Move_02:
		break;
	}
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
