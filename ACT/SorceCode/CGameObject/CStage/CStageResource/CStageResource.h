#pragma once
#include "CGameObject/CStage/CStageLoader/CStageLoader.h"
#include "CGameObject/CStage/CStage.h"	
#include "CGameObject/CStage/CStageCollision/CStageCollision.h"	

/***************************************************
*   ステージリソースクラス
*
*   マップに必要なデータの受け渡しを行うクラス
***************************************************/

class CStageResource
{
public:
	CStageResource(
		std::unique_ptr<CStage>& upStage,
		std::unique_ptr<CStageLoader>& upStageLoader,
		std::unique_ptr<CStageCollision>& upStageCollision);

	~CStageResource();

	void Create();

	//リソースを解放する関数(CGameでリソース交換するときは絶対に呼ぶ)
	void Relese();

private:

	CStage* m_pStage;						//ステージクラス
	CStageLoader* m_pStageLoader;			//ステージ読み込み定クラス
	CStageCollision* m_pStageCollision;	//ステージ当たり判定クラス
};