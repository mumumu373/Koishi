#include "CNormalAttack.h"
#include <CImage/CImageManager.h>



CNormalAttack::CNormalAttack()
	:Collision()
	, IMagePos()
	, Imagerag()
	, Antimension()
	, Attacked(false)
{
	for (int i = 0; i < CollisionMAX; i++) {
		Collision[i].x = 0;
		Collision[i].xw = AtakuSize;
		Collision[i].y = 0;
		Collision[i].yh = AtakuSize;
	}
}

CNormalAttack::~CNormalAttack()
{
}

void CNormalAttack::Strat(double rag,VECTOR2_f PlayerCenterPos)
{
	Antimension = 0;
	Attacked = true;
	IMagePos.x = PlayerCenterPos.x +-cos(rag)* (AtakuSize* CollisionMAX - AtakuSize /2);
	IMagePos.y = (PlayerCenterPos.y +-sin (rag)* (AtakuSize* CollisionMAX)- AtakuSize/2);
	for (int i = 0; i < CollisionMAX; i++) {
		Collision[i].x = PlayerCenterPos.x  +-cos(rag) * AtakuSize*(i+1)- AtakuSize/2;
		Collision[i].y = PlayerCenterPos.y + -sin(rag) * AtakuSize*(i+1) - AtakuSize / 2;
	}
	Imagerag = rag * 180  / M_PI + 180;
}



void CNormalAttack::Update()
{
	if (Attacked) {
		if (Antimension>Time) {
			Attacked = false;
		}
		Antimension++;
	}
}

void CNormalAttack::Draw(std::unique_ptr<CCamera>& pCamera)
{
	if (Attacked == true) {
		
		int i = Time / CollisionMAX;
		int t = Antimension / i;
		VECTOR2_f Pos = { Collision[0].x + Collision[0].xw / 2 - ((AtakuSize * CollisionMAX)/2),Collision[0].y + Collision[0].yh - AtakuSize  };
		VECTOR2_f pos= pCamera->CalcToPositionInCamera(&Pos);
		CImageManager::SelectImg(CImageManager::IMG_Slash)->TransAlBlendRotation3(
			pos.x,
			pos.y,
			AtakuSize * CollisionMAX,
			AtakuSize,
			0,
			32 * t,
			imageW,
			imageH,
			255, 0, 0,Imagerag
		);
	}
	}


void CNormalAttack::DrawColion(HDC hdc,HWND hwnd, std::unique_ptr<CCamera>& pCamera)
{
	if (Attacked == true) {
		PAINTSTRUCT ps;
		HDC hdc1 = BeginPaint(hwnd, &ps);
		for (int i = 0; i < CollisionMAX; i++) {
			VECTOR2_f m_Position;
			m_Position.x = Collision[i].x;
			m_Position.y = Collision[i].y;
			VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);
			CImageManager::SelectImg(CImageManager::enImgList::IMG_BackSky)->TransAlBlendRotation3(
				DispPos.x,
				DispPos.y,
				Collision[i].xw,
				Collision[i].yh,
				0,
				0,
				1,
				1,
				125, 0, 0, 0
			);
		}
		EndPaint(hwnd, &ps);
		// 円を描画
	}
}
