#include "CMouseInput//CMouseInput.h"
#include "CImage//CImageManager.h"

CMouseInput::CMouseInput()
	:MouseColorChange(false)
	, MouseLeft{ false,false }
	, MouseRight{ false,false }
	, MouseWheel{ false,false }
{
	Size = StertSize;
}
CMouseInput::~CMouseInput()
{
}
void CMouseInput::update() {
	//ƒ}ƒEƒX‚Ì“ü—Í‚ð‹L˜^
	MouseColorChange = false;
	MouseLeft[1] =  MouseLeft[0];
	MouseRight[1] = MouseRight[0];
	MouseWheel[1] = MouseWheel[0];
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		MouseLeft[0] = true;
	}
	else {
		MouseLeft[0] = false;
	}
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
		MouseRight[0] = true;
	}
	else {
		MouseRight[0] = false;
	}
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
		MouseWheel[0] = true;
	}
	else {
		MouseWheel[0] = false;
	}

	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(m_hWnd, &pos);
	MousePos = { (float)pos.x, (float)pos.y };

}
bool CMouseInput::getMouseLeft(bool Push, bool Moment)
{
	//Push‚ªtrue‚È‚ç‰Ÿ‚µ‚Ä‚¢‚é
	if (Push) {
		//Moment‚ªtrue‚È‚ç‰Ÿ‚³‚ê‚½uŠÔAfalse‚È‚ç‰Ÿ‚µ‘±‚¯‚Ä‚¢‚éŠÔ
		if (Moment == true) {
			if (MouseLeft[0] == true &&MouseLeft[1] == false) {
				return true;
			}
		}
		else {
			if (MouseLeft[0] == true) {
				return true;
			}
		}
		return false;
	}
	else
	{
		//Moment‚ªtrue‚È‚ç—£‚³‚ê‚½uŠÔAfalse‚È‚ç—£‚µ‘±‚¯‚Ä‚¢‚éŠÔ
		if (Moment == true) {
			if (MouseLeft[0] == false && MouseLeft[1] == true) {
				return true;
			}
		}
		else {
			if (MouseLeft[0] == false) {
				return true;
			}
		}
		return false;
	}
	



}
bool CMouseInput::getMouseRight(bool Push, bool Moment)
{
	//Push‚ªtrue‚È‚ç‰Ÿ‚µ‚Ä‚¢‚é
	if (Push) {
		//Moment‚ªtrue‚È‚ç‰Ÿ‚³‚ê‚½uŠÔAfalse‚È‚ç‰Ÿ‚µ‘±‚¯‚Ä‚¢‚éŠÔ
		if (Moment == true) {
			if (MouseRight[0] == true && MouseRight[1] == false) {
				return true;
			}
		}
		else {
			if (MouseRight[0] == true) {
				return true;
			}
		}
		return false;
	}
	else
	{
		//Moment‚ªtrue‚È‚ç—£‚³‚ê‚½uŠÔAfalse‚È‚ç—£‚µ‘±‚¯‚Ä‚¢‚éŠÔ
		if (Moment == true) {
			if (MouseRight[0] == false && MouseRight[1] == true) {
				return true;
			}
		}
		else {
			if (MouseRight[0] == false) {
				return true;
			}
		}
		return false;
	}
	

}
bool CMouseInput::getMouseWheel(bool Push, bool Moment)
{
	//Push‚ªtrue‚È‚ç‰Ÿ‚µ‚Ä‚¢‚é
	if (Push) {
		//Moment‚ªtrue‚È‚ç‰Ÿ‚³‚ê‚½uŠÔAfalse‚È‚ç‰Ÿ‚µ‘±‚¯‚Ä‚¢‚éŠÔ
		if (Moment == true) {
			if (MouseWheel[0] == true && MouseWheel[1] == false) {
				return true;
			}
		}
		else {
			if (MouseWheel[0] == true) {
				return true;
			}
		}
		return false;
	}
	else
	{
		//Moment‚ªtrue‚È‚ç—£‚³‚ê‚½uŠÔAfalse‚È‚ç—£‚µ‘±‚¯‚Ä‚¢‚éŠÔ
		if (Moment == true) {
			if (MouseWheel[0] == false && MouseWheel[1] == true) {
				return true;
			}
		}
		else {
			if (MouseWheel[0] == false) {
				return true;
			}
		}
		return false;
	}
	
}
void CMouseInput::InitialSettings(HWND hWnd)
{


	GetInstance()->m_hWnd = hWnd;
	for (int i = 0; i < 2; i++)
	{
		GetInstance()->MouseLeft[i] = false;
		GetInstance()->MouseRight[i] = false;
		GetInstance()->MouseWheel[i] = false;
	}
}
VECTOR2_f CMouseInput::GetMousePos()
{
		return 	GetInstance()->MousePos;
}
;
void CMouseInput::draw() {
	if (MouseColorChange) {
		CImageManager::SelectImg(CImageManager::enImgList::IMG_Mouse)->TransBltPlas(
			MousePos.x - Size / 2,
			MousePos.y - Size / 2,
			Size, Size,
			0, IMGSize,
			IMGSize, IMGSize
		);
	}
	else {
		CImageManager::SelectImg(CImageManager::enImgList::IMG_Mouse)->TransBltPlas(
			MousePos.x - Size / 2,
			MousePos.y - Size / 2,
			Size, Size,
			0, 0,
			IMGSize, IMGSize
		);
	}


	
}