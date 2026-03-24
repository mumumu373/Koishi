#include "CStage.h"
#include <fstream> 
#include <sstream> 
#include <crtdbg.h>

#include "CGameObject/CStage/CStageCollisin/CStageCollision.h"	//ステージ当たり判定クラス

CStage::CStage()
    : m_Chip(0, 0, 48, 48)
    , m_MapMax()
{
    m_Position = { 0, 0 };
    m_Framesplit = { 0, 0, 800, 600 };
}

CStage::~CStage()
{
}


void CStage::Update()
{
}

void CStage::Draw(std::unique_ptr<CCamera>& pCamera)
{
    // 背景の切り取り範囲設定（スクロール等の処理があればここで拡張）
    m_Framesplit = { 0, 0, WND_W, WND_H };

	//背景の描画
    VECTOR2_f StagePos = pCamera->CalcToStagePosition();
    BackGroundDraw(StagePos);

    // チップ描画
    for (int y = 0; y < m_MapMax.y; y++) 
    {
        for (int x = 0; x < m_MapMax.x; x++) 
        {
            // 15番目の画像は透過用
            if (m_mapData[y][x] == 15) continue;

            CImageManager::SelectImg(CImageManager::enImgList::IMG_StageMain)->TransBlt(
                (x * m_Chip.w + StagePos.x), // 表示x座標
                (y * m_Chip.h + StagePos.y), // 表示y座標
                m_Chip.w,                         // 切り取り幅
                m_Chip.h,                         // 切り取り高さ
                m_mapData[y][x] * m_Chip.w,       // 元画像x座標
                0);                               // 元画像y座標
        }
    }
}

void CStage::BackGroundDraw(VECTOR2_f NowStagePos)
{

    // ステージ背景1の描画
    CImageManager::SelectImg(CImageManager::enImgList::IMG_BackSky)->TransAlBlend(
        0,              // 表示位置x
        0,              // 表示位置y
        m_Framesplit.w, // 画像幅
        m_Framesplit.h, // 高さ
        m_Framesplit.x, // 元画像x
        m_Framesplit.y, // 元画像y
        255);           // 透明度

    // ステージ背景1の描画
    CImageManager::SelectImg(CImageManager::enImgList::IMG_BackGround)->TransAlBlend(
        0,              // 表示位置x
        0,              // 表示位置y
        m_Framesplit.w, // 画像幅
        m_Framesplit.h, // 高さ
        m_Framesplit.x, // 元画像x
        m_Framesplit.y, // 元画像y
        255);           // 透明度
}
