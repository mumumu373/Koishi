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
		{enImgList::IMG_StageMain	,"Data\\MapImage\\StageMain.bmp","MainStage"} ,
		{enImgList::IMG_Koishi		,"Data\\Image\\Koishi.bmp","Koishi"} ,
		{enImgList::IMG_Heart		,"Data\\Image\\Heart.bmp","Heart"} ,
		{enImgList::IMG_BreakeHeart	,"Data\\Image\\BreakeHeart.bmp","BreakeHeart"} ,
		{enImgList::IMG_Enemy		,"Data\\Image\\Enemy.bmp","Enemy"} ,
		{enImgList::IMG_Mouse		,"Data\\Image\\Mouse.bmp"				,"Mouse"} ,
		{enImgList::IMG_String		,"Data\\Image\\String.bmp"				,"String"} ,
		{enImgList::IMG_Wirepoint	,"Data\\Image\\WirePoint.bmp"			,"WirePoint"} ,
		{enImgList::IMG_BackGround	,"Data\\Image\\BackImage\\Back_Ground.bmp"	,"BackGround"} ,
		{enImgList::IMG_BackSky		,"Data\\Image\\BackImage\\Back_Sky.bmp"		,"BackSky"} ,
		{enImgList::IMG_Nazrin		,"Data\\Image\\Nazrina.bmp","Nazrin"} ,
		{enImgList::IMG_Apple		,"Data\\Image\\Apple.bmp","Apple"} ,
		{enImgList::IMG_Banana		,"Data\\Image\\Banana.bmp","Banana"} ,

		{enImgList::IMG_Title_BackSky		,"Data\\Image\\TitleImage\\Title_BackSky.bmp","Title_BackSky"} ,
		{enImgList::IMG_Title_Ground		,"Data\\Image\\TitleImage\\Title_Ground.bmp","Title_Ground"} ,

		{enImgList::IMG_Bullet		,"Data\\Image\\Bullet.bmp","Bullet"} ,
		{enImgList::IMG_Effect		,"Data\\Image\\Effect.bmp","Effect"} ,
		{enImgList::IMG_Slash		,"Data\\Image\\PlayerAttack.bmp"		,"Slash"} ,
		{enImgList::IMG_DeadEffect	,"Data\\Image\\EnemyDeadEffect.bmp"		,"DeadEffect"} ,

		{enImgList::IMG_Cursor		,"Data\\Image\\Cursor.bmp"			,"Cursor"} ,
		{enImgList::IMG_Black		,"Data\\Image\\Black.bmp"			,"Black"} ,
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
