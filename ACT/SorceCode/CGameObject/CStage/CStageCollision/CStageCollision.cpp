#include "CStageCollision.h"
#include <iostream>
//--------------------------------------------------------------------------------------------------------------

bool CStageCollision::IsHit(const VECTOR2_f& pos, float w, float h,
    const std::vector<std::vector<int>>& mapData,
    float chipW, float chipH)
{
    //チェックする座標のリストを作成
    float checkX[] = { pos.x, pos.x + w / 3.0f,pos.x + w / 1.5f, pos.x + w - 1.0f };
    float checkY[] = { pos.y, pos.y + h / 3.0f,pos.y + h / 1.5f, pos.y + h - 1.0f };
    bool test = false;

    for (float x : checkX) 
    {
        for (float y : checkY) 
        {
            //ピクセル座標をマップの配列番号に変換
            int mapX = (int)(x / chipW);
            int mapY = (int)(y / chipH);

            //配列の範囲外チェック（画面外は壁とみなす例）
            if (mapY < 0 || mapY >= (int)mapData.size() ||
                mapX < 0 || mapX >= (int)mapData[0].size()) 
            {
                return true;
            }

            //チップ番号の判定
            int tipID = mapData[mapY][mapX];
            if (tipID == 19)
            {
                std::cout << "EventHit" << std::endl;
            }

            if (tipID != 0 && tipID != 15) 
            {
                std::cout << mapData[mapY][mapX] << "Hit" << std::endl;
                return true; //どこか1点でも壁に当たればヒット
            }
        }
    }


    return false;
}

void CStageCollision::Resolve(VECTOR2_f& nextPos, const VECTOR2_f& currentPos, float w, float h, const std::vector<std::vector<int>>& mapData, float chipW, float chipH)
{
    // --- 1. X軸方向の判定と押し返し ---
    // Y座標は「移動前」のまま、Xだけ「移動後」にして判定
    if (IsHit({ nextPos.x, currentPos.y }, w, h, mapData, chipW, chipH))
    {
        if (nextPos.x > currentPos.x) {
            // 右移動中に衝突：チップの左側に合わせる
            // (右端の座標をチップ単位で切り捨てて、そこから幅w分戻す)
            nextPos.x = floor((nextPos.x + w - 1.0f) / chipW) * chipW - w;
        }
        else if (nextPos.x < currentPos.x) {
            // 左移動中に衝突：チップの右側に合わせる
            // (左端の座標があるチップの次のチップの左端＝現在のチップの右端)
            nextPos.x = floor(nextPos.x / chipW + 1.0f) * chipW;
        }
    }

    // --- 2. Y軸方向の判定と押し返し ---
    // X座標は「補正済み(1の結果)」を使い、Yを判定
    if (IsHit({ nextPos.x, nextPos.y }, w, h, mapData, chipW, chipH))
    {
        if (nextPos.y > currentPos.y) {
            // 下移動中に衝突：チップの上側に合わせる
            nextPos.y = floor((nextPos.y + h - 1.0f) / chipH) * chipH - h;
        }
        else if (nextPos.y < currentPos.y) {
            // 上移動中に衝突：チップの下側に合わせる
            nextPos.y = floor(nextPos.y / chipH + 1.0f) * chipH;
        }
    }
}
//--------------------------------------------------------------------------------------------------------------
