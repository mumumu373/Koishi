#pragma once
#include <Windows.h>

#pragma comment( lib, "msimg32.lib" )	//指定色を透過して描画で使用する.
//↑ 半透明の描画でもいる！


/********************************************
*		イメージクラス
*/
class CImage
{
public:
	//コマ割り構造体
	struct FRAMESPLIT
	{
		int x;		//読込ｘ座標
		int y;		//読込ｙ座標
		int w;	//読込表示幅
		int h;		//読込表示の高さ

		FRAMESPLIT(int xSrc=0, int ySrc=0, int wDest=0, int hDest=0)		//デフォルト引数
			:x(xSrc), y(ySrc), w(wDest), h(hDest)
		{ }
	};
public:
	CImage(HDC hScreenDC, HDC hMemDC, HDC hWorkDC, HDC hWorkDC2);
	~CImage();

	//画像の読み込み.
	bool LoadBmp(const char* fileName);

	//画像を表示する関数
	bool BBlt(int xDest, int yDest, int wDest, int hDest,
		int xSrc, int ySrc);

	//画像を透過して表示.
	bool TransBlt(int xDest, int yDest,int wDest, int hDest,
									int xSrc, int ySrc);
	//画像を透過して表示.
	bool TransBltPlas(int xDest, int yDest, int wDest, int hDest,
		int xSrc, int ySrc,int wSrc,int hSrc);

	//画像を半透明にして表示する関数
	bool AlBlend(int xDest, int yDest, int wDest, int hDest,
								  int xSrc, int ySrc, int Alpha);

	//画像を透過＋半透明にして表示する関数
	void TransAlBlend(int xDest, int yDest, int wDest, int hDest,
		int xSrc, int ySrc, int Alpha);

	//画像を透過＋半透明にして表示する関数(画像を詰めたりできる)
	void TransAlBlendPlas(int xDest, int yDest, int wDest, int hDest,
		int xSrc, int ySrc, int wSrc, int hSrc, int Alpha);

	//画像を透過＋半透明＋回転できる描画関数	
	void TransAlBlendRotation(int xDest, int yDest, int wDest, int hDest,
		int xSrc, int ySrc, int wSrc, int hSrc, int Alpha, float Delection);

	//画像を透過＋半透明＋拡大できる描画関数		Bigerが画像を大きくする
	void TransAlBlendExpansion(int xDest, int yDest, int xBiger, int yBiger, int wDest, int hDest,
		int xSrc, int ySrc, int Alpha);

	//カメラ用の描画関数		(カメラのみ使うように)
	void TransAlBlendCamera(int xDest, int yDest, int wDest, int hDest,
		int xSrc, int ySrc, int wSrc, int hSrc, int xBiger, int yBiger, int Alpha, float Delection);

private:
	//=delete [削除定義]　と呼ばれる機能
	//指定された場合、その関数は呼び出せなくなる
	CImage() = delete;		//デフォルトコンストラクタ削除(使用禁止)
private:
	HBITMAP m_hBmp;		//ビットマップ

	HDC			 m_hScreenDC;		//スクリーンDC(ディスプレイ)
	HDC			 m_hMemDC;			//メモリDC
	HDC			 m_hWorkDC;			//作業用DC
	HDC			 m_hWorkDC2;		//作業用DC2
};