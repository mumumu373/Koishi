#include "CStageDraw.h"
#include "CImage/CImage.h"
CStageDraw::CStageDraw()
    : m_Chip(0, 0, 48, 48)
    , m_MapMax()
    , m_mapData()
	, m_Framesplit(0, 0, WND_W, WND_H)
	, IsBossMap(false)
{
}

CStageDraw::~CStageDraw()
{
}

//--------------------------------------------------------------------------------------------------------------

void CStageDraw::Draw(std::unique_ptr<CCamera>& pCamera)
{
    // 背景の切り取り範囲設定（スクロール等の処理があればここで拡張）
    m_Framesplit = { 0, 0, WND_W, WND_H };

    // 背景の描画
    VECTOR2_f StagePos = pCamera->CalcToStagePosition();
    BackGroundDraw(StagePos);

    // StagePosをチップサイズで割り、左上に描画されるべきブロックを求める
    int startX = static_cast<int>(-StagePos.x / m_Chip.w);
    int startY = static_cast<int>(-StagePos.y / m_Chip.h);

	int endX = startX + (WND_W / m_Chip.w) + 2; //+2は、画面外に少しはみ出る分も描画するための余裕
    int endY = startY + (WND_H / m_Chip.h) + 2;

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX > m_MapMax.x) endX = m_MapMax.x;
    if (endY > m_MapMax.y) endY = m_MapMax.y;

    // チップ描画
    for (int y = startY; y < endY; y++)
    {
        for (int x = startX; x < endX; x++)
        {
            // 15番目の画像は透過用
            if (m_mapData[y][x] == 15) continue;

            CImageManager::SelectImg(CImageManager::enImgList::IMG_StageMain)->TransBlt(
                (x * m_Chip.w + StagePos.x),      // 表示x座標
                (y * m_Chip.h + StagePos.y),      // 表示y座標
                m_Chip.w,                         // 切り取り幅
                m_Chip.h,                         // 切り取り高さ
                m_mapData[y][x] * m_Chip.w,       // 元画像x座標
                0);                               // 元画像y座標
        }
    }
}
//--------------------------------------------------------------------------------------------------------------

void CStageDraw::BackGroundDraw(VECTOR2_f& CurrentStagePos)
{
    if(IsBossMap)
    {
		CurrentStagePos = { 0,0 }; 
        IsBossMap = false;
    }

    //クランプ
    float SkyloopX = fmod(CurrentStagePos.x * 0.5f, m_Framesplit.w);
    float MountainloopX = fmod(CurrentStagePos.x, m_Framesplit.w);

    //背景描画
    DrawSkyLoop(SkyloopX, CurrentStagePos.y);
    DrawMountainLoop(MountainloopX, CurrentStagePos.y);
}

//--------------------------------------------------------------------------------------------------------------

void CStageDraw::DrawMountainLoop(const float loopX, const float y) const
{
    CImageManager::SelectImg(CImageManager::enImgList::IMG_BackGround)->TransAlBlend(
        loopX,          
        0,              
        m_Framesplit.w, 
        m_Framesplit.h, 
        m_Framesplit.x, 
        m_Framesplit.y, 
        255);           

    CImageManager::SelectImg(CImageManager::enImgList::IMG_BackGround)->TransAlBlend(
        loopX + m_Framesplit.w,
        0,
        m_Framesplit.w,
        m_Framesplit.h,
        m_Framesplit.x,
        m_Framesplit.y,
        255);          

}

//--------------------------------------------------------------------------------------------------------------

void CStageDraw::DrawSkyLoop(const float loopX, const float y)
{
    CImageManager::SelectImg(CImageManager::enImgList::IMG_BackSky)->TransAlBlend(
        loopX,         
        0,
        m_Framesplit.w,
        m_Framesplit.h,
        m_Framesplit.x,
        m_Framesplit.y,
        255);          

    CImageManager::SelectImg(CImageManager::enImgList::IMG_BackSky)->TransAlBlend(
        loopX + m_Framesplit.w,
        0,
        m_Framesplit.w, 
        m_Framesplit.h, 
        m_Framesplit.x, 
        m_Framesplit.y, 
        255);           

}

//--------------------------------------------------------------------------------------------------------------
