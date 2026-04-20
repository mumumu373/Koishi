#include "CRotateBullet.h"

CRotateBullet::CRotateBullet(int Camp, VECTOR2_f Pos, int Color, double Speed, double X_Range, double Y_Range, double StartAngle, int Size, int ReleaseTime)
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
	//最大速度を記憶
	m_MaxSpeed = m_Speed;

	//中心からの位置セット
	m_Range = { X_Range ,Y_Range };
	//元の距離セット
	m_MasterRange = m_Range;

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

	//1.57 = 90度
	//角度をわかりやすくした後処理
	StartAngle /= 57.32484;
	//初期角度を決める
	m_Angle = { StartAngle,StartAngle };

	//バレットが消えるまでの時間をセット
	m_ReleaseTime = ReleaseTime;
	m_ReleaseTimeCo = 0;
}

CRotateBullet::~CRotateBullet()
{
}

void CRotateBullet::Update()
{
	switch (m_State) {
	case enState::Living:
		//まわる速度を入れる	(/100は速度調整用)
		m_Angle.x += m_Speed.x / 100;
		m_Angle.y += m_Speed.y / 100;

		//		元の位置からどれだけ離れて回るか
		m_Position.x = m_MasterPosition.x + m_Range.x * cos(m_Angle.x);
		m_Position.y = m_MasterPosition.y + m_Range.y * sin(m_Angle.y);

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

	//m_Delection = GetRadian(m_Position) * 180 / M_PI;
}

void CRotateBullet::Draw(std::unique_ptr<CCamera>& pCamera)
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

void CRotateBullet::Animation()
{
}
