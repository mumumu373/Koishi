#include "CNormalAttack.h"
#include <CImage/CImageManager.h>



CNormalAttack::CNormalAttack()
	:Colision()
	, IMagePos()
	, Imagerag()
	, Antimension()
	, Attacked(false)
{
	for (int i = 0; i < ColisionMAX; i++) {
		Colision[i].x = 0;
		Colision[i].xw = AtakuSize;
		Colision[i].y = 0;
		Colision[i].yh = AtakuSize;
	}
}

CNormalAttack::~CNormalAttack()
{
}

void CNormalAttack::Strat(double rag,VECTOR2_f PlayerCenterPos)
{
	Antimension = 0;
	Attacked = true;
	IMagePos.x = (PlayerCenterPos.x +-cos(rag)* AtakuSize* ColisionMAX)- AtakuSize /2;
	IMagePos.y = (PlayerCenterPos.y +-sin (rag)* AtakuSize* ColisionMAX)- AtakuSize*2/2;
	for (int i = 0; i < ColisionMAX; i++) {
		Colision[i].x = PlayerCenterPos.x  +-cos(rag) * AtakuSize*(i+1)- AtakuSize/2;
		Colision[i].y = PlayerCenterPos.y + -sin(rag) * AtakuSize*(i+1) - AtakuSize / 2;
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
		
		int i = Time / ColisionMAX;
		int t = Antimension / i;
		VECTOR2_f pos= pCamera->CalcToPositionInCamera(&IMagePos);
		CImageManager::SelectImg(CImageManager::IMG_Slash)->TransAlBlendRotation3(
			pos.x,
			pos.y,
			AtakuSize * ColisionMAX,
			AtakuSize,
			0,
			32 * t,
			imageW,
			imageH,
			255, 0, 0, Imagerag
		);
	}
	}


void CNormalAttack::DrawColion(HDC hdc,HWND hwnd, std::unique_ptr<CCamera>& pCamera)
{
	if (Attacked == true) {
		PAINTSTRUCT ps;
		HDC hdc1 = BeginPaint(hwnd, &ps);
		for (int i = 0; i < ColisionMAX; i++) {
			VECTOR2_f m_Position;
			m_Position.x = Colision[i].x;
			m_Position.y = Colision[i].y;
			VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);
			CImageManager::SelectImg(CImageManager::enImgList::IMG_BackSky)->TransAlBlendRotation3(
				DispPos.x,
				DispPos.y,
				Colision[i].xw,
				Colision[i].yh,
				0,
				0,
				1,
				1,
				100, 0, 0, 0
			);
		}
		EndPaint(hwnd, &ps);
		// 円を描画
	}
}
