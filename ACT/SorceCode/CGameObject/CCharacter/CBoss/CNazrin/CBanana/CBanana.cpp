#include "CBanana.h"

CBanana::CBanana(VECTOR2_f SetPos)
{
	m_Position = SetPos;

	m_Framesplit = { 0,0,80,80 };

	m_Delection = 0;

	m_FallingSpeed = 0;
}

CBanana::~CBanana()
{
}

void CBanana::Update()
{
	//地面に設置する
	if (m_Position.y <= 720 + m_Framesplit.h - 48) {
		m_FallingSpeed += Gravity;
		m_Position.y += m_FallingSpeed;
	}
}

void CBanana::Draw(std::unique_ptr<CCamera>& pCamera)
{
	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	//CImageManagerがシングルトン化しているので、サウンドのように使える
	CImageManager::SelectImg(CImageManager::enImgList::IMG_Banana)->TransAlBlendRotation(
		DispPos.x,				//表示位置x座標
		DispPos.y,				//表示位置y座標
		m_Framesplit.w,			//画像幅
		m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		32,					//元画像xサイズ		
		32,					//元画像yサイズ
		255, m_Delection);	//透明度、角度
}

void CBanana::SetBlowOffBanana()
{
	m_FallingSpeed = 0;

	//上に飛ぶようにする
	m_FallingSpeed = -20;
}

void CBanana::BlowOffBanana()
{
	//放物線を描いて左に飛ぶようにする
	m_FallingSpeed += Gravity;
	m_Position.y += m_FallingSpeed;

	m_Position.x -= 12;

	//回転するように
	m_Delection += 5;
}
