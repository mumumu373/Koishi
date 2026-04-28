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
			//エネミーがワイヤーに掴まれた状態になったら
			if (upEnemy[EnemyNo]->m_CatchWire == CEnemy::enCatchWire::Catch) {
				//プレイヤーが投げたら
				if (upEnemy[EnemyNo]->EnemyThrown == true) {
					//投げられたエネミーの当たり判定のセット
					ObjectInfo EnemyPos = SetEnemyInfo(upEnemy[EnemyNo], true);

					//投げられた奴に当たったエネミーを倒すようにする
					for (int EnemyToEnemyNo = 0; EnemyToEnemyNo < upEnemy.size(); EnemyToEnemyNo++) {
						//投げられたエネミーと被らないように
						if (EnemyNo != EnemyToEnemyNo) {
							//当たり判定のセット
							ObjectInfo EnemyToEnemyPos = SetEnemyInfo(upEnemy[EnemyToEnemyNo], true);

							//攻撃を始めて食らうなら
							if (upEnemy[EnemyToEnemyNo]->AttackHit == false) {
								if (CircleDetection(EnemyPos, EnemyToEnemyPos) == true) {
									//投げられたエネミーのヒットストップをする
									upEnemy[EnemyNo]->SetHitStop();
									//投げられたエネミーに当たったエネミーの処理	ダメージと位置をセット
									upEnemy[EnemyToEnemyNo]->ThrowEnemyHit(100, upEnemy[EnemyNo]->GetCenterPosition());
								}
							}
						}
					}
				}
			}
			if (upEnemy[EnemyNo]->m_State == CCharacter::enState::Living) {
				//当たり判定のセット
				ObjectInfo EnemyPos = SetEnemyInfo(upEnemy[EnemyNo], true);

				//当たったら
				if (CircleDetection(PlayerPos, EnemyPos) == true) {
					//プレイヤーがエネミーに当たったときの処理
					upPlayer->PlayerMyHit(upEnemy[EnemyNo]->GetCenterPosition());
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

void CCollisionDetection::PlayerAttackToEnemyCollision(std::unique_ptr<CNormalAttack>& upNormalAttack, std::vector<std::unique_ptr<CEnemy>>& upEnemy)
{
	//攻撃状態なら
	if (upNormalAttack->GetAttack() == true) {
		for (int AttackCollision = 0; AttackCollision < upNormalAttack->GetMAX(); AttackCollision++) {
			for (int EnemyNo = 0; EnemyNo < upEnemy.size(); EnemyNo++) {
				if (upEnemy[EnemyNo]->m_State == CCharacter::enState::Living) {
					//当たり判定のセット
					ObjectInfo EnemyPos = SetEnemyInfo(upEnemy[EnemyNo], true);

					if (upEnemy[EnemyNo]->AttackHit == false) {
						//円形と矩形の判定で見る
						if (CircleToSquareDetection(EnemyPos, upNormalAttack->GetColion(AttackCollision)) == true) {
							upEnemy[EnemyNo]->PlayerAttackHit(20, upNormalAttack->m_PlayerColor);
						}
					}
				}
			}
		}
	}
}

void CCollisionDetection::PlayerAttackToBossCollision(std::unique_ptr<CNormalAttack>& upNormalAttack, std::unique_ptr<CBoss>& upBoss)
{
	//攻撃状態なら
	if (upNormalAttack->GetAttack() == true) {
		for (int AttackCollision = 0; AttackCollision < upNormalAttack->GetMAX(); AttackCollision++) {
			if (upBoss->m_State == CCharacter::enState::Living) {
				ObjectInfo BossPos = SetBossInfo(upBoss, true);

				if (upBoss->NoHit == false) {
					if (upBoss->AttackHit == false) {
						//円形と矩形で見る
						if (CircleToSquareDetection(BossPos, upNormalAttack->GetColion(AttackCollision)) == true) {
							upBoss->PlayerAttackHit(20);
						}
					}
				}
			}
		}
	}
}

void CCollisionDetection::PlayerToBulletCollision(std::unique_ptr<CPlayer>& upPlayer, std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	if (upPlayer->m_State == CCharacter::enState::Living) {
		//当たり判定の位置情報セット
		ObjectInfo PlayerPos = SetPlayerInfo(upPlayer, true);

		//バレット
		for (int BulletNo = 0; BulletNo < upBullet.size(); BulletNo++) {
			if (upBullet[BulletNo]->m_State == CCharacter::enState::Living) {
				//陣営が違うなら
				if (upBullet[BulletNo]->m_MyCamp != upPlayer->m_MyCamp) {
					//当たり判定のセット
					ObjectInfo BulletPos = SetBulletInfo(upBullet[BulletNo]);

					//当たったら
					if (CircleDetection(PlayerPos, BulletPos) == true) {
						//プレイヤーがバレットに当たったときの処理
						upPlayer->BulletHit(upBullet[BulletNo]->m_Color, 20, upBullet[BulletNo]->m_NazrinBullet);

						//バレットがプレイヤーに当たった時の処理
						upBullet[BulletNo]->CharacterHit();
						//upPlayer->PlayerMyHit(upBullet[BulletNo]->GetCenterPosition());
					}
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

ObjectInfo CCollisionDetection::SetBulletInfo(std::unique_ptr<CBullet>& upBullet)
{
	ObjectInfo BulletInfo;

	BulletInfo = {
		//キャラクターの当たり判定
		upBullet->GetPosition().x,
		upBullet->GetPosition().y,
		upBullet->GetFrameSplit().w,
		upBullet->GetFrameSplit().h
	};

	return BulletInfo;
}

ObjectInfo CCollisionDetection::SetBossInfo(std::unique_ptr<CBoss>& upBoss, bool RealSize)
{
	ObjectInfo BossInfo;

	if (RealSize == true) {
		BossInfo = {
			//本来の当たり判定と一致させる計算
			upBoss->GetPosition().x + ((upBoss->GetFrameSplit().w - upBoss->m_RealFrameSplit.x) / 2),
			upBoss->GetPosition().y + ((upBoss->GetFrameSplit().h - upBoss->m_RealFrameSplit.y) / 2),
			upBoss->m_RealFrameSplit.x,
			upBoss->m_RealFrameSplit.y
		};
	}
	else {
		BossInfo = {
			//キャラクターの当たり判定
			upBoss->GetPosition().x,
			upBoss->GetPosition().y,
			upBoss->GetFrameSplit().w,
			upBoss->GetFrameSplit().h
		};
	}

	return BossInfo;
}
