#include "CHeart.h"

CHeart::CHeart(int PlayerHP)
{
	m_Position = { WND_W/2,0 };

	//透明度
	m_Alpha = 255;
	//元画像サイズ
	m_FrameSize = { 64,64 };
	//描画サイズ
	m_Framesplit = { 0,0,128,128 };

	m_Position.x -= m_Framesplit.w/2;
	
	//プレイヤーのHPをセット
	for (int i = 0; i < enColor::MAX; i++) {
		m_ColorHP[i] = PlayerHP;
	}
	m_HaveHP = PlayerHP;
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

int CHeart::ChangeHeartColor(int Color, int PlayerHP)
{
	//変更前のHPを入れる
	m_ColorHP[m_Color] = PlayerHP;

	//属性変更
	m_Color = Color;

	m_Framesplit.x = 64 * m_Color;

	//属性変更後のHPを渡す
	return m_ColorHP[Color];
}

int CHeart::PlayerHeartDamage(int Color, int PlayerHP)
{
	//ダメージを受けたときにHPを代入する
	return m_ColorHP[Color] = PlayerHP;
}

void CHeart::Animation()
{
}
