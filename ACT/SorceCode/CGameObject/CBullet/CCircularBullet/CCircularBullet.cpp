#include "CCircularBullet.h"

CCircularBullet::CCircularBullet(int Camp, VECTOR2_f Pos, int Color, double Speed, double Amount, double StartAngle, int Size, int ReleaseTime, double SpeedAcc)
{
	//生存中に
	m_State = enState::Living;

	//陣営セット
	m_MyCamp = Camp;

	//位置をセット
	m_Position = Pos;

	//属性セット
	m_Color = Color;

	//速さをセット
	m_Speed = { Speed,Speed };

	m_Framesplit = { 0,0,64,64 };
	switch (m_Color) {
	case enColor::NoColor:
		break;
		//色を対応させる
	case enColor::Blue:
		m_Framesplit.x += 32;
	case enColor::Green:
		m_Framesplit.x += 32;
	case enColor::Yellow:
		m_Framesplit.x += 32;
	case enColor::Red:
		m_Framesplit.x += 32;
		break;
	}
	//元のサイズ
	m_FrameSize = { 32,32 };

	//そのキャラの真ん中から出るようにする
	m_Position.x -= (m_Framesplit.w / 2);
	m_Position.y -= (m_Framesplit.h / 2);
	//元のポジションをセットする
	m_MasterPosition = m_Position;

	//角度を保存しておく
	m_Amount = Amount;
	m_StartAngle = StartAngle;

	//バレットを撃ちだす角度を計算
	m_Vector.x = cos((m_Amount + m_StartAngle) / 180 * M_PI) * m_Speed.x;
	m_Vector.y = sin((m_Amount + m_StartAngle) / 180 * M_PI) * m_Speed.y;

	//バレットが消えるまでの時間をセット
	m_ReleaseTime = ReleaseTime;
	m_ReleaseTimeCo = 0;

	//加速度をセット
	m_SpeedAcc = SpeedAcc / 10;
}

CCircularBullet::~CCircularBullet()
{
}

void CCircularBullet::Update()
{
	switch (m_State) {
	case enState::Living:
		//ベクトルを入れる
		m_Position.x += m_Vector.x;
		m_Position.y += m_Vector.y;

		//0であれば動作させない	(引数で0以下の数字を渡しても、/10しているので通らないようになる)
		if (m_SpeedAcc!=0.0f) {
		//だんだん速度を変えるようにする
			m_Speed.x += m_SpeedAcc;
			m_Speed.y += m_SpeedAcc;
			m_Vector.x = cos(m_Amount + m_StartAngle / 180 * M_PI) * m_Speed.x;
			m_Vector.y = sin(m_Amount + m_StartAngle / 180 * M_PI) * m_Speed.y;
		}

		//決めた時間になったら消す
		if (m_ReleaseTimeCo >= m_ReleaseTime) {
			m_State = enState::Dead;
		}
		else {
			m_ReleaseTimeCo++;
		}
		break;
	case enState::Dead:
		break;
	}
}

void CCircularBullet::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	//CImageManagerがシングルトン化しているので、サウンドのように使える
	CImageManager::SelectImg(CImageManager::enImgList::IMG_Bullet)->TransAlBlendRotation(
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

void CCircularBullet::CharacterHit()
{
}

void CCircularBullet::Animation()
{
}
