#include "CCharacter.h"

CCharacter::CCharacter()
	: m_Alpha(255)
	, m_Delection(0)
	, m_NoDamageCo(0)
	, m_NoDamageTime(false)
	, m_Speed(0, 0)
	, m_OldPosition(0, 0)
	, m_FrameSize(0, 0)
{
}