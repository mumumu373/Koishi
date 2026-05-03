#include "CWinlnit/CWinInit.h"
#include <iostream>

//メイン関数.
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,	//インスタンス番号(ウィンドウの番号).
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ PSTR lpCmdLine,
	_In_ int nCmdShow )
{
	
	HWND hWnd;		//ウィンドウハンドル.
	MSG msg;        //メッセージ.

	//ウィンドウ作成クラスのインスタンス生成
	CWinInit* pWinInit = new CWinInit();

	//ウィンドウの作成
	pWinInit->MakeWindow(&hWnd, hInstance);

	// コンソールを作成・coutを設定
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);

	// コンソール位置の調整 
	HWND hConsole = GetConsoleWindow();

	if (hConsole != NULL)
	{
		SetWindowPos(hConsole, NULL,
			0, 0,      // X, Y 座標
			600, 400,  // 幅, 高さ
			SWP_NOZORDER);
	}

	//メッセージループ.
	while( GetMessage( &msg, nullptr, 0, 0 ) > 0 ){
		//ウィンドウ関数にメッセージを送る.
		DispatchMessage( &msg );
	}

	//不要になったので削除
	if (pWinInit != nullptr) {
		delete pWinInit;
		pWinInit = nullptr;

		//コンソールの開放
		FreeConsole();

	}

	return 0;
}

