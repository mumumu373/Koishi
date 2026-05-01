#pragma once
#include <iostream>
#include <fstream>
#include "Nlohmann/json.hpp"
#include "CGame/Factory/CEnemyFactory/CEnemyFactory.h"
#include <Windows.h> // 追加
#include <string>

/**********************************************************************************
*		エネミーをファイルから読み取りセットするクラス
*/
class CEnemySet
{
public:
	CEnemySet();
	~CEnemySet();

	////保存用(本実装では消すように)
	//static inline std::unique_ptr<CEnemy> Otamesi()
	//{
	//	//jsonファイルを開く
	//	std::ifstream file("EnemyState.json");
	//	//もし開けなければ
	//	if (!file.is_open()) {
	//		// ファイルがない場合はエラーログを出して終了
	//		return nullptr;
	//	}

	//	//このjsonは、読み込むとき動作などを軽くしてくれるライブラリを使ったものの型
	//	json data;
	//	//fileのjsonデータをdataに映す	>>　←これは、右側にある変数にデータを流し込めという意味
	//	file >> data;

	//	//ポジションを入れたいので、jsonファイル側のところから、Enemiesという者たちのまとまりから、Posという箱の中のx,yの数を見る
	//	//.get<float>は、取り出した数値をどの型にして渡すか
	//	//[0]となっているものは、jsonファイルのEnemiesの場所に[]があり、その次の{}の中身が、[0]の次のものになる
	//	//jsonの[]も、配列のようなもの		{}の数だけ配列ができる
	//	VECTOR2_f s = { data["Enemies"][0]["Pos"]["x"].get<float>(),data["Enemies"][0]["Pos"]["y"].get<float>() };

	//	//Colorのところは数値で、文字列ではない
	//	return std::make_unique<CKedama>(data["Enemies"][0]["Color"], s, 60, 5, 5, 5, 60);
	//}

	//static inline std::unique_ptr<CEnemy> otamesi()
	//{
	//	std::ifstream file("EnemyState.json");
	//	if (!file.is_open()) {
	//		MessageBoxA(NULL, "ファイルが見つかりません！", "エラー", MB_OK);
	//		return nullptr;
	//	}

	//	json data;
	//	try {
	//		// ここでエラーが起きるはず
	//		file >> data;

	//		// 値の取り出し
	//		float x = data["Enemies"][0]["Pos"]["x"].get<float>();
	//		float y = data["Enemies"][0]["Pos"]["y"].get<float>();
	//		int color = data["Enemies"][0]["Color"].get<int>();

	//		VECTOR2_f s = { x, y };
	//		return std::make_unique<CKedama>(color, s,60, 5, 5, 5, 60);

	//	}
	//	catch (const std::exception& e) {
	//		// クラッシュする代わりに、エラーの正体をポップアップで表示する！
	//		MessageBoxA(NULL, e.what(), "JSON読み込みエラー！", MB_OK);
	//		return nullptr;
	//	}
	//}

	//ステージ1のエネミーセット
	static inline void LoadEnemies_Stage1(std::vector<std::unique_ptr<CEnemy>>& upEnemy)
	{
		std::ifstream file("Stage_01_Enemies.json");
		if (!file.is_open()) {
			MessageBoxA(NULL, "ファイルが見つかりません！", "エラー", MB_OK);
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

		// "Enemies" 配列の中身を1つずつ取り出して処理する
		// これなら敵が何人いても、このループだけで全員生成できます
		for (const auto& item : data["Enemies"])
		{
			// 1. データの読み取り
			double x = item["Pos"]["x"].get<double>();
			double y = item["Pos"]["y"].get<double>();
			int color = item["Color"].get<int>();
			int Size = item["Size"].get<int>();
			std::string type = item["Type"].get<std::string>();

			VECTOR2_f pos = { x, y };

			// 2. Typeに合わせて作るクラスを変える（ここがスマート！）
			if (type == "Kedama") {
				upEnemy.push_back(CEnemyFactory::CreateKedama(color, pos, Size, item["Speed"].get<double>(),
					item["JumpPower"].get<double>(), item["JumpTime"].get<int>()));
			}
			else if (type == "Fairy") {
				upEnemy.push_back(CEnemyFactory::CreateFairy(color, pos, Size,
					{ item["Speed_X"].get<double>(),item["Speed_Y"].get<double>() },
					item["MoveType"].get<int>(), item["MoveOption1"].get<int>(), item["MoveOption2"].get<int>()));
			}
			else if (type == "YinYangBall") {
				upEnemy.push_back(CEnemyFactory::CreateYinYangBall(color, pos, Size));
			}
		}

	}

	//ステージ2のエネミーセット
	static inline void LoadEnemies_Stage2(std::vector<std::unique_ptr<CEnemy>>& upEnemy)
	{
		std::ifstream file("Stage_02_Enemies.json");
		if (!file.is_open()) {
			MessageBoxA(NULL, "ファイルが見つかりません！", "エラー", MB_OK);
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

		// "Enemies" 配列の中身を1つずつ取り出して処理する
		// これなら敵が何人いても、このループだけで全員生成できます
		for (const auto& item : data["Enemies"])
		{
			// 1. データの読み取り
			double x = item["Pos"]["x"].get<double>();
			double y = item["Pos"]["y"].get<double>();
			int color = item["Color"].get<int>();
			int Size = item["Size"].get<int>();
			std::string type = item["Type"].get<std::string>();

			VECTOR2_f pos = { x, y };

			// 2. Typeに合わせて作るクラスを変える（ここがスマート！）
			if (type == "Kedama") {
				upEnemy.push_back(CEnemyFactory::CreateKedama(color, pos, Size, item["Speed"].get<double>(),
					item["JumpPower"].get<double>(), item["JumpTime"].get<int>()));
			}
			else if (type == "Fairy") {
				upEnemy.push_back(CEnemyFactory::CreateFairy(color, pos, Size,
					{ item["Speed_X"].get<double>(),item["Speed_Y"].get<double>() },
					item["MoveType"].get<int>(), item["MoveOption1"].get<int>(), item["MoveOption2"].get<int>()));
			}
			else if (type == "YinYangBall") {
				upEnemy.push_back(CEnemyFactory::CreateYinYangBall(color, pos, Size));
			}
		}

	}

	//ステージボスのエネミーセット
	static inline void LoadEnemies_Boss(std::vector<std::unique_ptr<CEnemy>>& upEnemy)
	{
		std::ifstream file("Stage_Boss_Enemies.json");
		if (!file.is_open()) {
			MessageBoxA(NULL, "ファイルが見つかりません！", "エラー", MB_OK);
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

		// "Enemies" 配列の中身を1つずつ取り出して処理する
		// これなら敵が何人いても、このループだけで全員生成できます
		for (const auto& item : data["Enemies"])
		{
			// 1. データの読み取り
			double x = item["Pos"]["x"].get<double>();
			double y = item["Pos"]["y"].get<double>();
			int color = item["Color"].get<int>();
			int Size = item["Size"].get<int>();
			std::string type = item["Type"].get<std::string>();

			VECTOR2_f pos = { x, y };

			// 2. Typeに合わせて作るクラスを変える（ここがスマート！）
			if (type == "Kedama") {
				upEnemy.push_back(CEnemyFactory::CreateKedama(color, pos, Size, item["Speed"].get<double>(),
					item["JumpPower"].get<double>(), item["JumpTime"].get<int>()));
			}
			else if (type == "Fairy") {
				upEnemy.push_back(CEnemyFactory::CreateFairy(color, pos, Size,
					{ item["Speed_X"].get<double>(),item["Speed_Y"].get<double>() },
					item["MoveType"].get<int>(), item["MoveOption1"].get<int>(), item["MoveOption2"].get<int>()));
			}
			else if (type == "YinYangBall") {
				upEnemy.push_back(CEnemyFactory::CreateYinYangBall(color, pos, Size));
			}
		}

	}
private:
	using json = nlohmann::json;
};