#include "CKedama.h"

CKedama::CKedama(int Kinds, VECTOR2_f StagePos)
{
	m_Color = Kinds;
	//自分は毛玉キャラクターです
	m_MyCharacter = enMyCharacter::Kedama;
	//左に進みます
	m_MoveState = enMoveState::MoveLeft;

	//ステージに配置する
	m_Position = StagePos;

	StartSetting();
}

CKedama::~CKedama()
{
}

void CKedama::StartSetting()
{
	switch (m_Color) {
	case enColor::NoColor:
		m_Framesplit = { 0,0,80,80 };
		m_Speed = { 2,0 };
		break;
	case enColor::Red:
		m_Framesplit = { 0,32,100,100 };
		m_Speed = { 1,0 };
		break;
	case enColor::Blue:
		m_Framesplit = { 0,0,200,200 };
		m_Speed = { 0,0 };
		break;
	}
	//元画像サイズ
	m_FrameSize = { 32,32 };

	//当たり判定
	m_RealFrameSplit = { m_Framesplit.w,m_Framesplit.h };

	//敵陣営です
	m_MyCamp = enMyCamp::EnemyCamp;

	m_OldPosition = m_Position;
}

void CKedama::Update()
{
	m_OldPosition = m_Position;

	switch (m_Color) {
	case enColor::NoColor:
		break;
	case enColor::Red:
		break;
	case enColor::Blue:
		break;
	}

	switch (m_MoveState) {
	case enMoveState::Wait:
		break;
	case enMoveState::MoveLeft:
		m_Position.x -= m_Speed.x;
		break;
	case enMoveState::MoveRight:
		m_Position.x += m_Speed.x;
		break;
	}

}

void CKedama::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	CImageManager::SelectImg(CImageManager::enImgList::IMG_Enemy)->TransAlBlendRotation(
		DispPos.x,				//表示位置x座標
		DispPos.y,				//表示位置y座標
		m_Framesplit.w,			//画像幅
		m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		m_Alpha, m_Delection);	//透明度、角度
}

void CKedama::Animation()
{
}
