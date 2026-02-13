#pragma once

#include <Windows.h>

//ライブラリ読込
#pragma comment(lib,"winmm.lib") //音再生で必要


/****************************************************************
*		サウンドクラス
*					midi,mp3,wav形式ファイルの再生・停止などを行う
****************/
class CSound
{
public://public:外部からアクセス可能

	//定数宣言
	static const int STR_BUFFER_MAX = 256;		//文字バッファ最大数

	//コンストラクタ
	//インスタンスの生成時に自動的に呼び出される特殊なメンバ関数

	CSound();

	//デストラクタ
	//インスタンスの破棄されたときに自動的に呼び出される特殊なメンバ関数
	//関数名はクラス名に「~(チルダ)」を付ける

	~CSound();

	//音ファイルを開く
	bool Open(const char* sFileName, const char* sAlias, HWND hWnd);
	//SEの音ファイルを開く
	bool SEOpen(const char* sFileName, const char* sAlias, HWND hWnd, int No);

	//音ファイルを閉じる
	bool Close();

	//音を再生する(状態通知の有無フラグあり)
	bool Play(bool bNotify = false);	//dNotifyは、デフォルト引数

	//音を再生する(SEで使う)
	bool PlaySE();

	//音を再生する(ループ)
	bool PlayLoop();

	//音を停止する
	bool Stop();

	//音の状態を取得する
	bool GetStatus(char* sStatus);

	//音の停止を取得する
	bool IsStopped();

	//音の再生位置を最初にする
	bool SeekToStart();

private://private:外部からアクセス不可能
				 //メンバ変数は基本的にpurivateで宣言する
	//初期値を設定する関数
	void SetInitParam(const char* sAlias, HWND hWnd);

private://private:外部からアクセス不可能
				 //メンバ変数は基本的にpurivateで宣言する
	HWND		m_hWnd;													//ウィンドウハンドル
	char			m_sAlias[STR_BUFFER_MAX];		//エイリアス
};

//命名規則とハンガリアン記法
//m_	:member	(メンバー)
//g_	:global			(グローバル)
//s_	:static			(スタティック)
//iNo	:int					(int型の番号)
//str	:string			(ストリング: 文字列)