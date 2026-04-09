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

void CStageResource::Create()
{
	//マップデータの読み込み

}

void CStageResource::Relese()
{
	m_pStage = nullptr;
	m_pStageLoader = nullptr;
	m_pStageCollision = nullptr;
}
