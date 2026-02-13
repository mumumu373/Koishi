#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <Windows.h>
#include "MyMath.h"
#include "MyMacro.h"	//SAFE_DELETEを使うため

//===================================
// 定数宣言
//===================================
const int FPS = 60;			//FPS(基準).
const int WND_W = 1280;		//ウィンドウの幅.
const int WND_H = 720;		//ウィンドウの高さ

const double Gravity = 0.8;//ゲーム内の重力

//---------------------------------------
//	構造体宣言.
//---------------------------------------

typedef struct _GameWindow
{
	HWND		hWnd;			//ウィンドウハンドル.
	HDC			hScreenDC;		//バックバッファ.
	SIZE		size;			//サイズ.
	DWORD		dwFPS;			//FPS( Frame Per Second: フレーム / 秒 ).
	BOOL		isWndClose;		//ウィンドウ閉じてるか確認用.
} GameWindow;

//x,yのベクトルをラジアンに変える
double GetRadian(VECTOR2_f Vect);

//正規化にしてくれる関数
VECTOR2_f NormalVector(VECTOR2_f Vect);

//方向にして返す
double GetDelectionVect(VECTOR2_f enemipos, VECTOR2_f player);

//キャラのポジションの距離を計る
double GetHowToLong(VECTOR2_f enemipos, VECTOR2_f player);