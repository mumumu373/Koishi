#pragma once
#include "Global.h"
class CStageCollisionDraw
{
public:
    static CStageCollisionDraw* GetInstance()
    {
        static CStageCollisionDraw s_Instance;
        return &s_Instance;
    }

	~CStageCollisionDraw();

    void CollisionDraw(RECT rect);
    void SetScreenDC(HDC hDC) { hScreenDC = hDC; }

private:
    CStageCollisionDraw();
    CStageCollisionDraw(const CStageCollisionDraw& rhs) = delete;
    CStageCollisionDraw& operator=(CStageCollisionDraw& rhs) = delete;

    HDC hScreenDC;

};
