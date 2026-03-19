#include "CWire/CWire.h"

#include "CMouseInput//CMouseInput.h"//マウス入力クラス


CWire::CWire()
{
}

CWire::~CWire()
{
}

void CWire::Shot(std::unique_ptr<CPlayer>& Player, VECTOR2_f Targetpoint)
{
	if (m_ShotState == ShotSteto::no) {
		m_Targetpoint = Targetpoint;
		m_DpPlayer = Player.get();
		m_Toptpoint.x = m_DpPlayer->GetCenterPosition().x-size/2;
		m_Toptpoint.y= m_DpPlayer->GetCenterPosition().y-size/2;

		m_Radian = GetDelectionVect (m_Targetpoint, m_DpPlayer->GetCenterPosition());
		m_ShotState = ShotSteto::forward;
}
	
}

void CWire::Update()
{
	if (m_ShotState == ShotSteto::forward) {
		m_Toptpoint.x += cos(m_Radian) * GoSpeed;
		m_Toptpoint.y += sin(m_Radian) * GoSpeed;
		
		//ワイヤーの先端がプレイヤーから目標地点より遠くなったら引き返す
		if ((GetHowToLong(m_DpPlayer->GetPosition(), m_Targetpoint) < GetHowToLong(m_DpPlayer->GetPosition(), { m_Toptpoint.x + size / 2,m_Toptpoint.y + size / 2 }))|| CMouseInput::GetMouseRight(false,true)) {
		
			m_ShotState = ShotSteto::back;
		}

	}
	else if (m_ShotState == ShotSteto::back) {
		//帰ってくる時はプレイヤーに向かって動く
		
		double Radian = GetDelectionVect(m_DpPlayer->GetCenterPosition(), { m_Toptpoint.x + size / 2,m_Toptpoint.y+ size / 2 });
		m_Toptpoint.x += cos(Radian) * ComebackSpeed;
		m_Toptpoint.y += sin(Radian) * ComebackSpeed;
		///ワイヤーの先端がプレイヤーから50px以内になったらワイヤーを消す
		int i = GetHowToLong(m_DpPlayer->GetCenterPosition(), { m_Toptpoint.x + size / 2,m_Toptpoint.y + size / 2 });
		if (i < size) {
			m_ShotState = ShotSteto::no;
		}
	}
}

void CWire::Draw(std::unique_ptr<CCamera>& pCamera)
{
	if (m_ShotState != ShotSteto::no) {
		int pieces = GetHowToLong({ m_Toptpoint.x + size / 2,m_Toptpoint.y + size / 2 }, m_DpPlayer->GetCenterPosition()) / size;
		double Radian = GetDelectionVect(m_DpPlayer->GetCenterPosition(), { m_Toptpoint.x + size / 2,m_Toptpoint.y + size / 2 });
		for (int i = 0; i < pieces; i++)
		{
			if (i == 0) {
				m_Framesplit.x = IMGSize;
			}
			else {
				m_Framesplit.x = 0;
			}
			double x = (m_Toptpoint.x )+ cos(-Radian)* size* i;
			double y = (m_Toptpoint.y )- sin(-Radian) * size * i;
			VECTOR2_f pos = { x,y };
			VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&pos);
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

void CWire::StatoWire(VECTOR2_f pos)
{
	m_ShotState = ShotSteto::Rock;
	m_Toptpoint = pos;
}
