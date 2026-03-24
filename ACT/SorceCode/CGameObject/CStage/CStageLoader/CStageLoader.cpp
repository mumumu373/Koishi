#include "CStageLoader.h"
#include <fstream> 
#include <sstream> 
#include <crtdbg.h>

CStageLoader::CStageLoader()
    : m_Chip(0, 0, 48, 48)
	, m_MapMax()
	, m_mapData()
{
}

CStageLoader::~CStageLoader()
{
}

// マップデータ読込
bool CStageLoader::LoadMap(const std::string& fileName)
{
    // ファイルを開く
    std::ifstream ifs(fileName);
    if (!ifs.is_open())
    {
        _ASSERT_EXPR(false, L"マップデータ読込失敗: ファイルが見つかりません");
        return false;
    }

    std::string line; // 1行用バッファ

    // 1行目: マップのサイズ（縦,横）を読み込む
    if (std::getline(ifs, line))
    {
        std::stringstream ss(line); // 文字列ストリームに変換
        std::string val;

        // カンマ区切りでyとxを取得
        if (std::getline(ss, val, ',')) m_MapMax.y = (float)std::stoi(val);
        if (std::getline(ss, val, ',')) m_MapMax.x = (float)std::stoi(val);
    }
    else
    {
        return false; // データが空
    }

    // y,x分のサイズで配列作成
    m_mapData.assign((size_t)m_MapMax.y, std::vector<int>((size_t)m_MapMax.x, 0));

    //  2行目以降: マップデータの数値を読み込む 
    for (int y = 0; y < m_MapMax.y; ++y)
    {
        if (!std::getline(ifs, line)) break; // 行が足りなければ中断

        std::stringstream ss(line);
        std::string val;

        for (int x = 0; x < m_MapMax.x; ++x)
        {
            // カンマ区切りで1文字ずつ数値に変換して格納
            if (std::getline(ss, val, ','))
            {
                try {
                    m_mapData[y][x] = std::stoi(val);
                }
                catch (...) {
                    m_mapData[y][x] = 0; // 数値変換できない場合は0
                }
            }
        }
    }
    return true;
}

