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
    bool IsHit(const double& PosX, const double& PosY, float w, float h, VECTOR2_f offset);


    /*****************************************************************************************
	* @brief    マップデータをセットする関数。Managerで使用する
	* @param    mapData : 現在のマップデータ
    *****************************************************************************************/
    void SetCurrentMapData(const std::vector<std::vector<int>>& mapData) { m_MapData = mapData; }

    int GetHitMapX() const { return m_HitMapX; }
    int GetHitMapY() const { return m_HitMapY; }

    bool GetHitStageChange()const { return m_isStageChangeHit; }

    bool GetHitEvent()const { return m_isEventChipHit; }

    bool GetHitDamageKedama()const { return m_isDamageKedamaHit; }

    bool GetBossSetPos()const { return m_isBossSetPosHit; }

private:
    CStageCollision();
    CStageCollision(const CStageCollision& rhs) = delete;
    CStageCollision& operator=(CStageCollision& rhs) = delete;

    //ステージチェンジチップに当たったかどうかのフラグ
    bool m_isStageChangeHit;

    // イベントチップに当たったかどうかのフラグ
	bool m_isEventChipHit;

    //ダメージ毛玉に当たったかどうかのフラグ
    bool m_isDamageKedamaHit;

    //ボス(今回はナズーリンのみ)
    bool m_isBossSetPosHit;

	// 現在のマップデータを保持する変数
    std::vector<std::vector<int>> m_MapData;

    int m_HitMapX;	// 当たったマップチップのX座標
    int m_HitMapY;	// 当たったマップチップのY座標
};