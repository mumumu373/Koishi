#pragma once
#include <wtypes.h>
#include <wingdi.h>
#include <vector>

class NEGA
{
public:
	NEGA();
	~NEGA();
	//画面全体の色を反転させる関数
	void Draw(HDC Scre);
	//白黒か
	void DrawBH(HDC Scre);

	void DrawSepia(HDC Scre);

	void DrawCH(HDC Scre);
	void DrawCH1(HDC Scre);

	void iiii(int start, int end, std::vector<DWORD> pixels);

	DWORD RotateHou(BYTE r, BYTE g, BYTE b, float angle);
	int CLAMP(int& i);
private:

};
