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
			//[1]がOldPositionらしい
			pos[1] = pos[0];
			pos[0] = m_dpPlayer->GetCenterPosition();

			double Long = GetHowToLong(m_dpPlayer->GetCenterPosition(), m_dpWirePoint->GetCenterPosition());

			if (GetAsyncKeyState('W') & 0x8000) {
				Long -= 2;
			}
			else {
				if (GetAsyncKeyState('S') & 0x8000) {
					Long += 5;
					if (Long > m_dpWire->GetWireMaxRongr()) {
						Long = m_dpWire->GetWireMaxRongr();
					}
				}
			}
			if (Long <= 128) { Long = 128; }//0で割るのはエラーになるから
			double Radian = GetDelectionVect(m_dpPlayer->GetCenterPosition(), m_dpWirePoint->GetCenterPosition());

			int kakudo = (Radian * 180 / M_PI);


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
				if (diff > +5) {
					NawSpeed -= Gravity;
				}

			}

			NawSpeed *= 0.99;
			double NawSpeed2 = NawSpeed;


			double i = (double)NawSpeed2 / Long;
			Radian += i;
			double x = (m_dpWirePoint->GetCenterPosition().x) + (cos(Radian) * Long) - m_dpPlayer->GetFrameSplit().w / 2;
			double y = (m_dpWirePoint->GetCenterPosition().y) + (sin(Radian) * Long) - m_dpPlayer->GetFrameSplit().h / 2;
			m_dpPlayer->MoveSafeWrier({ x,y });

			//ステージの当たり判定を行う
			//StageCollision(40, 40);
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

//ステージとの判定を見る
void CWireActionSupporter::StageCollision(double OffsetPos_X, double OffsetPos_Y)
{
	VECTOR2_f offsetPos = { OffsetPos_X, OffsetPos_Y };

	//ブロックに触れていないなら						プレイヤーやキャラは画像の位置が少しずれているので指定して直す
	if (CStageCollision::GetInstance()->IsHit(m_dpPlayer->GetPosition().x, m_dpPlayer->GetPosition().y, 60, 100, offsetPos) != true) {
		
	}
	else {
		//動いた距離
		double MoveRangeX = m_dpPlayer->GetPosition().x - pos[1].x;
		double MoveRangeY = m_dpPlayer->GetPosition().y - pos[1].y;

		//まだ横に動こうとしているなら
		if (MoveRangeY != 0.0f) {
			//その場所に行けるかどうかを確認し、いけないのであれば、移動距離を減らしていってを繰り返す
			while (1) {
				VECTOR2_f checkPos = pos[1];
				checkPos.y += MoveRangeY;

				//Y方向に動くことができるか						プレイヤーやキャラは画像の位置が少しずれているので指定して直す
				if (CStageCollision::GetInstance()->IsHit(pos[1].y, checkPos.y, 60, 100, offsetPos) == true) {
					//std::abs(数値)：絶対値（Absolute）
					//これを使うことで、どれだけ移動するかの絶対値を見ることができるので、+や-の区分無く判定することができる
					if (std::abs(MoveRangeY) <= 1.0f) {
						MoveRangeY = 0; // 1px以下なら移動不可として終了	壁に張り付いている感じ
						m_dpPlayer->SetPos_X(pos[1].x);
						break;
					}

					//移動距離を 0 に近づける（後ずさりする）
					if (MoveRangeY > 0) {
						//右に動こうとしていたなら、1px 左に戻す
						MoveRangeY = MoveRangeY - 1.0;
					}
					else {
						//左に動こうとしていた（マイナスだった）なら、1px 右に戻す
						//例: -5.0 + 1.0 = -4.0 （0に近づく！）
						MoveRangeY = MoveRangeY + 1.0;
					}
					//		↓こんな方法も
					// 符号を維持したまま、絶対値を1減らす
					// 例: 5.0 -> 4.0 / -5.0 -> -4.0
					//double sign = (MoveRangeX > 0) ? 1.0 : -1.0;
					//MoveRangeX -= sign * 1.0;
				}
				//動けるなら
				else {
					m_dpPlayer->SetPos_X(checkPos.x);
					break;
				}
			}
		}

		//まだ縦に動こうとしているなら
		if (MoveRangeX != 0.0f) {
			while (1) {
				VECTOR2_f checkPos = pos[1];
				checkPos.x += MoveRangeX;
				//Y方向に動けるか									プレイヤーやキャラは画像の位置が少しずれているので指定して直す
				if (CStageCollision::GetInstance()->IsHit(checkPos.x, pos[1].y, 60, 100, offsetPos)) {

					if (std::abs(MoveRangeX) <= 1.0f) {
						//地面の判定
						if (MoveRangeX > 0) {
							
						}
						//天井の判定
						else {
						}

						MoveRangeX = 0; // 1px以下なら移動不可として終了	床や天井に当たったときの感じ
						m_dpPlayer->SetPos_Y(pos[1].y);
						break;
					}

					//移動距離を 0 に近づける（後ずさりする）
					//地面判定としてみることもできる
					if (MoveRangeX > 0) {
						//右に動こうとしていたなら、1px 左に戻す
						MoveRangeX = MoveRangeX - 1.0;
					}
					else {
						//左に動こうとしていた（マイナスだった）なら、1px 右に戻す
						//例: -5.0 + 1.0 = -4.0 （0に近づく！）
						MoveRangeX = MoveRangeX + 1.0;
					}

					// 符号を維持したまま、絶対値を1減らす
					// 例: 5.0 -> 4.0 / -5.0 -> -4.0
					//double sign = (MoveRangeY > 0) ? 1.0 : -1.0;
					//MoveRangeY -= sign * 1.0;
				}
				else {
					m_dpPlayer->SetPos_Y(checkPos.y);
					break;
				}
			}
		}
	}
}
