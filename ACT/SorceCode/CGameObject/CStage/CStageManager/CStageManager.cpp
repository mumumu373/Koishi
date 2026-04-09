#include "CStageManager.h"

CStageManager::CStageManager()
	: m_upStageLoader		(nullptr)
	, m_upStage				(nullptr)
	, m_upStageCollision	(nullptr)
	, m_upStageResource		(nullptr)

{
}

CStageManager::~CStageManager()
{
}

bool CStageManager::Create()
{
	//ステージのインスタンス生成
	m_upStage = std::make_unique<CStage>();
	m_upStageCollision = std::make_unique<CStageCollision>();
	m_upStageLoader = std::make_unique<CStageLoader>();
	m_upStageResource = std::make_unique<CStageResource>(m_upStage, m_upStageLoader, m_upStageCollision);

	m_upStageResource->Create();

	if (m_upStageLoader->LoadMap("Data\\MapData\\Map01.csv") == false) return false;
	m_upStage->SetMapMax(m_upStageLoader->GetMapWidth(), m_upStageLoader->GetMapHeight());
	m_upStage->SetMapData(m_upStageLoader->GetMapData());

}

void CStageManager::Init()
{
}

void CStageManager::Update()
{
}

void CStageManager::Draw(std::unique_ptr<CCamera>& pCamera)
{
	m_upStage->Draw(pCamera);
}

void CStageManager::Destroy()
{
}

bool CStageManager::IsHit(CCharacter& charactor)
{
	struct Size
	{
		int w;
		int h;
	};

	//当たり判定に必要な情報を取得
	VECTOR2_f	playerPos	= charactor.GetPosition();
	Size		size		= { charactor.GetFrameSplit().w,charactor.GetFrameSplit().h };
	auto		mapData		= m_upStageLoader->GetMapData();
	constexpr	float chipW = 48;
	constexpr	float chipH = 48;

	//当たり判定クラスのIsHit関数に情報を渡して、当たっているかどうかを返す
	return m_upStageCollision->IsHit(
			playerPos,
			size.w, size.h,
			mapData,
			chipW, chipH);
}
