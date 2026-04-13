#include "CStageCollisionDraw.h"

CStageCollisionDraw::CStageCollisionDraw()
	: hScreenDC(nullptr)
{

}
CStageCollisionDraw::~CStageCollisionDraw()
{
}

void CStageCollisionDraw::CollisionDraw(RECT rect)
{
	// 枠線の色（ブラシ）を作成（例：赤色）
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));

	// 枠を描画
	FrameRect(hScreenDC, &rect, hBrush);

	// 使い終わったらブラシを破棄
	DeleteObject(hBrush);
}
