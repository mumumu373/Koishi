#include "CRotateBullet.h"

CRotateBullet::CRotateBullet(int Camp, VECTOR2_f Pos, double X_Speed, double Y_Speed)
{
	//陣営セット
	m_MyCamp = Camp;
	
	//位置をセット
	m_Position = Pos;

	//速さをセット
	m_Speed = { X_Speed,Y_Speed };
	//最大速度を記憶
	m_MaxSpeed = m_Speed;

	m_FrameSize = { 32,32 };
	m_Framesplit = { 0,0,64,64 };
}

CRotateBullet::~CRotateBullet()
{
}

void CRotateBullet::Update()
{
}

void CRotateBullet::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	//CImageManagerがシングルトン化しているので、サウンドのように使える
	CImageManager::SelectImg(CImageManager::enImgList::IMG_Enemy)->TransAlBlendRotation(
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

void CRotateBullet::CharacterHit()
{
}
