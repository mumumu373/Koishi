#include "CImageManager.h"

CImageManager::CImageManager()
{
}

CImageManager::~CImageManager()
{
}

bool CImageManager::Load(HWND hWnd, HDC hScreenDC, HDC hMemDC, HDC hWorkDC, HDC hWorkDC2)
{
	struct ImgList
	{
		int listNo;		//enList列挙型を設定
		char path[256];	//ファイルの名前(パス付)
		char alias[32];	//エイリアス名
	};
	ImgList Img_List[]
	{ 
		{enImgList::IMG_StageMain,"Data\\MapImage\\Otamesi.bmp","MainStage"} ,
		{enImgList::IMG_BuckGround1,"Data\\MapImage\\BG1.bmp","BackGround_1"} ,
		{enImgList::IMG_BuckGround2,"Data\\MapImage\\BG2.bmp","BackGround_2"} ,
		{enImgList::IMG_Koishi,"Data\\Image\\Koishi.bmp","Koishi"} ,
		{enImgList::IMG_Heart,"Data\\Image\\Heart.bmp","Heart"} ,
		{enImgList::IMG_Enemy,"Data\\Image\\Enemy.bmp","Enemy"} ,
		{enImgList::IMG_Nazrin,"Data\\Image\\Nazrin.bmp","Nazrin"} ,
		{enImgList::IMG_Bullet,"Data\\Image\\Bullet.bmp","Bullet"} ,
		{enImgList::IMG_Effect,"Data\\Image\\Effect.bmp","Effect"} ,
	};
	int Img_List_max = sizeof(Img_List) / sizeof(Img_List[0]);
	
	//イメージクラスインスタンス生成
	for (int i = 0;i < Img_List_max;++i) {
		m_upImg.push_back(std::make_unique<CImage>(hScreenDC, hMemDC, hWorkDC, hWorkDC2));
	}
	//画像読込		.sizeで作った分の数値を出す
	for (int i = 0;i <m_upImg.size();++i) {
		m_upImg[i].get()->LoadBmp(Img_List[i].path);
	}

	return true;
}

void CImageManager::Release()
{
}
