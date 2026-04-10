#include "CHeart.h"

CHeart::CHeart()
{
	m_Position = { WND_W/2,0 };

	//透明度
	m_Alpha = 255;
	//元画像サイズ
	m_FrameSize = { 64,64 };
	//描画サイズ
	m_Framesplit = { 0,0,128,128 };

	m_Position.x -= m_Framesplit.w/2;
	//m_Position.y -= m_Framesplit.h/2;
}

CHeart::~CHeart()
{
}

void CHeart::Update()
{
}

void CHeart::Draw()
{
	//アニメーション処理
	Animation();

	CImageManager::SelectImg(CImageManager::enImgList::IMG_Heart)->TransAlBlendPlas(
		m_Position.x,			//表示位置x座標
		m_Position.y,			//表示位置y座標
		m_Framesplit.w,			//画像幅
		m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		m_Alpha);				//透明度
}

void CHeart::ChangeHeartColor(int Color)
{
	m_Color = Color;

	m_Framesplit.x = 64 * m_Color;
}

void CHeart::Animation()
{
}
