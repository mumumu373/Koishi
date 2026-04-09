#define _USE_MATH_DEFINES
#include "CImage.h"
#include <cmath>


//透過色(緑を指定).
const UINT TRANS_COLOR = RGB(0x00, 0xFF, 0x00);

//コンストラクタ(引数あり)
CImage::CImage(HDC hScreenDC, HDC hMemDC, HDC hWorkDC, HDC hWorkDC2)
	:m_hBmp				(nullptr)
	,m_hScreenDC	(hScreenDC)
	,m_hMemDC		(hMemDC)
	,m_hWorkDC		(hWorkDC)
	,m_hWorkDC2		(hWorkDC2)
{
}

//デストラクタ
CImage::~CImage()
{
	DeleteObject(m_hBmp);

	//外部で作られたものなのでnullptrで初期化
	m_hWorkDC2 = nullptr;
	m_hWorkDC = nullptr;
	m_hMemDC = nullptr;
	m_hScreenDC = nullptr;
}

//画像の読み込み.
bool CImage::LoadBmp(const char* fileName )
{
	m_hBmp = (HBITMAP)LoadImage(
		nullptr,			//インスタンス.
		fileName,			//ファイル名(パス含む).
		IMAGE_BITMAP,		//ビットマップ.
		0, 0,				//読み込む座標.
		LR_LOADFROMFILE );	//ファイルから読み込む.

	if( m_hBmp == nullptr ){
		MessageBox( nullptr,
			fileName, "画像読み込み失敗", MB_OK );
		return false;
	}
	return true;
}

//画像を表示する関数
bool CImage::BBlt(int xDest, int yDest, int wDest, int hDest,
										  int xSrc, int ySrc)
{
	SelectObject(m_hMemDC, m_hBmp);

	if (BitBlt(					
		m_hScreenDC,	//デバイスコンテキスト
		xDest, yDest,		//表示位置x,y座標
		wDest, hDest,		//表示幅,高さ
		m_hMemDC,		//読み込み元DC
		xSrc, ySrc,				//読み込み元画像x,y座標
		SRCCOPY) == FALSE)//コピーする
	{
		return false;
	}

	return true;
}

//画像を透過して表示.
bool CImage::TransBlt(int xDest, int yDest,int wDest, int hDest,
													int xSrc, int ySrc )
{

	SelectObject(m_hMemDC, m_hBmp);

	if( TransparentBlt(
		m_hScreenDC,			//表	示先(実はディスプレイ).
		xDest, yDest,		//表示位置x,y座標.
		wDest, hDest,		//表示幅、高さ.
		m_hMemDC,				//画像が設定されたメモリDC.
		xSrc, ySrc,			//元画像の取り出し開始x,y座標.
		wDest, hDest,		//元画像の取り出し幅、高さ.
		TRANS_COLOR )//透過色(緑を指定).
		== FALSE )
	{
		return false;
	}
	return true;
}

bool CImage::TransBltPlas(int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc)
{

	SelectObject(m_hMemDC, m_hBmp);

	if (TransparentBlt(
		m_hScreenDC,			//表	示先(実はディスプレイ).
		xDest, yDest,		//表示位置x,y座標.
		wDest, hDest,		//表示幅、高さ.
		m_hMemDC,				//画像が設定されたメモリDC.
		xSrc, ySrc,			//元画像の取り出し開始x,y座標.
		wSrc, hSrc,			//元画像の取り出し幅、高さ.
		TRANS_COLOR)//透過色(緑を指定).
		== FALSE)
	{
		return false;
	}
	return true;
}

//画像を半透明にして表示する関数
bool CImage::AlBlend(int xDest, int yDest, int wDest, int hDest,
												  int xSrc, int ySrc, 
												  int Alpha)		//a値(二進数)
{
	//BLENDFUNCTION構造体
	BLENDFUNCTION blendfunc = { AC_SRC_OVER,0,(BYTE)Alpha,0 };	//数字の意味が分からん時は定義を覗け

	SelectObject(m_hMemDC, m_hBmp);

	//半透明にする画像の表示
	if (AlphaBlend(
		m_hScreenDC,	//デバイスコンテキスト
		xDest, yDest,				//表示位置x,y座標
		wDest, hDest,			//画像幅、高さ	<-切り取り幅
		m_hMemDC,				//メモリDC
		xSrc, ySrc,				//元画像x,y座標
		wDest, hDest,			//読み込み元画像幅,高さ
		blendfunc))		//BLENDFUNCTION構造体
	{
		return true;
	}
	return false;
}
//画像を透過＋半透明にして表示する関数
void CImage::TransAlBlend(int xDest, int yDest, int wDest, int hDest,
																int xSrc, int ySrc,
																int Alpha)
{
	SelectObject(m_hMemDC, m_hBmp);

	//スクリーン→作業用DCへ
	BitBlt(
		m_hWorkDC,		//出力先:作業用DC
		0, 0,							//出力先:x,yは０で固定
		wDest, hDest,		
		m_hScreenDC,	//読み込み元:スクリーン
		xDest, yDest,		//スクリーン上で表示したいx,y座標
		SRCCOPY);
	//↓
	//メモリDC→作業用DCへ
	TransparentBlt(
		m_hWorkDC,		//出力先:作業用DC
		0, 0,							//出力先:x,yは０で固定
		wDest, hDest,
		m_hMemDC,		//SelectObjectで取得	読み込み元:メモリ
		xSrc , ySrc,
		wDest, hDest,
		TRANS_COLOR);
	//↓
	//作業用DC→スクリーンへ
	BLENDFUNCTION blendfunc = { AC_SRC_OVER,0,(BYTE)Alpha,0 };
	AlphaBlend(
		m_hScreenDC,	//出力先:スクリーン
		xDest, yDest,		//スクリーン上で表示したいx,y座標
		wDest, hDest,
		m_hWorkDC,		//読み込み元:作業用DC
		0, 0,							//出力先:x,yは０で固定
		wDest, hDest,
		blendfunc);

}

void CImage::TransAlBlendPlas(int xDest, int yDest, int wDest, int hDest,
	int xSrc, int ySrc, int wSrc, int hSrc, int Alpha)
{
	SelectObject(m_hMemDC, m_hBmp);

	//スクリーン→作業用DCへ
	BitBlt(
		m_hWorkDC,		//出力先:作業用DC
		0, 0,							//出力先:x,yは０で固定
		wDest, hDest,
		m_hScreenDC,	//読み込み元:スクリーン
		xDest, yDest,		//スクリーン上で表示したいx,y座標
		SRCCOPY);
	//↓
	//メモリDC→作業用DCへ
	TransparentBlt(
		m_hWorkDC,		//表	示先(実はディスプレイ).
		0, 0,		//表示位置x,y座標.
		wDest, hDest,		//表示幅、高さ.
		m_hMemDC,			//画像が設定されたメモリDC.
		xSrc, ySrc,			//元画像の取り出し開始x,y座標.
		wSrc, hSrc,			//元画像の取り出し幅、高さ.
		TRANS_COLOR);		//透過色(緑を指定).

	//↓
	//作業用DC→スクリーンへ
	BLENDFUNCTION blendfunc = { AC_SRC_OVER,0,(BYTE)Alpha,0 };
	AlphaBlend(
		m_hScreenDC,	//出力先:スクリーン
		xDest, yDest,		//スクリーン上で表示したいx,y座標
		wDest, hDest,
		m_hWorkDC,		//読み込み元:作業用DC
		0, 0,							//出力先:x,yは０で固定
		wDest, hDest,
		blendfunc);

}

void CImage::TransAlBlendRotation(int xDest, int yDest,
	int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc, int Alpha, float Delection)
{
	SelectObject(m_hMemDC, m_hBmp);

	//透過する前のキャラ
	BitBlt(
		m_hWorkDC,								//出力先:作業用DC
		0, 0,									//出力先:x,yは０で固定
		wDest * 2, hDest * 2,					//切り取る範囲
		m_hScreenDC,							//読み込み元:スクリーン
		xDest - wDest / 2, yDest - hDest / 2,	//スクリーン上で表示したいx,y座標		切り取った範囲とほかのオブジェクトの描画の位置を合わせるための式		(いじるとミラージュ状態になる)
		SRCCOPY);
	//2個m_hWorkDCをつかう(透過した後に使うキャラ)
	BitBlt(
		m_hWorkDC2,								//出力先:作業用DC2
		0, 0,									//出力先:x,yは０で固定
		wDest * 2, hDest * 2,					//切り取る範囲
		m_hScreenDC,							//読み込み元:スクリーン
		xDest - wDest / 2, yDest - hDest / 2,	//スクリーン上で表示したいx,y座標		切り取った範囲とほかのオブジェクトの描画の位置を合わせるための式		(いじるとミラージュ状態になる)
		SRCCOPY);

	//Visual Studioのライブラリからとってきてる
	POINT Po[3];
	//行列作る変数	角度を入れている
	float Der = Delection * M_PI / 180;

	//左上
	Po[0].x = ((xDest - (xDest + wDest / 2)) * cos(Der) - (yDest - (yDest + hDest / 2)) * sin(Der)) + wDest;
	Po[0].y = ((xDest - (xDest + wDest / 2)) * sin(Der) + (yDest - (yDest + hDest / 2)) * cos(Der)) + hDest;
	//右上
	Po[1].x = ((xDest + wDest - (xDest + wDest / 2)) * cos(Der) - (yDest - (yDest + hDest / 2)) * sin(Der)) + wDest;
	Po[1].y = ((xDest + wDest - (xDest + wDest / 2)) * sin(Der) + (yDest - (yDest + hDest / 2)) * cos(Der)) + hDest;
	//左下
	Po[2].x = ((xDest - (xDest + wDest / 2)) * cos(Der) - (yDest + hDest - (yDest + hDest / 2)) * sin(Der)) + wDest;
	Po[2].y = ((xDest - (xDest + wDest / 2)) * sin(Der) + (yDest + hDest - (yDest + hDest / 2)) * cos(Der)) + hDest;

	//こっちでキャラの回転させて表示する
	PlgBlt(
		m_hWorkDC,			//出力先:作業用DC
		Po, m_hMemDC,		//Poの場所はPOINTの型なので上で作った行列を入れる
		xSrc, ySrc,			//元画像の位置
		wSrc, hSrc,			//元画像の表示範囲	大体 TransparentBlt と同じ	(表示したい大きさ(Framsplit)に元画像の表示範囲を参照して表示してくれる)
		NULL,				//BitMapはいったんNULL	別の用途に使う
		0, 0);				//同じく

	//上で回転させたキャラを読み取って出力する
	TransparentBlt(
		m_hWorkDC2,			//出力先:作業用DC2
		0, 0,				//出力先:x,yは０で固定
		wDest * 2, hDest * 2,	//サイズが大きくなる(切り取った画像が)
		m_hWorkDC,			//読み込み元:作業用DC（背景+透過前キャラ）
		0, 0,
		wDest * 2, hDest * 2,	//サイズが大きくなる(画像自体が)
		TRANS_COLOR);

	//上で出力した m_hWorkDC2 のほうを読み取ってスクリーンに表示する
	BLENDFUNCTION blendfunc = { AC_SRC_OVER,0,(BYTE)Alpha,0 };
	AlphaBlend(
		m_hScreenDC,							//出力先:スクリーン
		xDest - wDest / 2, yDest - hDest / 2,	//スクリーン上で表示したいx,y座標	(描画を大きくとって途切れないようにするための式が入っている)
		wDest * 2, hDest * 2,					//切り取った範囲全体の大きさを変える	
		m_hWorkDC2,								//読み込み元:作業用DC2
		0, 0,									//出力先:x,yは０で固定
		wDest * 2, hDest * 2,					//最終表示範囲を決める	(＊を消すと無理やり半分で表示されて補完される)
		blendfunc);

}

void CImage::TransAlBlendExpansion(int xDest, int yDest, int xBiger, int yBiger,
	int wDest, int hDest, int xSrc, int ySrc, int Alpha)
{
	SelectObject(m_hMemDC, m_hBmp);
	BitBlt(
		m_hWorkDC,		//出力先:作業用DC
		0, 0,							//出力先:x,yは０で固定
		wDest, hDest,
		m_hScreenDC,	//読み込み元:スクリーン
		xDest, yDest,		//スクリーン上で表示したいx,y座標		プレイヤーの表示位置を帰る
		SRCCOPY);
	TransparentBlt(
		m_hWorkDC,		//出力先:作業用DC
		0, 0,							//出力先:x,yは０で固定
		wDest + xBiger, hDest + yBiger,	//サイズが大きくなる(切り取った画像が)
		m_hMemDC,		//SelectObjectで取得	読み込み元:メモリ
		xSrc, ySrc,
		wDest, hDest,	//サイズが大きくなる(画像自体が)
		TRANS_COLOR);

	BLENDFUNCTION blendfunc = { AC_SRC_OVER,0,(BYTE)Alpha,0 };
	AlphaBlend(
		m_hScreenDC,	//出力先:スクリーン
		xDest, yDest,		//スクリーン上で表示したいx,y座標
		wDest, hDest,		//切り取り範囲を拡大する		これを使えば拡大表示できそう
		m_hWorkDC,		//読み込み元:作業用DC
		0, 0,							//出力先:x,yは０で固定
		wDest, hDest,		//画像自体を小さくして切り取っている
		blendfunc);
}

void CImage::TransAlBlendRotation3(int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc, int Alpha, float Delection_X, float Delection_Y, float Delection_Z)
{
	SelectObject(m_hMemDC, m_hBmp);

	//透過する前のキャラ
	BitBlt(
		m_hWorkDC,								//出力先:作業用DC
		0, 0,									//出力先:x,yは０で固定
		wDest * 2, hDest * 2,					//切り取る範囲
		m_hScreenDC,							//読み込み元:スクリーン
		xDest - wDest / 2, yDest - hDest / 2,	//スクリーン上で表示したいx,y座標		切り取った範囲とほかのオブジェクトの描画の位置を合わせるための式		(いじるとミラージュ状態になる)
		SRCCOPY);
	//2個m_hWorkDCをつかう(透過した後に使うキャラ)
	BitBlt(
		m_hWorkDC2,								//出力先:作業用DC2
		0, 0,									//出力先:x,yは０で固定
		wDest * 2, hDest * 2,					//切り取る範囲
		m_hScreenDC,							//読み込み元:スクリーン
		xDest - wDest / 2, yDest - hDest / 2,	//スクリーン上で表示したいx,y座標		切り取った範囲とほかのオブジェクトの描画の位置を合わせるための式		(いじるとミラージュ状態になる)
		SRCCOPY);

	//Visual Studioのライブラリからとってきてる
	Vct3 Po[3];
	Vct3 pos[3];
	int z = 0;	//z軸の値は適当
	//左上
	pos[0].x = xDest - (xDest + wDest / 2);
	pos[0].y = yDest - (yDest + hDest / 2);
	pos[0].z = z;
	//右上
	pos[1].x = xDest + wDest - (xDest + wDest / 2);
	pos[1].y = yDest - (yDest + hDest / 2);
	pos[1].z = z;
	//左下
	pos[2].x = xDest - (xDest + wDest / 2);
	pos[2].y = yDest + hDest - (yDest + hDest / 2);
	pos[2].z = z;
	//行列作る変数	角度を入れている
	//左上
	Po[0] = MakeRotation(pos[0], Delection_X, Delection_Y, Delection_Z);
	//右上
	Po[1] = MakeRotation(pos[1], Delection_X, Delection_Y, Delection_Z);
	//左下
	Po[2] = MakeRotation(pos[2], Delection_X, Delection_Y, Delection_Z);

	POINT Point[3];
	//左上
	Point[0].x = Po[0].x+ wDest;
	Point[0].y = Po[0].y + hDest;
	//右上
	Point[1].x = Po[1].x + wDest;
	Point[1].y = Po[1].y + hDest;
	//左下
	Point[2].x = Po[2].x + wDest;
	Point[2].y = Po[2].y + hDest;

	//こっちでキャラの回転させて表示する
	PlgBlt(
		m_hWorkDC,			//出力先:作業用DC
		Point, m_hMemDC,		//Poの場所はPOINTの型なので上で作った行列を入れる
		xSrc, ySrc,			//元画像の位置
		wSrc, hSrc,			//元画像の表示範囲	大体 TransparentBlt と同じ	(表示したい大きさ(Framsplit)に元画像の表示範囲を参照して表示してくれる)
		NULL,				//BitMapはいったんNULL	別の用途に使う
		0, 0);				//同じく

	//上で回転させたキャラを読み取って出力する
	TransparentBlt(
		m_hWorkDC2,			//出力先:作業用DC2
		0, 0,				//出力先:x,yは０で固定
		wDest * 2, hDest * 2,	//サイズが大きくなる(切り取った画像が)
		m_hWorkDC,			//読み込み元:作業用DC（背景+透過前キャラ）
		0, 0,
		wDest * 2, hDest * 2,	//サイズが大きくなる(画像自体が)
		TRANS_COLOR);

	//上で出力した m_hWorkDC2 のほうを読み取ってスクリーンに表示する
	BLENDFUNCTION blendfunc = { AC_SRC_OVER,0,(BYTE)Alpha,0 };
	AlphaBlend(
		m_hScreenDC,							//出力先:スクリーン
		xDest - wDest / 2, yDest - hDest / 2,	//スクリーン上で表示したいx,y座標	(描画を大きくとって途切れないようにするための式が入っている)
		wDest * 2, hDest * 2,					//切り取った範囲全体の大きさを変える	
		m_hWorkDC2,								//読み込み元:作業用DC2
		0, 0,									//出力先:x,yは０で固定
		wDest * 2, hDest * 2,					//最終表示範囲を決める	(＊を消すと無理やり半分で表示されて補完される)
		blendfunc);

}

void CImage::TransAlBlendCamera(int xDest, int yDest, int wDest, int hDest, int xSrc, int ySrc, int wSrc, int hSrc, int xBiger, int yBiger, int Alpha, float Delection)
{
	SelectObject(m_hMemDC, m_hBmp);

	//透過する前のキャラ
	BitBlt(
		m_hWorkDC,								//出力先:作業用DC
		0, 0,									//出力先:x,yは０で固定
		wDest, hDest,							//切り取る範囲
		m_hScreenDC,							//読み込み元:スクリーン
		xDest, yDest,							//スクリーン上で表示したいx,y座標		切り取った範囲とほかのオブジェクトの描画の位置を合わせるための式		(いじるとミラージュ状態になる)
		SRCCOPY);
	//2個m_hWorkDCをつかう(透過した後に使うキャラ)
	BitBlt(
		m_hWorkDC2,								//出力先:作業用DC2
		0, 0,									//出力先:x,yは０で固定
		wDest, hDest,							//切り取る範囲
		m_hScreenDC,							//読み込み元:スクリーン
		xDest, yDest,							//スクリーン上で表示したいx,y座標		切り取った範囲とほかのオブジェクトの描画の位置を合わせるための式		(いじるとミラージュ状態になる)
		SRCCOPY);

	//Visual Studioのライブラリからとってきてる
	POINT Po[3];
	//行列作る変数	角度を入れている
	float Der = Delection * M_PI / 180;

	//左上
	Po[0].x = ((xDest - (xDest + wDest / 2)) * cos(Der) - (yDest - (yDest + hDest / 2)) * sin(Der)) + wDest / 2;		//一番右のDestは表示位置を示す
	Po[0].y = ((xDest - (xDest + wDest / 2)) * sin(Der) + (yDest - (yDest + hDest / 2)) * cos(Der)) + hDest / 2;
	//右上
	Po[1].x = ((xDest + wDest - (xDest + wDest / 2)) * cos(Der) - (yDest - (yDest + hDest / 2)) * sin(Der)) + wDest / 2;
	Po[1].y = ((xDest + wDest - (xDest + wDest / 2)) * sin(Der) + (yDest - (yDest + hDest / 2)) * cos(Der)) + hDest / 2;
	//左下
	Po[2].x = ((xDest - (xDest + wDest / 2)) * cos(Der) - (yDest + hDest - (yDest + hDest / 2)) * sin(Der)) + wDest / 2;
	Po[2].y = ((xDest - (xDest + wDest / 2)) * sin(Der) + (yDest + hDest - (yDest + hDest / 2)) * cos(Der)) + hDest / 2;

	//こっちでキャラの回転させて表示する
	PlgBlt(
		m_hWorkDC,			//出力先:作業用DC
		Po, m_hMemDC,		//Poの場所はPOINTの型なので上で作った行列を入れる
		xSrc, ySrc,			//元画像の位置
		wSrc, hSrc,			//元画像の表示範囲	大体 TransparentBlt と同じ	(表示したい大きさ(Framsplit)に元画像の表示範囲を参照して表示してくれる)
		NULL,				//BitMapはいったんNULL	別の用途に使う
		0, 0);				//同じく

	//上で回転させたキャラを読み取って出力する
	TransparentBlt(
		m_hWorkDC2,			//出力先:作業用DC2
		0, 0,				//出力先:x,yは０で固定
		wDest, hDest,		//サイズが大きくなる(切り取った画像が)
		m_hWorkDC,			//読み込み元:作業用DC（背景+透過前キャラ）
		0 + xBiger, 0 + yBiger,
		wDest - xBiger * 2, hDest - yBiger * 2,		//サイズが大きくなる(画像自体が)
		TRANS_COLOR);

	//上で出力した m_hWorkDC2 のほうを読み取ってスクリーンに表示する
	BLENDFUNCTION blendfunc = { AC_SRC_OVER,0,(BYTE)Alpha,0 };
	AlphaBlend(
		m_hScreenDC,					//出力先:スクリーン
		xDest, yDest,					//スクリーン上で表示したいx,y座標	(描画を大きくとって途切れないようにするための式が入っている)
		wDest , hDest,					//切り取った範囲全体の大きさを変える		(映っているすべてのものを拡大できる)
		m_hWorkDC2,						//読み込み元:作業用DC2
		0, 0 ,							//出力先:x,yは０で固定
		wDest, hDest,					//最終表示範囲を決める	
		blendfunc);
}

CImage::Vct3 CImage::MakeRotation(Vct3 pos, float Delection_X, float Delection_Y, float Delection_Z)
{
	Vct3 point;
	Vct3 point2;
	double XDer = Delection_X * M_PI / 180;
	double YDer = Delection_Y * M_PI / 180;
	double ZDer = Delection_Z * M_PI / 180;
	//x軸回転
	point.x = pos.x * 1 + pos.y * 0 + pos.z * 0;
	point.y = pos.x * 0 + pos.y * cos(XDer) +pos.z * (-sin(XDer));
	point.z = pos.x * 0 + pos.y * sin(XDer) + pos.z * cos(XDer);
	point2 = point;
	//y軸回転
	point.x = point2.x * cos(YDer) + point2.y * 0 + point2.z * sin(YDer);
	point.y = point2.x * 0 + point2.y * 1 + point2.z * 0;
	point.z = point2.x * (-sin(YDer)) + point2.y * 0 + point2.z * cos(YDer);
	point2 = point;
	//z軸回転
	point.x = point2.x * cos(ZDer) + point2.y * (-sin(ZDer)) + point2.z * 0;
	point.y = point2.x * sin(ZDer) + point2.y * cos(ZDer) + point2.z * 0;
	point.z = point2.x * 0 + point2.y * 0 + point2.z * 1;

	return point;
}
