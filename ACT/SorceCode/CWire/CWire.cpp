#include "CWire/CWire.h"

#include "CMouseInput//CMouseInput.h"//マウス入力クラス
#include "CGameObject/CCharacter/CWirepoint/CWirepoint.h"
CWire::CWire()
	: m_CatchWirepoint(nullptr)
	, m_DpPlayer(nullptr)
	, m_Radian()
	, m_Toptpoint()
	, m_Targetpoint()
	, m_EnemiCatch(false)
{
}

CWire::~CWire()
{
}

void CWire::Shot(std::unique_ptr<CPlayer>& Player, VECTOR2_f Targetpoint)
{
	if (m_ShotState == ShotSteto::no) {
		m_Targetpoint=Targetpoint;
		
		m_DpPlayer = Player.get();
		m_Toptpoint.x = m_DpPlayer->GetCenterPositionDawn().x-size/2;
		m_Toptpoint.y= m_DpPlayer->GetCenterPositionDawn().y-size/2;

		m_Radian = GetDelectionVect (m_Targetpoint, m_DpPlayer->GetCenterPositionDawn());
		m_ShotState = ShotSteto::forward;
}
	
}

void CWire::Update()
{
	if (m_ShotState == ShotSteto::forward) {
		double Radian = GetDelectionVect(m_Targetpoint, { m_Toptpoint.x + size / 2,m_Toptpoint.y + size / 2 });

		m_Toptpoint.x += cos(m_Radian) * GoSpeed;
		m_Toptpoint.y += sin(m_Radian) * GoSpeed;
		
		//ワイヤーの先端がプレイヤーから目標地点より遠くなったら引き返す
		if ((GetHowToLong(m_DpPlayer->GetCenterPositionDawn(), m_Targetpoint) < GetHowToLong(m_DpPlayer->GetCenterPositionDawn(), {m_Toptpoint.x + size / 2,m_Toptpoint.y + size / 2}))
			|| CMouseInput::GetMouseRight(false, true)
			|| WireMaxRongr< GetHowToLong(m_DpPlayer->GetCenterPositionDawn(), { m_Toptpoint.x + size / 2,m_Toptpoint.y + size / 2 })) {
		
			m_ShotState = ShotSteto::back;
		}

	}
	else if (m_ShotState == ShotSteto::back) {
		//帰ってくる時はプレイヤーに向かって動く
		
		double Radian = GetDelectionVect(m_DpPlayer->GetCenterPositionDawn(), { m_Toptpoint.x + size / 2,m_Toptpoint.y+ size / 2 });
		m_Toptpoint.x += cos(Radian) * ComebackSpeed;
		m_Toptpoint.y += sin(Radian) * ComebackSpeed;
		///ワイヤーの先端がプレイヤーから50px以内になったらワイヤーを消す
		int i = GetHowToLong(m_DpPlayer->GetCenterPosition(), { m_Toptpoint.x + size / 2,m_Toptpoint.y + size / 2 });
		if (i < size) {
			m_ShotState = ShotSteto::no;
			m_Targetpoint = { 0,0 };
			m_DpPlayer = nullptr;
		}
	}
}

void CWire::Draw(std::unique_ptr<CCamera>& pCamera)
{
	if (m_ShotState != ShotSteto::no) {
	

		//ワイヤーの先端とプレイヤーの距離を測る
		int pieces = GetHowToLong(m_DpPlayer->GetCenterPositionDawn(),{ m_Toptpoint.x + size / 2,m_Toptpoint.y + size / 2 }) / (size-1);
		//ワイヤーの先端とプレイヤーの角度を測る
		double Radian = GetDelectionVect(m_DpPlayer->GetCenterPositionDawn(), { m_Toptpoint.x + size / 2,m_Toptpoint.y + size / 2 });
		for (int i = 0; i < pieces+1; i++)
		{
			if (i == 0) {
				m_Framesplit.x = IMGSize;
			}
			else {
				m_Framesplit.x = 0;
			}
			VECTOR2_f pos;
			pos .x= (m_Toptpoint.x )+( cos(Radian)* (size-1)* i);
			pos.y = (m_Toptpoint.y )+( sin(Radian) * (size-1 ) * i);
			
			VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(pos);
			CImageManager::SelectImg(CImageManager::enImgList::IMG_String)->TransAlBlendRotation(
				DispPos.x,				//表示位置x座標
				DispPos.y,				//表示位置y座標
				size,			//画像幅
				size,			//高さ	<-拡大して表示するサイズ
				m_Framesplit.x,			//元画像x座標
				m_Framesplit.y,			//元画像y座標
				IMGSize,			//元画像xサイズ		
				IMGSize,			//元画像yサイズ
				255, 180+(Radian*180/ M_PI));					//透明度、角度
		}

		
	
	}
}

void CWire::WireHandDraw(std::unique_ptr<CCamera>& pCamera, CPlayer* m_DPlayer)
{
	if (m_ShotState != ShotSteto::no&& m_EnemiCatch==false&& m_DPlayer->GetStete()==CPlayer::enState::Living) {
		//ワイヤーの先端とプレイヤーの距離を測る
		int pieces = GetHowToLong(m_DpPlayer->GetCenterPositionDawn(), { m_Toptpoint.x + size / 2,m_Toptpoint.y + size / 2 });
		if (pieces<1) {
			return;
		}
		//ワイヤーの先端とプレイヤーの角度を測る
		double Radian = GetDelectionVect(m_DpPlayer->GetCenterPositionDawn(), { m_Toptpoint.x + size / 2,m_Toptpoint.y + size / 2 });

		VECTOR2_f pos;
		int Size = m_DpPlayer->GetFrameSplit().w-55;
		pos.x = m_DpPlayer->GetCenterPositionDawn().x - Size;
		pos.y = m_DpPlayer->GetCenterPositionDawn().y - Size;

		VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(pos);
		CImageManager::SelectImg(CImageManager::enImgList::IMG_Koishi)->TransAlBlendRotation(
			DispPos.x,				//表示位置x座標
			DispPos.y,				//表示位置y座標
			Size *2,			//画像幅
			Size *2,			//高さ	<-拡大して表示するサイズ
			64,			//元a画像x座標
			64 * 4,			//元画像y座標
			64,			//元画像xサイズ		
			64,			//元画像yサイズ
			m_DpPlayer->GetAlpha(),  (Radian * 180 / M_PI));					//透明度、角度
	}

}

void CWire::StatoWire(VECTOR2_f pos)
{
	m_ShotState = ShotSteto::Rock;
	m_Toptpoint = pos;
}

bool CWire::Getcatch()
{
	if (m_ShotState == ShotSteto::forward) {
		return true;
	}
	else {
		return false;
	}
}

void CWire::Startcatch(VECTOR2_f pos)
{
	m_ShotState = ShotSteto::Rock;
	m_Toptpoint = pos;

}

bool CWire::canShot()
{
	if (m_ShotState == ShotSteto::no) {
		return true;
	}
}

void CWire::EnemiCatch()
{
	m_EnemiCatch = true;
}

void CWire::EnemiCatchEND()
{
	m_EnemiCatch = false;
}
