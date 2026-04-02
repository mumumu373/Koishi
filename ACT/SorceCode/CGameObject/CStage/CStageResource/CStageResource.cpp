#include "CStageResource.h"

CStageResource::CStageResource(
	std::unique_ptr<CStage>& upStage,
	std::unique_ptr<CStageLoader>& upStageLoader,
	std::unique_ptr<CStageCollision>& upStageCollision)
	: m_pStage				(upStage.get())
	, m_pStageLoader		(upStageLoader.get())
	, m_pStageCollision		(upStageCollision.get())
{
}

CStageResource::~CStageResource()
{
}

bool CStageResource::Create()
{
	//マップデータの読み込み
	if(m_pStageLoader->LoadMap("Data\\MapData\\Map01.csv") == false) return false;
	m_pStage->SetMapMax(m_pStageLoader->GetMapWidth(), m_pStageLoader->GetMapHeight());
	m_pStage->SetMapData(m_pStageLoader->GetMapData());

}

void CStageResource::Relese()
{
	m_pStage = nullptr;
	m_pStageLoader = nullptr;
	m_pStageCollision = nullptr;
}
