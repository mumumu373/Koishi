#pragma once
#include "Global.h"	
#include <vector>

/***************************************************
*   ステージ判定クラス
*
*   マップとプレイヤーの衝突判定を行うクラス。
***************************************************/

class CStageCollision
{
public:
	CStageCollision();
	~CStageCollision();

	void SetMapData(std::vector<std::vector<int>>& MapData) { m_mapData = MapData; }

private:

	VECTOR2_f m_PlayerPosition;
	std::vector<std::vector<int>> m_mapData;

};