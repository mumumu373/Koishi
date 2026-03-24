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
		{enImgList::IMG_StageMain	,"Data\\MapImage\\StageMain.bmp"		,"MainStage"} ,
		{enImgList::IMG_BuckGround1	,"Data\\MapImage\\BackGround_1.bmp"		,"BackGround_1"} ,
		{enImgList::IMG_BuckGround2	,"Data\\MapImage\\BackGround_1.bmp"		,"BackGround_2"} ,
		{enImgList::IMG_Player		,"Data\\Image\\PlayerKoishiDebug.bmp"	,"Player"} ,
		{enImgList::IMG_Enemy		,"Data\\Image\\Enemy.bmp"				,"Enemy"} ,
		{enImgList::IMG_Mouse		,"Data\\Image\\Mouse.bmp"				,"IMG_Mouse"} ,
		{enImgList::IMG_BackGround	,"Data\\Image\\BackImage\\Back_Ground.bmp"	,"BackGround"} ,
		{enImgList::IMG_BackSky		,"Data\\Image\\BackImage\\Back_Sky.bmp"		,"BackSky"} ,
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
