#pragma once
#include "CGameObject/CGameObject.h"	//ゲームオブジェクトクラス
#include "Global.h"		//ウィンドウサイズで必要

/**************************************************
*			ステージクラス
**/
class CStage
	:public CGameObject		//ゲームオブジェクトクラス継承
{
public:
	CStage();
	~CStage();

	//動作関数
	void Update() override;

	//描画関数
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	//ステージは複数のImgを持つようにする
	void SetImage(CImage* pStageMainImg,
		CImage* pStageBG_1Img,CImage* pStageBG_2Img) 
	{ 
		m_pStageMainImg = pStageMainImg;
		m_pStageBG_1Img = pStageBG_1Img;
		m_pStageBG_2Img = pStageBG_2Img;
	}

	//ステージ幅を取得.※データの読み込み完了後のみ取得可能
	double GetWidth() { return m_Chip.w * m_MapMax.x; }
	//ステージの高さを取得
	double GetHeight() { return m_Chip.h * m_MapMax.y; }

	//マップデータ読込
	bool LoadData(const char* fileName);

	//マップデータ開放
	void ReleaseData();

private:
	void Animation();

private:
	CImage* m_pStageMainImg;	//ステージメイン画像
	CImage* m_pStageBG_1Img;	//ステージ背景1の画像
	CImage* m_pStageBG_2Img;	//ステージ背景2の画像

	CImage::FRAMESPLIT m_Chip;	//マップチップ画像1つあたりの幅、高さ

	VECTOR2_f m_MapMax;			//マップデータの縦と横の最大数(int型で執り行う)
	int** m_ppData;				//マップデータ(ポインタのポインタorダブルポインタ)
};