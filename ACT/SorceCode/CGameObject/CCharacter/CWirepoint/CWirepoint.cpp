#include "CGameObject/CCharacter/CWirepoint/CWirepoint.h"

CWirepoint::CWirepoint(VECTOR2_f pos, int Size)
{
	m_State = enState::Living;
	m_Position = pos;
	m_Framesplit = { 0,0,size,size };
	//元画像サイズ
	m_FrameSize = { IMGSize,IMGSize };

	m_AnimationCo = 0;
}

CWirepoint::~CWirepoint()
{
}

void CWirepoint::StartSetting()
{
}

void CWirepoint::Update()
{
}

void CWirepoint::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	CImageManager::SelectImg(CImageManager::enImgList::IMG_Wirepoint)->TransAlBlendRotation3(
		DispPos.x,				//表示位置x座標
		DispPos.y,				//表示位置y座標
		m_Framesplit.w,			//画像幅
		m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		255, 
		m_Delection.x, m_Delection.y, m_Delection.z);	//透明度、角度
}

void CWirepoint::Animation()
{
	//アニメーションを追加
	if (m_AnimationCo >= 30) {
		m_AnimationCo = 0;
		m_Framesplit.x += IMGSize;
		//繰り返す
		if (m_Framesplit.x > 64) {
			m_Framesplit.x = 0;
		}
	}
	else {
		m_AnimationCo++;
	}
}
