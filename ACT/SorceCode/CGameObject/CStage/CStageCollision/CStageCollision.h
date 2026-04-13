#pragma once
#include "Global.h"
#include <vector>

class CStageCollision {
public:

    static CStageCollision* GetInstance()
    {
        static CStageCollision s_Instance;
        return &s_Instance;
    }

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
    * @return   trueならヒット!
    *****************************************************************************************/
    bool IsHit(const VECTOR2_f& pos, float w, float h,
        float chipW, float chipH, VECTOR2_f offset);


    /*****************************************************************************************
	* @brief    マップデータをセットする関数。Managerで使用する
	* @param    mapData : 現在のマップデータ
    *****************************************************************************************/
    void SetCurrentMapData(const std::vector<std::vector<int>>& mapData) { m_MapData = mapData; }

    int GetHitMapX() const { return m_HitMapX; }
    int GetHitMapY() const { return m_HitMapY; }

private:
    CStageCollision();
    CStageCollision(const CStageCollision& rhs) = delete;
    CStageCollision& operator=(CStageCollision& rhs) = delete;

    // イベントチップに当たったかどうかのフラグ
	bool m_isEventChipHit;

	// 現在のマップデータを保持する変数
    std::vector<std::vector<int>> m_MapData;

    int m_HitMapX;	// 当たったマップチップのX座標
    int m_HitMapY;	// 当たったマップチップのY座標
};