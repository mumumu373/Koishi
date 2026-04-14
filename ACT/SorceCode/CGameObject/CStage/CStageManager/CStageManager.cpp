#include "CStageManager.h"

CStageManager::CStageManager()
	: m_upStageLoader		(nullptr)
	, m_upStageDraw			(nullptr)
	, m_StageName			()
{
}

CStageManager::~CStageManager()
{
}

//--------------------------------------------------------------------------------------------------------------

bool CStageManager::Create()
{
	//ステージのインスタンス生成
	m_upStageLoader = std::make_unique<CStageLoader>();
	m_upStageDraw = std::make_unique<CStageDraw>();

	SetStageData();
	ChangeStage(enStage::Map01);

	return true;
}

//--------------------------------------------------------------------------------------------------------------

void CStageManager::Init()
{
}

//--------------------------------------------------------------------------------------------------------------

void CStageManager::Update()
{
	if(GetAsyncKeyState('L') & 0x8000)
	{
		ChangeStage(enStage::Map02);
	}
}

//--------------------------------------------------------------------------------------------------------------

void CStageManager::Draw(std::unique_ptr<CCamera>& pCamera)
{
	m_upStageDraw->Draw(pCamera);
}

//--------------------------------------------------------------------------------------------------------------

void CStageManager::Destroy()
{
}

//--------------------------------------------------------------------------------------------------------------

void CStageManager::SetStageData()
{
	m_StageName[enStage::Map01] = "Data\\MapData\\Map01.csv";
	m_StageName[enStage::Map02] = "Data\\MapData\\Map02.csv";
}

//--------------------------------------------------------------------------------------------------------------

bool CStageManager::IsHit(CCharacter& charactor)
{
	//可読性のために作成
	struct Size
	{
		int w;
		int h;
	};

	//当たり判定に必要な情報を取得
	VECTOR2_f	charactorPos	= charactor.GetPosition();
	Size		size		= { charactor.GetFrameSplit().w,charactor.GetFrameSplit().h };
	auto		mapData		= m_upStageLoader->GetMapData();
	constexpr	float chipW = 48;
	constexpr	float chipH = 48;


	//=========当たり判定=========

	return CStageCollision::GetInstance()->IsHit(
			charactorPos.x, charactorPos.y,
			size.w, size.h,
			{0, 0});

	//============================
}

//--------------------------------------------------------------------------------------------------------------

void CStageManager::ChangeStage(enStage stageNum)
{
	std::string m_CurrentStage = m_StageName[stageNum];

	// マップデータ読み込み
	if (m_upStageLoader->LoadMap(m_CurrentStage) == false) return;

	CStageCollision::GetInstance()->SetCurrentMapData(m_upStageLoader->GetMapData());
	m_upStageDraw->SetMapMax(m_upStageLoader->GetMapWidth(), m_upStageLoader->GetMapHeight());
	m_upStageDraw->SetMapData(m_upStageLoader->GetMapData());
}

//--------------------------------------------------------------------------------------------------------------

