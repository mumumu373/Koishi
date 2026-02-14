#include "CCollisionDetection.h"

CCollisionDetection::CCollisionDetection()
	: m_pPlayer(nullptr)
	, m_pEnemy(nullptr)
{
}

CCollisionDetection::~CCollisionDetection()
{
	m_pPlayer = nullptr;
	m_pEnemy = nullptr;
}

void CCollisionDetection::Update()
{
	//キャラクター同士の当たり判定
	CharacterCollision();
}

void CCollisionDetection::CharacterCollision()
{
	//位置情報セット
	ObjectInfo PlayerInfo = SetCharacterInfo(m_pPlayer);
	ObjectInfo EnemyInfo = SetCharacterInfo(m_pEnemy);

	//プレイヤーとエネミーの当たり判定
	if (m_pPlayer->m_State == CCharacter::enState::Living) {
		if (m_pEnemy->m_State == CCharacter::enState::Living) {
			//無敵中で無いなら
			if (m_pPlayer->m_NoDamageTime == false) {
				//当たったら
				if (CircleDetection(EnemyInfo, PlayerInfo) == true) {
					//ダメージ量
					m_pPlayer->EnemyHit(50);
				}
			}
		}
	}
}

CCollisionDetection::ObjectInfo CCollisionDetection::SetCharacterInfo(CCharacter* pCharacter)
{
	ObjectInfo CharacterInfo = {
		pCharacter->GetPosition().x + ((pCharacter->GetFrameSplit().w - pCharacter->m_RealFrameSplit.x) / 2),
		pCharacter->GetPosition().y + ((pCharacter->GetFrameSplit().h - pCharacter->m_RealFrameSplit.y) / 2),
		pCharacter->m_RealFrameSplit.x,
		pCharacter->m_RealFrameSplit.y
	};

	return CharacterInfo;
}
