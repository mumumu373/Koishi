#include "CCollisionDetection.h"

#include <CMouseInput/CMouseInput.h>
CCollisionDetection::CCollisionDetection()
{
}

CCollisionDetection::~CCollisionDetection()
{
}

void CCollisionDetection::Update()
{
}

void CCollisionDetection::PlayerToEnemyCollision(std::unique_ptr<CPlayer>& upPlayer, std::vector<std::unique_ptr<CEnemy>>& upEnemy)
{
	if (upPlayer->m_State == CCharacter::enState::Living) {
		//当たり判定の位置情報セット
		ObjectInfo PlayerPos = SetPlayerInfo(upPlayer, true);

		//エネミー
		for (int EnemyNo = 0; EnemyNo < upEnemy.size(); EnemyNo++) {
			if (upEnemy[EnemyNo]->m_State == CCharacter::enState::Living) {
				//当たり判定のセット
				ObjectInfo EnemyPos = SetEnemyInfo(upEnemy[EnemyNo], true);

				//当たったら
				if (CircleDetection(PlayerPos, EnemyPos) == true) {
					//プレイヤーがエネミーに当たったときの処理
					upPlayer->EnemyHit(upEnemy[EnemyNo]->m_MyCharacter, upEnemy[EnemyNo]->m_Color);
				}
			}
		}
	}
}

void CCollisionDetection::MouseToEnemyCollision(std::vector<std::unique_ptr<CEnemy>>& upEnemy, std::unique_ptr<CCamera>& Camera)
{
	//エネミー
	for (int EnemyNo = 0; EnemyNo < upEnemy.size(); EnemyNo++) {
		if (upEnemy[EnemyNo]->m_State == CCharacter::enState::Living) {
			//当たり判定のセット
			ObjectInfo EnemyPos = SetEnemyInfo(upEnemy[EnemyNo], true);


			//当たったら
			if (CircleDetection({ CMouseInput::GetMousePosCamera(Camera.get()).x,CMouseInput::GetMousePosCamera(Camera.get()).y,1,1 }, EnemyPos) == true) {
				VECTOR2_f i;
				i = Camera->CalcToPositionInCamera(upEnemy[EnemyNo]->GetCenterPosition());
				CMouseInput::SetMousePos(i);
				CMouseInput::ColorChange();
			}
		}
	}

}

void CCollisionDetection::MouseToWirePoint(std::vector<std::unique_ptr<CWirepoint>>& upEnemy, std::unique_ptr<CCamera>& Camera)
{
	//エネミー
	for (int EnemyNo = 0; EnemyNo < upEnemy.size(); EnemyNo++) {
		if (upEnemy[EnemyNo]->m_State == CCharacter::enState::Living) {
			//当たり判定のセット
			ObjectInfo EnemyPos;
			EnemyPos.x = upEnemy[EnemyNo]->GetPosition().x;
			EnemyPos.y = upEnemy[EnemyNo]->GetPosition().y;
			EnemyPos.xw = upEnemy[EnemyNo]->GetFrameSplit().w;
			EnemyPos.yh = upEnemy[EnemyNo]->GetFrameSplit().h;
			Camera.get();
			//当たったら
				if (CircleDetection( CMouseInput::GetcollisionMouseCamera(Camera.get()), EnemyPos) == true) {
				VECTOR2_f i;
				i = Camera->CalcToPositionInCamera(upEnemy[EnemyNo]->GetCenterPosition());
				CMouseInput::SetMousePos(i);
				CMouseInput::ColorChange();
			}
		}
	}
}

void CCollisionDetection::WireToWirepointCollision(std::vector<std::unique_ptr<CWirepoint>>& m_pCWirepoint, std::unique_ptr<CWire>& pWire, std::unique_ptr<CPlayer>& upPlayer, std::unique_ptr<CWireActionSupporter>& m_upWireActionSupporter)
{
	if (pWire->Getcatch()) {
		for (int EnemyNo = 0; EnemyNo < m_pCWirepoint.size(); EnemyNo++) {
			if (m_pCWirepoint[EnemyNo]->m_State == CCharacter::enState::Living) {
				//当たり判定のセット
				ObjectInfo EnemyPos;
				EnemyPos.x = m_pCWirepoint[EnemyNo]->GetPosition().x;
				EnemyPos.y = m_pCWirepoint[EnemyNo]->GetPosition().y;
				EnemyPos.xw = m_pCWirepoint[EnemyNo]->GetSize();
				EnemyPos.yh = m_pCWirepoint[EnemyNo]->GetSize();

				//当たったら
				if (CircleDetection({ pWire->GetTopPoint().x, pWire->GetTopPoint().y, pWire->GetSize(), pWire->GetSize() }, EnemyPos) == true) {

					pWire->Startcatch({ m_pCWirepoint[EnemyNo]->GetCenterPosition().x - pWire->GetSize() / 2, m_pCWirepoint[EnemyNo]->GetCenterPosition().x - pWire->GetSize() / 2});
					m_upWireActionSupporter->StartWireAction(upPlayer.get(), pWire.get(), m_pCWirepoint[EnemyNo].get());
				}
			}
		}
	}	
}

void CCollisionDetection::WireToEnemyCollision(std::vector<std::unique_ptr<CEnemy>>& upEnemy, std::unique_ptr<CWire>& pWire, std::unique_ptr<CPlayer>& upPlayer, std::unique_ptr<CWireActionSupporter>& m_upWireActionSupporter)
{
	if (pWire->Getcatch()) {
		for (int EnemyNo = 0; EnemyNo < upEnemy.size(); EnemyNo++) {
			if (upEnemy[EnemyNo]->m_State == CCharacter::enState::Living) {
				//当たり判定のセット
				ObjectInfo EnemyPos;
				EnemyPos.x = upEnemy[EnemyNo]->GetPosition().x;
				EnemyPos.y = upEnemy[EnemyNo]->GetPosition().y;
				EnemyPos.xw = upEnemy[EnemyNo]->GetFrameSplit().w;
				EnemyPos.yh = upEnemy[EnemyNo]->GetFrameSplit().h;

				//当たったら
				if (CircleDetection({ pWire->GetTopPoint().x, pWire->GetTopPoint().y, pWire->GetSize(), pWire->GetSize() }, EnemyPos) == true) {

					pWire->Startcatch({ upEnemy[EnemyNo]->GetCenterPosition().x - pWire->GetSize() / 2, upEnemy[EnemyNo]->GetCenterPosition().x - pWire->GetSize() / 2 });
					m_upWireActionSupporter->StartWireActionEnemi(upPlayer.get(), pWire.get(), upEnemy[EnemyNo].get());

				}
			}
		}
	}

}



ObjectInfo CCollisionDetection::SetPlayerInfo(std::unique_ptr<CPlayer>& upPlayer, bool RealSize)
{
	ObjectInfo PlayerInfo;

	if (RealSize == true) {
		PlayerInfo = {
			//本来の当たり判定と一致させる計算
			upPlayer->GetPosition().x + ((upPlayer->GetFrameSplit().w - upPlayer->m_RealFrameSplit.x) / 2),
			upPlayer->GetPosition().y + ((upPlayer->GetFrameSplit().h - upPlayer->m_RealFrameSplit.y) / 2),
			upPlayer->m_RealFrameSplit.x,
			upPlayer->m_RealFrameSplit.y
		};
	}
	else {
		PlayerInfo = {
			//キャラクターの当たり判定
			upPlayer->GetPosition().x,
			upPlayer->GetPosition().y,
			upPlayer->GetFrameSplit().w,
			upPlayer->GetFrameSplit().h
		};
	}

	return PlayerInfo;
}

ObjectInfo CCollisionDetection::SetEnemyInfo(std::unique_ptr<CEnemy>& upEnemy, bool RealSize)
{
	ObjectInfo EnemyInfo;

	if (RealSize == true) {
		EnemyInfo = {
			//本来の当たり判定と一致させる計算
			upEnemy->GetPosition().x + ((upEnemy->GetFrameSplit().w - upEnemy->m_RealFrameSplit.x) / 2),
			upEnemy->GetPosition().y + ((upEnemy->GetFrameSplit().h - upEnemy->m_RealFrameSplit.y) / 2),
			upEnemy->m_RealFrameSplit.x,
			upEnemy->m_RealFrameSplit.y
		};
	}
	else {
		EnemyInfo = {
			//キャラクターの当たり判定
			upEnemy->GetPosition().x,
			upEnemy->GetPosition().y,
			upEnemy->GetFrameSplit().w,
			upEnemy->GetFrameSplit().h
		};
	}

	return EnemyInfo;
}
