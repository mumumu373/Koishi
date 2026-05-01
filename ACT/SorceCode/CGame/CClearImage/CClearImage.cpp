#include "CClearImage.h"

CClearImage::CClearImage()
	: m_Alpha(0)
	, m_Fade(true)
{
}

CClearImage::~CClearImage()
{
}

void CClearImage::Update()
{
	//フェードイン中なら
	if (m_Fade == true) {
		if (m_Alpha >= 255) {
			//クリアイメージの描画完了
			DrawClearImage = true;

			m_Alpha = 255;
		}
		else {
			m_Alpha += 3;
		}
	}
	//フェードアウト中
	else {
		if (m_Alpha <= 0) {
			FadeCompleted = true;
			m_Alpha = 0;

		}
		else {
			m_Alpha -= 3;
		}
	}
}

void CClearImage::Draw()
{
	CImageManager::SelectImg(CImageManager::enImgList::IMG_Clear)->TransAlBlend(
		0,				//表示位置x座標
		0,				//表示位置y座標
		1280,			//画像幅
		720,			//高さ	<-拡大して表示するサイズ		
		0,			//元画像x座標
		0,			//元画像y座標
		m_Alpha);					//透明度
}