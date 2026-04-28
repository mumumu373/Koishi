#include "CStageCollision.h"
#include <iostream>

CStageCollision::CStageCollision()
    : m_isEventChipHit  (false)
    , m_isDamageKedamaHit(false)
    , m_isBossSetPosHit (false)
	, m_MapData()
    , m_HitMapX()
    , m_HitMapY()
{
}


CStageCollision::~CStageCollision()
{

}

//--------------------------------------------------------------------------------------------------------------

bool CStageCollision::IsHit(const double& PosX, const double& PosY, float w, float h, VECTOR2_f offset)
{
    float startX = PosX + offset.x;
    float startY = PosY + offset.y;

    float checkX[] = 
    {
        startX,                  
        startX + w / 3.0f,       
        startX + w * 2.0f / 3.0f,
        startX + w - 1.0f        
    };

    float checkY[] =
    {
        startY,
        startY + h / 3.0f,
        startY + h * 2.0f / 3.0f,
        startY + h - 1.0f
    };

    m_isEventChipHit = false;
    m_isDamageKedamaHit = false;
    m_isBossSetPosHit = false;

    for (int i = 0; i < 4; ++i)
    {
        float x = checkX[i]; // 現在のX座標

        for (int j = 0; j < 4; ++j)
        {
            float y = checkY[j]; // 現在のY座標

            int mapX = (int)(x / 48);
            int mapY = (int)(y / 48);

            if (mapY < 0 || mapY >= (int)m_MapData.size() ||
                mapX < 0 || mapX >= (int)m_MapData[0].size())
            {
                return true;
            }

            int tipID = m_MapData[mapY][mapX];

            switch (tipID)
            {
            case 19:
                if (i == 0)
                {
                    m_isEventChipHit = true;
                }
                break;

            case 20:
                // i == 0 ならば、checkX[0] (一番左側) の判定点である
                if (i == 0)
                {
                    m_isEventChipHit = true;
                }
                break;
            case 21:
                //ダメージ毛玉に当たった
                if (i == 0)
                {
                    m_isDamageKedamaHit = true;
                }
                break;
            case 22:
                //ボス出現ブロックに当たった
                if (i == 0) {
                    m_isBossSetPosHit = true;
                }
                break;
            default:
                break;
            }

            // 壁判定（0と15以外は衝突）
            if (tipID != 0 && tipID != 15 && tipID != 19 && tipID != 20 && tipID != 22)
            {
                m_HitMapX = mapX; 
                m_HitMapY = mapY;
                return true;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------------------------------
