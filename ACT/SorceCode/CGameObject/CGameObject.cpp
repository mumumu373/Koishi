#include "CGameObject.h"

//ゲームオブジェで実体を作っておく
CGameObject::CGameObject()
	: m_Position(0.0,0.0)
	, m_Framesplit(0,0,0,0)
{
}

VECTOR2_f CGameObject::GetCenterPosition()
{
	VECTOR2_f CenterPos;

	CenterPos.x = m_Position.x + (m_Framesplit.w / 2);
	CenterPos.y = m_Position.y + (m_Framesplit.h / 2);

	return CenterPos;
}

VECTOR2_f CGameObject::GetCenterPositionDawn()
{
	VECTOR2_f CenterPos;

	CenterPos.x = m_Position.x + (m_Framesplit.w / 2);
	CenterPos.y = m_Position.y + (m_Framesplit.h / 2) + (m_Framesplit.h / 4)- (m_Framesplit.h / 8);

	return CenterPos;
}
