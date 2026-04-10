#include "CNormalAttack.h"
#include <CImage/CImageManager.h>



CNormalAttack::CNormalAttack()
	:Colion()
	, IMagePos()
	, Imagerag()
	, Antimension()
	, Attacked(false)
{
	for (int i = 0; i < ColionMAX; i++) {
		Colion[i].x = 0;
			Colion[i].xw = AtakuSize;
			Colion[i].y = 0;
			Colion[i].yh = AtakuSize;
	}
}

CNormalAttack::~CNormalAttack()
{
}

void CNormalAttack::Strat(double rag,VECTOR2_f PlayerCenterPos)
{
	Antimension = 0;
	Attacked = true;
	IMagePos.x = PlayerCenterPos.x +-cos(rag)* AtakuSize* ColionMAX- AtakuSize /2;
	IMagePos.y = (PlayerCenterPos.y +-sin (rag)* AtakuSize* ColionMAX)- AtakuSize*2/2;
	for (int i = 0; i < ColionMAX; i++) {
		Colion[i].x = PlayerCenterPos.x  +-cos(rag) * AtakuSize*(i+1)- AtakuSize/2;
		Colion[i].y = PlayerCenterPos.y + -sin(rag) * AtakuSize*(i+1) - AtakuSize / 2;
	}
	Imagerag = rag * 180  / M_PI-90;

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
		
		int i = Time / ColionMAX;
		int t = Antimension / i;
		VECTOR2_f pos= pCamera->CalcToPositionInCamera(&IMagePos);
		CImageManager::SelectImg(CImageManager::IMG_Slash)->TransAlBlendRotation3(
			pos.x,
			pos.y,
			AtakuSize,
			AtakuSize * ColionMAX,
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
		for (int i = 0; i < ColionMAX; i++) {
			VECTOR2_f m_Position;
			m_Position.x = Colion[i].x;
			m_Position.y = Colion[i].y;
			VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);
			CImageManager::SelectImg(CImageManager::enImgList::IMG_BackSky)->TransAlBlendRotation3(
				DispPos.x,
				DispPos.y,
				Colion[i].xw,
				Colion[i].yh,
				0,
				0,
				1,
				1,
				255, 0, 0, 0
			);


		}
		EndPaint(hwnd, &ps);
		// 円を描画
	}



}

ObjectInfo CNormalAttack::GetColion(int No)
{
	return Colion[No];
}
