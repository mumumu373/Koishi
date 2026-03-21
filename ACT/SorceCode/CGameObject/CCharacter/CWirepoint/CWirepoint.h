#pragma once
#include "CGameObject/CCharacter/CCharacter.h"

class CWirepoint
	:public CCharacter
{
private:
	static constexpr int size = 64;
	static constexpr int IMGSize = 64;
public:
	CWirepoint(VECTOR2_f pos,int Size= size);
	
	~CWirepoint();

	 void StartSetting() override ;
	 void Update() override ;
	 void Draw(std::unique_ptr<CCamera>& pCamera) override ;
	 int GetSize() { return size; }
protected:
	void Animation() override {};
};

