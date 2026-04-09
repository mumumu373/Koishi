#include "CWireActionSupporter.h"
#include "CMouseInput//CMouseInput.h"
CWireActionSupporter::CWireActionSupporter()
	:m_dpPlayer(nullptr),
	 m_dpWire(nullptr),
	 m_dpWirePoint(nullptr),
	NawSpeed(0),
	WireActioning(false)
{
}

CWireActionSupporter::~CWireActionSupporter()
{
}

void CWireActionSupporter::StartWireAction(CPlayer*m_DPlayer,CWire* m_DPWire, CWirepoint* m_DPWirePoint)
{
	if (WireActioning == false) {
		WireActioning = true;

		pos[0] = m_DPlayer->GetCenterPosition();
		pos[1] = m_DPlayer->GetCenterPosition();

		m_dpPlayer = m_DPlayer;
		m_dpPlayer->StartWirePointCatch();
		m_dpWire = m_DPWire;
		m_dpWirePoint = m_DPWirePoint;
		NawSpeed = m_DPlayer->GetWireStartSpeed();

		double Radian = GetDelectionVect(m_dpPlayer->GetCenterPosition(), m_dpWirePoint->GetCenterPosition());

		int kakudo = (Radian * 180 / M_PI);
		//180度未満
		int kakudo2 = 90;
		int diff = kakudo - kakudo2;
		// 2. 差を -180 ~ 180 の範囲に補正する
		if (diff > 180)  diff -= 360;
		if (diff < -180) diff += 360;
		if (diff < 0) {
			NawSpeed = NawSpeed;
		}
		else {
			NawSpeed = -NawSpeed;
		}
	}
	
}



void CWireActionSupporter::Update()
{
	if (m_dpPlayer != nullptr && m_dpWire != nullptr && m_dpWirePoint != nullptr) {
		if (CMouseInput::GetMouseRight(false, false)) {
			WireActionEnd();
		}
	}


		if (m_dpPlayer != nullptr && m_dpWire != nullptr && m_dpWirePoint != nullptr) {
			if (m_dpPlayer->GetStete() == CCharacter::enState::Living)
			{

				pos[1] = pos[0];
				pos[0] = m_dpPlayer->GetCenterPosition();

				double Long = GetHowToLong(m_dpPlayer->GetCenterPosition(), m_dpWirePoint->GetCenterPosition());

				if (GetAsyncKeyState('W') & 0x8000) {
					Long -= 2;
				}
				else {
					if (GetAsyncKeyState('S') & 0x8000) {
						Long += 5;
						if (Long> m_dpWire->GetWireMaxRongr()) {
							Long = m_dpWire->GetWireMaxRongr();
						}
					}
				}
				if (Long <= 128) { Long = 128; }//0で割るのはエラーになるから
				double Radian = GetDelectionVect(m_dpPlayer->GetCenterPosition(), m_dpWirePoint->GetCenterPosition());

				if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
					int i = 0;
				}
				
				int kakudo = (Radian *   180/ M_PI);
			

				//180度未満
				int kakudo2 = 90;

			

				if (GetAsyncKeyState('A') & 0x8000) {
					kakudo2 += 20;
				}
				else {
					if (GetAsyncKeyState('D') & 0x8000) {
						kakudo2 -= 20;
					}
				}
				int diff = kakudo - kakudo2;

				// 2. 差を -180 ~ 180 の範囲に補正する
				if (diff > 180)  diff -= 360;
				if (diff < -180) diff += 360;

			
					//185度より小さい
					if (diff < -5) {
						NawSpeed += Gravity;
					}
				else {
					if (diff >  + 5) {
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

void CWireActionSupporter::WireActionEnd()
{
	int janpPware = 10;
		WireActioning = false;
	
		double Radian = GetDelectionVect( pos[0], pos[1]);//1つ前の自分の位置と今の自分の位置から角度を出す
		m_dpWire->WireEnd();
		if (NawSpeed<0) {
			NawSpeed = -NawSpeed;
		}
		m_dpPlayer->WireEnd({ cos(Radian) * NawSpeed ,sin(Radian) * NawSpeed - janpPware });
		m_dpPlayer = nullptr;
		m_dpWire = nullptr;
		m_dpWirePoint = nullptr;
		NawSpeed = 0;
}

