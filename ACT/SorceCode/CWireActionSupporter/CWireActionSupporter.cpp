#include "CWireActionSupporter.h"

CWireActionSupporter::CWireActionSupporter()
	:m_dpPlayer(nullptr),
	 m_dpWire(nullptr),
	 m_dpWirePoint(nullptr),
	NawSpeed(0)
{
}

CWireActionSupporter::~CWireActionSupporter()
{
}

void CWireActionSupporter::StartWireAction(CPlayer*m_DPlayer,CWire* m_DPWire, CWirepoint* m_DPWirePoint)
{
	
	m_dpPlayer = m_DPlayer;
	m_dpPlayer->StartWirePointCatch();
	m_dpWire = m_DPWire;
	m_dpWirePoint = m_DPWirePoint;
}



void CWireActionSupporter::Update()
{
	
		if (m_dpPlayer != nullptr && m_dpWire != nullptr && m_dpWirePoint != nullptr) {
			if (m_dpPlayer->GetStete() == CCharacter::enState::Living)
			{
				double Long = GetHowToLong(m_dpPlayer->GetCenterPosition(), m_dpWirePoint->GetCenterPosition());
				if (Long == 0) { Long = 1; }//0で割るのはエラーになるから
				double Radian = GetDelectionVect(m_dpPlayer->GetCenterPosition(), m_dpWirePoint->GetCenterPosition());
				int kakudo = Radian *   180/ M_PI;
				kakudo +=90;
				//180度未満
				int kakudo2 = 180;

				if (GetAsyncKeyState('A') & 0x8000) {
					kakudo2 += 20;
				}
				else {
					if (GetAsyncKeyState('D') & 0x8000) {
						kakudo2 -= 20;
					}
				}
				if (kakudo< kakudo2) {
					//185度より小さい
					if (kakudo< kakudo2-5) {
						NawSpeed += Gravity;
					}
					
				}
				else {
					if (kakudo > kakudo2 + 5) {
						NawSpeed -= Gravity;
					}
					
				}
				NawSpeed *= 0.99;
				double NawSpeed2 = NawSpeed;
			
				
				double i = (double)NawSpeed2 / Long;
				Radian += i;
				double x = (m_dpWirePoint->GetCenterPosition().x )+(cos(Radian)* Long) - m_dpPlayer->GetFrameSplit().w / 2;
				double y = (m_dpWirePoint->GetCenterPosition().y )+ (sin(Radian) * Long) - m_dpPlayer->GetFrameSplit().h / 2;
				m_dpPlayer->SetPosition({x,y });

			}


		}
	}

