#include "CEnemy.h"

CEnemy::CEnemy()
	: m_PlayerPos()
	, m_AnimationCo(0)
	, m_AnimationTime(20)	//アニメーションを変更する時間を指定
	, m_HitBack(false)
	, m_HitBackCo(0)
	, m_HitBackSpeed(0,0)	//ヒットバックする速度
{
}
