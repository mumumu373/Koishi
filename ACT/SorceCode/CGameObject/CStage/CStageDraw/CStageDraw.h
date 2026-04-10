#pragma once

#include <vector>  
#include <string>  
#include <memory>
#include "CCamera/CCamera.h"

class CStageDraw
{
public:
	CStageDraw();
	~CStageDraw();

	/*****************************************************************************************
	* @brief    描画関数
	*****************************************************************************************/
	void Draw(std::unique_ptr<CCamera>& pCamera);


	/*****************************************************************************************
	* @brief    背景描画関数
	* @parama   CurrentStagePos : 現在のステージの位置
	*****************************************************************************************/
	void BackGroundDraw(VECTOR2_f CurrentStagePos);


	/*****************************************************************************************
	* @brief		マップの「縦・横」最大数をセットする関数
	*****************************************************************************************/
	void SetMapMax(float x, float y) {
		m_MapMax.x = x;
		m_MapMax.y = y;
	}

	/*****************************************************************************************
	* @brief    背景描画関数
	* @param    MapData : マップデータ
	*****************************************************************************************/
	void SetMapData(const std::vector<std::vector<int>>& MapData) 
	{ 
		m_mapData = MapData;
	}

private:
	void DrawSkyLoop(float LoopX);
	void DrawMountainLoop(float LoopX);

private:

	CImage::FRAMESPLIT m_Chip;  // マップチップ1つあたりの幅、高さ
	VECTOR2_f m_MapMax;         // マップデータの縦と横の最大数

	// ステージ情報
	std::vector<std::vector<int>> m_mapData;

	CImage::FRAMESPLIT m_Framesplit;//キャラクターの切り取り範囲
};