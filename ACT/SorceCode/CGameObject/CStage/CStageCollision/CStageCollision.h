#pragma once
#include "Global.h"
#include <vector>

class CStageCollision {
public:
    CStageCollision();
    ~CStageCollision();
    /*****************************************************************************************
    * @brief    マップデータの読込
    *
    * @param    pos     : チェックしたい位置 
    *           w       : プレイヤーの幅, 
    *           h       : プレイヤーの高さ
    *			mapData : マップデータ
    *			chipW   : チップの幅
    *			chipH   : チップの高さ
    *****************************************************************************************/
    bool IsHit(const VECTOR2_f& pos, float w, float h,
        float chipW, float chipH);


    /*****************************************************************************************
	* @brief    マップデータをセットする関数。Managerで使用する
	* @param    mapData : 現在のマップデータ
    *****************************************************************************************/
    void SetCurrentMapData(const std::vector<std::vector<int>>& mapData) { m_MapData = mapData; }


private:
    // イベントチップに当たったかどうかのフラグ
	bool m_isEventChipHit;

	// 現在のマップデータを保持する変数
    std::vector<std::vector<int>> m_MapData;
};