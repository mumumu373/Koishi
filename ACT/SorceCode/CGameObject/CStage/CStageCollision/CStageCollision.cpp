#include "CStageCollision.h"
#include <iostream>

CStageCollision::CStageCollision()
    : m_isEventChipHit  (false)
	, m_MapData()
{
}


CStageCollision::~CStageCollision()
{

}

//--------------------------------------------------------------------------------------------------------------

bool CStageCollision::IsHit(const VECTOR2_f& pos, float w, float h,
    float chipW, float chipH, VECTOR2_f offset)
{
    float startX = pos.x + offset.x;
    float startY = pos.y + offset.y;

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

    for (int i = 0; i < 4; ++i)
    {
        float x = checkX[i]; // 現在のX座標

        for (int j = 0; j < 4; ++j)
        {
            float y = checkY[j]; // 現在のY座標

            int mapX = (int)(x / chipW);
            int mapY = (int)(y / chipH);

            if (mapY < 0 || mapY >= (int)m_MapData.size() ||
                mapX < 0 || mapX >= (int)m_MapData[0].size())
            {
                return true;
            }

            int tipID = m_MapData[mapY][mapX];

            switch (tipID)
            {
            case 19:
                std::cout << "EventHit" << std::endl;
                m_isEventChipHit = true;
                break;

            case 20:
                // i == 0 ならば、checkX[0] (一番左側) の判定点である
                if (i == 0)
                {
                    std::cout << "MapChange" << std::endl;
                    m_isEventChipHit = true;
                }
                break;

            default:
                break;
            }

            // 壁判定（0と15以外は衝突）
            if (tipID != 0 && tipID != 15 && tipID != 19 && tipID != 20)
            {
                std::cout << tipID << std::endl;
                m_HitMapX = mapX; 
                m_HitMapY = mapY;
                return true;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------------------------------
