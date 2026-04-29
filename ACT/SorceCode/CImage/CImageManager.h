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
		IMG_Koishi,			//こいし(プレイヤー)
		IMG_Heart,			//ハート(体力)
		IMG_BreakeHeart,	//ハートの亀裂
		IMG_Enemy,			//エネミー
		IMG_Mouse,			//マウス
		IMG_String,			//ワイヤー	
		IMG_Wirepoint,		//ワイヤーポイント
		IMG_BackGround,		//背景
		IMG_BackSky,		//空
		IMG_Nazrin,			//ナズーリン	1面ボス
		IMG_Apple,			//リンゴ

		IMG_Bullet,			//全員のバレット
		IMG_Effect,			//エフェクト
		IMG_Slash,			//斬撃
		IMG_DeadEffect,		//死んだときのエフェクト(エネミー)

		IMG_Title,			//タイトルの画像
		IMG_Cursor,			//カーソルの画像
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