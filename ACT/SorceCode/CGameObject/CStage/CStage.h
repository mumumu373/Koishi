#pragma once
#include <vector>  // STL: 動的配列
#include <string>  // STL: 文字列
#include <memory>  // unique_ptr用
#include "CGameObject/CGameObject.h" // ゲームオブジェクトクラス
#include "Global.h"                  // ウィンドウサイズ等

/***************************************************
*   ステージクラス
*   
*   読み込みと描画を一緒に行うクラス。
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

    void LoadData();

    double GetWidth() { return m_Chip.w * m_MapMax.x; }
    double GetHeight() { return m_Chip.h * m_MapMax.y; }

    // マップデータ読込
    bool LoadMap(const std::string& fileName);

private:
    void Animation();

private:
    CImage::FRAMESPLIT m_Chip;  // マップチップ1つあたりの幅、高さ

    VECTOR2_f m_MapMax;         // マップデータの縦と横の最大数

    // STLによる2次元配列。メモリ管理はすべてvectorが自動で行います。
    std::vector<std::vector<int>> m_mapData;
};