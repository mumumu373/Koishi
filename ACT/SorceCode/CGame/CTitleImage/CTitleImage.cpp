#include "CTitleImage.h"

CTitleImage::CTitleImage()
	: m_Position(0,0)
	, m_Speed(0)
{
	//左の見えない場所に移す
	m_Position.x = -WND_W;
}

CTitleImage::~CTitleImage()
{
}

void CTitleImage::Update()
{
	//規定の場所まで移動したら
	if (m_Position.x >= SetGroundPos - 1) {
		//必ず左から入るので、ポジションを入れて動作を止める
		m_Position.x = SetGroundPos;

		//セット完了
		SetCompleted = true;
	}
	else {
		//スピードを一度リセットしてから
		m_Speed = 0;

		//中央から離れるほどスピードが上がる
		m_Speed = (SetGroundPos - m_Position.x) / 25;

		//スピード分移動
		m_Position.x += m_Speed;

		//まだセットが完了していない
		SetCompleted = false;
	}
}

void CTitleImage::Draw()
{
	CImageManager::SelectImg(CImageManager::enImgList::IMG_Title_BackSky)->TransAlBlend(
		0,				//表示位置x座標
		0,				//表示位置y座標
		1280 ,			//画像幅
		720 ,			//高さ	<-拡大して表示するサイズ		
		0,			//元画像x座標
		0,			//元画像y座標
		255);					//透明度

	CImageManager::SelectImg(CImageManager::enImgList::IMG_Title_Ground)->TransAlBlend(
		m_Position.x,		//表示位置x座標
		m_Position.y,		//表示位置y座標
		WND_W,				//画像幅
		WND_H,				//高さ	<-拡大して表示するサイズ		
		0,					//元画像x座標
		0,					//元画像y座標
		255);				//透明度
}

void CTitleImage::SetInitializePos()
{
	//規定の場所にセットする
	m_Position = { SetGroundPos,0 };

	//セット完了にする
	SetCompleted = true;
}
