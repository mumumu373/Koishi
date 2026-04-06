#pragma once
#include <iostream>
#include <fstream>
#include "Nlohmann/json.hpp"
#include "CGame/CEnemyFactory/CEnemyFactory.h"
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

	//エネミーのjsonファイルを開く関数
	static inline void LoadEnemies() 
	{
		//jsonファイルを開く
		std::ifstream file("EnemyState.json");
		//ファイルを開けなかったら
		if (!file.is_open()) {
			std::cout << "ファイルが開けませんでした！" << std::endl;
			return;
		}

		json data;
		file >> data;

		std::string type = data["Enemies"][0]["Pos"]["Type"];
		std::cout << "1体目の敵は: " << type << std::endl;
	}

	//保存用(本実装では消すように)
	static inline std::unique_ptr<CEnemy> Otamesi()
	{
		//jsonファイルを開く
		std::ifstream file("EnemyState.json");
		//もし開けなければ
		if (!file.is_open()) {
			// ファイルがない場合はエラーログを出して終了
			return nullptr;
		}

		//このjsonは、読み込むとき動作などを軽くしてくれるライブラリを使ったものの型
		json data;
		//fileのjsonデータをdataに映す	>>　←これは、右側にある変数にデータを流し込めという意味
		file >> data;

		//ポジションを入れたいので、jsonファイル側のところから、Enemiesという者たちのまとまりから、Posという箱の中のx,yの数を見る
		//.get<float>は、取り出した数値をどの型にして渡すか
		//[0]となっているものは、jsonファイルのEnemiesの場所に[]があり、その次の{}の中身が、[0]の次のものになる
		//jsonの[]も、配列のようなもの		{}の数だけ配列ができる
		VECTOR2_f s = { data["Enemies"][0]["Pos"]["x"].get<float>(),data["Enemies"][0]["Pos"]["y"].get<float>() };

		//Colorのところは数値で、文字列ではない
		return std::make_unique<CKedama>(data["Enemies"][0]["Color"], s);
	}

	static inline std::unique_ptr<CEnemy> otamesi()
	{
		std::ifstream file("EnemyState.json");
		if (!file.is_open()) {
			MessageBoxA(NULL, "ファイルが見つかりません！", "エラー", MB_OK);
			return nullptr;
		}

		json data;
		try {
			// ここでエラーが起きるはず
			file >> data;

			// 値の取り出し
			float x = data["Enemies"][0]["Pos"]["x"].get<float>();
			float y = data["Enemies"][0]["Pos"]["y"].get<float>();
			int color = data["Enemies"][0]["Color"].get<int>();

			VECTOR2_f s = { x, y };
			return std::make_unique<CKedama>(color, s);

		}
		catch (const std::exception& e) {
			// クラッシュする代わりに、エラーの正体をポップアップで表示する！
			MessageBoxA(NULL, e.what(), "JSON読み込みエラー！", MB_OK);
			return nullptr;
		}
	}
private:
	using json = nlohmann::json;
};