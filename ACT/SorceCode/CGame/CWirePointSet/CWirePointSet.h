#pragma once
#include <iostream>
#include <fstream>
#include "Nlohmann/json.hpp"
#include "CGameObject/CCharacter/CWirepoint/CWirepoint.h"
#include <Windows.h> // 追加
#include <string>

/**********************************************************************************
*		ワイヤーポイントをファイルから読み取りセットするクラス
*/
class CWirePointSet
{
public:
	CWirePointSet();
	~CWirePointSet();

	//ステージ1のワイヤーポイントセット
	static inline void LoadWirePoints_Stage1(std::vector<std::unique_ptr<CWirepoint>>& upWirePoint)
	{
		std::ifstream file("Stage_01_WirePoints.json");
		if (!file.is_open()) {
			MessageBoxA(NULL, "NoFile", "error", MB_OK);
			return;
		}

		json data;
		try {
			file >> data;
		}
		catch (const nlohmann::json::parse_error& e) {
			// ここで具体的なエラー内容を表示する
			std::string errMsg = "JSONの書き方がおかしいです:\n";
			errMsg += e.what();
			MessageBoxA(NULL, errMsg.c_str(), "パースエラー", MB_OK);
			return;
		}

		// "WirePoint" 配列の中身を1つずつ取り出して処理する
		// これなら敵が何人いても、このループだけで全員生成できます
		for (const auto& item : data["WirePoint"])
		{
			// 1. データの読み取り
			double x = item["Pos"]["x"].get<double>();
			double y = item["Pos"]["y"].get<double>();

			VECTOR2_f pos = { x, y };

			upWirePoint.push_back(std::make_unique<CWirepoint>(pos));
		}
	}

	//ステージ2のワイヤーポイント
	static inline void LoadWirePoints_Stage2(std::vector<std::unique_ptr<CWirepoint>>& upWirePoint)
	{
		std::ifstream file("Stage_02_WirePoints.json");
		if (!file.is_open()) {
			MessageBoxA(NULL, "NoFile", "error", MB_OK);
			return;
		}

		json data;
		try {
			file >> data;
		}
		catch (const nlohmann::json::parse_error& e) {
			// ここで具体的なエラー内容を表示する
			std::string errMsg = "JSONの書き方がおかしいです:\n";
			errMsg += e.what();
			MessageBoxA(NULL, errMsg.c_str(), "パースエラー", MB_OK);
			return;
		}

		// "WirePoint" 配列の中身を1つずつ取り出して処理する
		// これなら敵が何人いても、このループだけで全員生成できます
		for (const auto& item : data["WirePoint"])
		{
			// 1. データの読み取り
			double x = item["Pos"]["x"].get<double>();
			double y = item["Pos"]["y"].get<double>();

			VECTOR2_f pos = { x, y };

			upWirePoint.push_back(std::make_unique<CWirepoint>(pos));
		}
	}

	//ステージボスのワイヤーポイント
	static inline void LoadWirePoints_Boss(std::vector<std::unique_ptr<CWirepoint>>& upWirePoint)
	{
		std::ifstream file("Stage_02_WirePoints.json");
		if (!file.is_open()) {
			MessageBoxA(NULL, "NoFile", "error", MB_OK);
			return;
		}

		json data;
		try {
			file >> data;
		}
		catch (const nlohmann::json::parse_error& e) {
			// ここで具体的なエラー内容を表示する
			std::string errMsg = "JSONの書き方がおかしいです:\n";
			errMsg += e.what();
			MessageBoxA(NULL, errMsg.c_str(), "パースエラー", MB_OK);
			return;
		}

		// "WirePoint" 配列の中身を1つずつ取り出して処理する
		// これなら敵が何人いても、このループだけで全員生成できます
		for (const auto& item : data["WirePoint"])
		{
			// 1. データの読み取り
			double x = item["Pos"]["x"].get<double>();
			double y = item["Pos"]["y"].get<double>();

			VECTOR2_f pos = { x, y };

			upWirePoint.push_back(std::make_unique<CWirepoint>(pos));
		}
	}
private:
	using json = nlohmann::json;
};