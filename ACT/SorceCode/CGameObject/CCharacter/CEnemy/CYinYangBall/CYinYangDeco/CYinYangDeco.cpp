#include "CYinYangDeco.h"

CYinYangDeco::CYinYangDeco(VECTOR2_f Pos, double X_Speed, double Y_Speed, double X_Range, double Y_Range, double StartAngle)
{
	//キャラの位置
	m_NowPosition = Pos;

	m_FrameSize = { 32,32 };
	m_Framesplit = { 0,0,64,64 };

	//真ん中から出るようにする
	m_NowPosition.x -= (m_Framesplit.w / 2);
	m_NowPosition.y -= (m_Framesplit.h / 2);
	//キャラの位置を合わせる
	m_Position = m_NowPosition;
	m_MasterPosition = m_NowPosition;

	//速さをセット
	m_Speed = { X_Speed,Y_Speed };

	m_Range = { X_Range / 1.5,Y_Range / 1.5};

	//1.57 = 90度
	//角度をわかりやすくした後処理
	StartAngle /= 57.32484;
	m_Angle = { StartAngle,StartAngle };

	m_Alpha = 150;
	m_Delection = 0;
}

CYinYangDeco::~CYinYangDeco()
{
}

void CYinYangDeco::Update(VECTOR2_f YinYangPos)
{
	m_NowPosition = YinYangPos;
	//真ん中から出るようにする
	m_NowPosition.x -= (m_Framesplit.w / 2);
	m_NowPosition.y -= (m_Framesplit.h / 2);

	//まわる速度を入れる	(/100は速度調整用)
	m_Angle.x += m_Speed.x / 100;
	m_Angle.y += m_Speed.y / 100;

	//		元の位置からどれだけ離れて回るか
	m_Position.x = m_MasterPosition.x + m_Range.x * cos(m_Angle.x);
	m_Position.y = m_MasterPosition.y + m_Range.y * sin(m_Angle.y);

	//動いたときに原点だけ動くようにする
	m_Position.x -= m_MasterPosition.x - m_NowPosition.x;
	m_Position.y -= m_MasterPosition.y - m_NowPosition.y;

	//回転するように
	m_Delection += 3;
}

void CYinYangDeco::Draw(std::unique_ptr<CCamera>& pCamera)
{
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
