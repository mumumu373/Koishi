#pragma once
#include <vector>  
#include <string>  
#include <memory>  
#include "CGameObject/CGameObject.h"
#include "Global.h"                 


/***************************************************
*   ステージクラス
*   
*   マップの読み込みと描画を一緒に行うクラス。
* 
*   ステージの描画とマップデータの管理は分けた方がいい。
***************************************************/
class CStage : public CGameObject
{
public:
    CStage();
    ~CStage();

    void Update() override;
    void Draw(std::unique_ptr<CCamera>& pCamera) override;


/*****************************************************************************************
* @brief    マップの大きさを設定する関数(縦・横)
*****************************************************************************************/
    void SetMapMax(float x, float y) { 
        m_MapMax.x = x;
        m_MapMax.y = y; 
    }

    void SetMapData(const std::vector<std::vector<int>>& MapData) { m_mapData = MapData; }


private:

/*****************************************************************************************
* @brief    背景のスクロールなどのアニメーション処理を行う関数
* 
* @param    CurrentStagePos : 現在のステージ位置座標(スクロールで使用予定)
*****************************************************************************************/
	void BackGroundDraw(VECTOR2_f CurrentStagePos);

private:
    CImage::FRAMESPLIT m_Chip;  // マップチップ1つあたりの幅、高さ

    VECTOR2_f m_MapMax;         // マップデータの縦と横の最大数

    // ステージ情報
    std::vector<std::vector<int>> m_mapData;
};