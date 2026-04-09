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

	void Draw(std::unique_ptr<CCamera>& pCamera);

	void BackGroundDraw(VECTOR2_f CurrentStagePos);

	void SetMapMax(float x, float y) {
		m_MapMax.x = x;
		m_MapMax.y = y;
	}

	void SetMapData(const std::vector<std::vector<int>>& MapData) 
	{ 
		m_mapData = MapData;
	}

private:

	CImage::FRAMESPLIT m_Chip;  // マップチップ1つあたりの幅、高さ

	VECTOR2_f m_MapMax;         // マップデータの縦と横の最大数

	// ステージ情報
	std::vector<std::vector<int>> m_mapData;

	CImage::FRAMESPLIT m_Framesplit;
};