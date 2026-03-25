#pragma once
#include "Global.h"
#include <vector>

class CStageCollision {
public:
    CStageCollision() {}

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
        const std::vector<std::vector<int>>& mapData,
        float chipW, float chipH);
};