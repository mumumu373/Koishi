#include "CCollisionDetection.h"

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

CCollisionDetection::ObjectInfo CCollisionDetection::SetPlayerInfo(std::unique_ptr<CPlayer>& upPlayer, bool RealSize)
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

CCollisionDetection::ObjectInfo CCollisionDetection::SetEnemyInfo(std::unique_ptr<CEnemy>& upEnemy, bool RealSize)
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
