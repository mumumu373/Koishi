#pragma once
#include "CImage.h"	//イメージクラス
#include "Global.h"
#include <vector>		//stdのvectorを使えるように
#include <memory>

/***********************************************************************************************************
*		イメージマネージャー
*/

class CImageManager
{
public:
	enum enImgList
	{
		IMG_StageMain = 0,	//ステージのメイン画像(ブロック状の物達)
		IMG_BuckGround1,	//背景1
		IMG_BuckGround2,	//背景2
		IMG_Player,			//プレイヤー
		IMG_Enemy,			//エネミー
		IMG_Mouse,			//マウス
		IMG_String,			//ワイヤー	
		IMG_Wirepoint,		//ワイヤーポイント
	};
public:
	static CImageManager* GetInstance()
	{
		static CImageManager s_Instance;
		return &s_Instance;
	}
	~CImageManager();

	//サウンドデータ読込関数
	bool Load(HWND hWnd, HDC hScreenDC, HDC hMemDC, HDC hWorkDC, HDC hWorkDC2);
	//サウンドデータ開放処理
	void Release();

	static CImage* SelectImg(int FileName) {
		
		return GetInstance()->m_upImg[FileName].get();
	}

private:
	//外部からコンストラクタへのアクセスを禁止する
	CImageManager();
	//コピーコンストラクタによるコピーを禁止する
	CImageManager(const CImageManager& rhs) = delete;
	//代入演算子によるコピーを禁止する
	CImageManager& operator=(CImageManager& rhs) = delete;
	//↑この3つの演算子のセットでシングルトン化が成立する

	//CImage* m_pImg[MAX];	←これみたいな感じ
	std::vector<std::unique_ptr<CImage>> m_upImg;		//ヴェクター(配列上限なし)のCImage型のユニークポインター(ポインター)
};