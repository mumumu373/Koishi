#pragma once
#include "CGameObject/CStage/CStageCollision/CStageCollision.h"	//ステージ当たり判定クラス
#include "CGameObject/CStage/CStageLoader/CStageLoader.h"		//ステージ読み込み定クラス
#include "CGameObject/CStage/CStageResource/CStageResource.h"	//ステージ読み込み定クラス
#include "CGameObject/CStage/CStage.h"						//ステージクラス
#include "CGameObject/CCharacter/CCharacter.h"			//キャラクタークラス



class CStageManager
{
public:
	CStageManager();
	~CStageManager();

	void Create();
	void Init();
	void Update();
	void Draw(std::unique_ptr<CCamera>& pCamera);
	void Destroy();

	std::unique_ptr<CStageLoader>& GetStageLoader() { return m_upStageLoader; }
	std::pair<float, float> GetMapSize() { return { m_upStageLoader->GetWidth(), m_upStageLoader->GetHeight() }; }

	bool IsHit(CCharacter& charactor);

private:

	std::unique_ptr<CStage>			 m_upStage;
	std::unique_ptr<CStageCollision> m_upStageCollision;
	std::unique_ptr<CStageLoader>	 m_upStageLoader;
	std::unique_ptr<CStageResource>  m_upStageResource;

};