#pragma once
#include "CGameObject/CCharacter/CBoss/CNazrin/CNazrin.h"		//ナズーリン

/************************************************************************************************
*		ボスを作る場所
*/
class CBossFactory
{
public:
	CBossFactory();
	~CBossFactory();
	
	//1面ボス	ナズーリン作成
	static inline std::unique_ptr<CBoss> CreateNazrin()
	{
		return std::make_unique<CNazrin>();
	}

private:

};