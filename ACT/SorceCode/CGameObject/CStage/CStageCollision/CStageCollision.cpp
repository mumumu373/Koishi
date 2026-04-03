#include "CStageCollision.h"

bool CStageCollision::IsHit(const VECTOR2_f& pos, float w, float h,
    const std::vector<std::vector<int>>& mapData,
    float chipW, float chipH)
{
    // チェックする座標のリストを作成
    float checkX[] = { pos.x, pos.x + w / 2.0f, pos.x + w - 1.0f };
    float checkY[] = { pos.y, pos.y + h / 2.0f, pos.y + h - 1.0f };

    for (float x : checkX) 
    {
        for (float y : checkY) 
        {
            // ピクセル座標をマップの配列番号に変換
            int mapX = (int)(x / chipW);
            int mapY = (int)(y / chipH);

            // 配列の範囲外チェック（画面外は壁とみなす例）
            if (mapY < 0 || mapY >= (int)mapData.size() ||
                mapX < 0 || mapX >= (int)mapData[0].size()) 
            {
                return true;
            }

            // チップ番号の判定
            int tipID = mapData[mapY][mapX];
            if (tipID != 0 && tipID != 15) 
            {
                return true; // どこか1点でも壁に当たればヒット
            }
        }
    }
    return false;
}