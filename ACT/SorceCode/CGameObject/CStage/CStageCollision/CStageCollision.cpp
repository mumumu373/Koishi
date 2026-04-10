#include "CStageCollision.h"
#include <iostream>

CStageCollision::CStageCollision()
    : m_isEventChipHit(false)
{
}


CStageCollision::~CStageCollision()
{

}

//--------------------------------------------------------------------------------------------------------------

bool CStageCollision::IsHit(const VECTOR2_f& pos, float w, float h,
    const std::vector<std::vector<int>>& mapData,
    float chipW, float chipH)
{
    float checkX[] = { pos.x, pos.x + w / 3.0f, pos.x + w / 1.5f, pos.x + w - 1.0f };
    float checkY[] = { pos.y, pos.y + h / 3.0f, pos.y + h / 1.5f, pos.y + h - 1.0f };
    m_isEventChipHit = false;

    // 添え字(i, j)を使ってループを回す
    for (int i = 0; i < 4; ++i)
    {
        float x = checkX[i]; // 現在のX座標

        for (int j = 0; j < 4; ++j)
        {
            float y = checkY[j]; // 現在のY座標

            int mapX = (int)(x / chipW);
            int mapY = (int)(y / chipH);

            if (mapY < 0 || mapY >= (int)mapData.size() ||
                mapX < 0 || mapX >= (int)mapData[0].size())
            {
                return true;
            }

            int tipID = mapData[mapY][mapX];

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
            if (tipID != 0 && tipID != 15)
            {
                std::cout << tipID << std::endl;
                return true;
            }
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------------------------------

void CStageCollision::EventChipPattern()
{

}