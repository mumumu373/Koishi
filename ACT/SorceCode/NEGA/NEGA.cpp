#define _USE_MATH_DEFINES
#include "NEGA/NEGA.h"
#include <Global.h>
#include <vector>
#include <thread>
#include <threads.h>
#include <atomic>
NEGA::NEGA()
{
	// 準備段階（初期化時やサイズ変更時に1回だけ行うのが理想）
	pPixels = nullptr; // ピクセルデータへの直接
	 bmi = { 0 };

	//ビットマップの情報を設定
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//構造体のサイズ
	bmi.bmiHeader.biWidth = WND_W;//ビットマップの幅
	bmi.bmiHeader.biHeight = -WND_H; // 上下逆転を防ぐためにマイナスを指定
	bmi.bmiHeader.biPlanes = 1;//プレーン数は常に1
	bmi.bmiHeader.biBitCount = 32;   // 32bitを指定
	bmi.bmiHeader.biCompression = BI_RGB;//圧縮形式はBI_RGB(非圧縮)を指定


}

NEGA::~NEGA()
{
}

void NEGA::Draw(HDC Scre)
{
	//
	BitBlt(Scre, 0, 0, WND_W, WND_H, Scre, 0, 0, DSTINVERT);

}
/*
{
	int Size = WND_W * WND_H;
	std::vector<DWORD> pixels(Size);
	HBITMAP bmp = (HBITMAP)GetCurrentObject(Scre, OBJ_BITMAP);//現在のビットマップを取得

	GetDIBits(Scre, bmp, 0, WND_H, &pixels[0], &bmi, DIB_RGB_COLORS);

	for (int i = 0; i < Size; i++) {
		pixels[i] = pixels[i] xor 0x0FFFFFF;
	}
	SetDIBits(Scre, bmp, 0, WND_H, &pixels[0], &bmi, DIB_RGB_COLORS);

}
*/
void NEGA::DrawBH(HDC Scre)
{
	int Size = WND_W * WND_H;
	std::vector<DWORD> pixels(Size);
	HBITMAP bmp = (HBITMAP)GetCurrentObject(Scre, OBJ_BITMAP);//現在のビットマップを取得
	BITMAPINFO bmi = { 0 };

	//ビットマップの情報を設定
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//構造体のサイズ
	bmi.bmiHeader.biWidth = WND_W;//ビットマップの幅
	bmi.bmiHeader.biHeight = -WND_H; // 上下逆転を防ぐためにマイナスを指定
	bmi.bmiHeader.biPlanes = 1;//プレーン数は常に1
	bmi.bmiHeader.biBitCount = 32;   // 32bitを指定
	bmi.bmiHeader.biCompression = BI_RGB;//圧縮形式はBI_RGB(非圧縮)を指定


	GetDIBits(Scre, bmp, 0, WND_H, &pixels[0], &bmi, DIB_RGB_COLORS);

	for (int i = 0; i < Size; i++) {
		BYTE r = GetRValue(pixels[i]);//赤の値を取得
		BYTE g = GetGValue(pixels[i]);//緑の値を取得
		BYTE b = GetBValue(pixels[i]);//青の値を取得
		//BYTE cheng = (BYTE)((r + g + b) / 3);//平均値を計算(雑)
		BYTE cheng = (BYTE)(0.299*r + 0.587 * g + 0.114 * b);//平均値を計算(正確)

		pixels[i] = RGB(cheng, cheng, cheng);//平均値をRGBに変換して保存
	}
	SetDIBits(Scre, bmp, 0, WND_H, &pixels[0], &bmi, DIB_RGB_COLORS);

}
void NEGA::DrawSepia(HDC Scre)
{
	int Size = WND_W * WND_H;
	std::vector<DWORD> pixels(Size);

	BITMAPINFO bmi = { 0 };

	//ビットマップの情報を設定
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//構造体のサイズ
	bmi.bmiHeader.biWidth = WND_W;//ビットマップの幅
	bmi.bmiHeader.biHeight = -WND_H; // 上下逆転を防ぐためにマイナスを指定
	bmi.bmiHeader.biPlanes = 1;//プレーン数は常に1
	bmi.bmiHeader.biBitCount = 32;   // 32bitを指定
	bmi.bmiHeader.biCompression = BI_RGB;//圧縮形式はBI_RGB(非圧縮)を指定

	HBITMAP bmp = (HBITMAP)GetCurrentObject(Scre, OBJ_BITMAP);//現在のビットマップを取得


	GetDIBits(Scre, bmp, 0, WND_H, &pixels[0], &bmi, DIB_RGB_COLORS);

	for (int i = 0; i < Size; i++) {
		BYTE r = GetRValue(pixels[i]);//赤の値を取得
		BYTE g = GetGValue(pixels[i]);//緑の値を取得
		BYTE b = GetBValue(pixels[i]);//青の値を取得
		int tr = (int)(0.393 * r + 0.769 * g + 0.189 * b);
		int tg = (int)(0.349 * r + 0.686 * g + 0.168 * b);
		int tb = (int)(0.272 * r + 0.534 * g + 0.131 * b);

		pixels[i] = RGB(CLAMP(tr), CLAMP(tg), CLAMP(tb));//平均値をRGBに変換して保存
	}
	SetDIBits(Scre, bmp, 0, WND_H, &pixels[0], &bmi, DIB_RGB_COLORS);

}
void NEGA::DrawCH(HDC Scre)
{
	int Size = WND_W * WND_H;
	std::vector<DWORD> pixels(Size);
	HBITMAP bmp = (HBITMAP)GetCurrentObject(Scre, OBJ_BITMAP);//現在のビットマップを取得
	BITMAPINFO bmi = { 0 };

	//ビットマップの情報を設定
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//構造体のサイズ
	bmi.bmiHeader.biWidth = WND_W;//ビットマップの幅
	bmi.bmiHeader.biHeight = -WND_H; // 上下逆転を防ぐためにマイナスを指定
	bmi.bmiHeader.biPlanes = 1;//プレーン数は常に1
	bmi.bmiHeader.biBitCount = 32;   // 32bitを指定
	bmi.bmiHeader.biCompression = BI_RGB;//圧縮形式はBI_RGB(非圧縮)を指定


	GetDIBits(Scre, bmp, 0, WND_H, &pixels[0], &bmi, DIB_RGB_COLORS);

	for (int i = 0; i < Size; i++) {
		BYTE r = GetRValue(pixels[i]);//赤の値を取得
		BYTE g = GetGValue(pixels[i]);//緑の値を取得
		BYTE b = GetBValue(pixels[i]);//青の値を取得


		pixels[i] = RGB(b, r, g);//平均値をRGBに変換して保存
	}
	SetDIBits(Scre, bmp, 0, WND_H, &pixels[0], &bmi, DIB_RGB_COLORS);

}
void NEGA::DrawCH1(HDC Scre)
{

	// 準備段階（初期化時やサイズ変更時に1回だけ行うのが理想）
	void* pPixels = nullptr; // ピクセルデータへの直接ポインタ

	int Size = WND_W * WND_H;
	std::vector<DWORD> pixels(Size);
	//HBITMAP bmp = (HBITMAP)GetCurrentObject(Scre, OBJ_BITMAP);//現在のビットマップを取得
	

	BITMAPINFO bmi = { 0 };

	//ビットマップの情報を設定
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//構造体のサイズ
	bmi.bmiHeader.biWidth = WND_W;//ビットマップの幅
	bmi.bmiHeader.biHeight = -WND_H; // 上下逆転を防ぐためにマイナスを指定
	bmi.bmiHeader.biPlanes = 1;//プレーン数は常に1
	bmi.bmiHeader.biBitCount = 32;   // 32bitを指定
	bmi.bmiHeader.biCompression = BI_RGB;//圧縮形式はBI_RGB(非圧縮)を指定

	HBITMAP bmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pPixels, NULL, 0);
	DWORD* pData = static_cast<DWORD*>(pPixels);

	//GetDIBits(Scre, bmp, 0, WND_H, &pixels[0], &bmi, DIB_RGB_COLORS);
	
	int numThreads = std::thread::hardware_concurrency(); // CPUのコア数を取得
	
	int totalPixels = Size;
	int grainSize = totalPixels / numThreads;

	std::vector<std::thread> workers;
	int chunkSize = totalPixels / numThreads;

	for (int i = 0; i < numThreads; i++) {
		int start = i * chunkSize;
		int end = (i + 1) * chunkSize;

		// もし最後の一人だったら、計算上のendではなく
		// 画像の本当のサイズをendにする
		if (i == numThreads - 1) {
			end = totalPixels;
		}
		workers.emplace_back(&NEGA::iiii, this, start, end, pixels
		);
	}
	// 全員の仕事が終わるまで待つ
	for (auto& t : workers) {
		t.join();
	}

	//SetDIBits(Scre, bmp, 0, WND_H, &pixels[0], &bmi, DIB_RGB_COLORS);

}
void NEGA::iiii(int start, int end, std::vector<DWORD> c)
{
	// ここでstartからendまでのピクセルを処理する
	std::vector<DWORD> v = c;
	for (int i = start; i < end; ++i) {
		BYTE r = GetRValue(v[i]);//赤の値を取得
		BYTE g = GetGValue(v[i]);//緑の値を取得
		BYTE b = GetBValue(v[i]);//青の値を取得

		v[i] = RotateHou(r, g, b, 180 * M_PI / 180);
	}
	for (int i = start; i < end; ++i) {
		c[i]	 = v[i];
	}

}
DWORD NEGA::RotateHou(BYTE r, BYTE g, BYTE b, float angle)
{
	float fr = r / 255.0f;
	float fg = g / 255.0f;
	float fb = b / 255.0f;
	float max = max(max(fr, fg), fb);	
	float min = min(min(fr, fg), fb);
	float h, s, v = max;
	float d = max - min;

	if (max == 0) {
		s = 0;
	}
	else {
		s = d / max;
	};
	if (max == min) {
		h = 0;
	}
	else if (max == fr) {
		h = (fg - fb) / d;
		if (fg < fb) {
			h += 6;
		};
	}
	else if (max == fg) {
		h = (fb - fr) / d + 2;
	}
	else if (max == fb) {
		h = (fr - fg) / d + 4;
		h = h / 6;
	};
	// / 360.0f;
	h = fmod(h + angle , 1.0f);	
	float resR, resG, resB;
	int i = (int)(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s), q = v * (1 - f * s), t = v * (1 - (1 - f) * s);

	switch (i%6)
	{
	case 0:
		resR = v;
		resG = t;
		resB = p;
		break;
	case 1:
		resR = q;
		resG = v;
		resB = p;
		break;
	case 2:
		resR = p;
		resG = v;
		resB = t;
		break;
	case 3:
		resR = p;
		resG = q;
		resB = v;
		break;
	case 4:
		resR = t;
		resG = p;
		resB = v;
		break;
	case 5:
		resR = v;
		resG = p;
		resB = q;
		break;
	}

	return RGB(resR*255,resG*255,resB*255);
}
int NEGA::CLAMP(int& i)
{
	if (i > 255) {
		i = 255;
	}
	else if (i < 0) {
		i = 0;
	}
	return i;
}
;
