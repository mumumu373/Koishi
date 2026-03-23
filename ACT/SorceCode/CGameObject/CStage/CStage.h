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
* @brief    マップの大きさを取得する関数(縦・横)
*****************************************************************************************/
    float GetWidth() { return m_Chip.w * m_MapMax.x; }
    float GetHeight() { return m_Chip.h * m_MapMax.y; }


/*****************************************************************************************
* @brief    マップデータの読込
* @param    読み込みたいマップデータのファイル名
*****************************************************************************************/
    bool LoadMap(const std::string& fileName);


private:

/*****************************************************************************************
* @brief    背景のスクロールなどのアニメーション処理を行う関数
*****************************************************************************************/
    void Animation();

private:
    CImage::FRAMESPLIT m_Chip;  // マップチップ1つあたりの幅、高さ

    VECTOR2_f m_MapMax;         // マップデータの縦と横の最大数

    // ステージ情報
    std::vector<std::vector<int>> m_mapData;
};