#include "CGameObject.h"

//ゲームオブジェで実体を作っておく
CGameObject::CGameObject()
	: m_pImg(nullptr)
	, m_Position(0.0,0.0)
	, m_Framesplit(0,0,0,0)
{
}

VECTOR2_f CGameObject::GetPosition()
{
	VECTOR2_f CenterPos;

	CenterPos.x = m_Position.x + (m_Framesplit.w / 2);
	CenterPos.y = m_Position.y + (m_Framesplit.h / 2);

	return CenterPos;
}
