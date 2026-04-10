#pragma once
#include "CGameObject/CCharacter/CEnemy/CKedama/CKedama.h"
#include "CGameObject/CCharacter/CEnemy/CFairy/CFairy.h"
#include "CGameObject/CCharacter/CEnemy/CYinYangBall/CYinYangBall.h"

/***********************************************************************
*	エネミーを作り出すクラス
*/
static class CEnemyFactory
{
public:
	//ここの部分はcpp内にないと、同じ関数がここで2つ作られて、どっち使えばいいのというエラーが出る
	CEnemyFactory();
	~CEnemyFactory();

	//ファクトリークラスからエネミークラスを返す関数を作る
	//ユニークポインター型のエネミークラスを返すようにする
	static inline std::unique_ptr<CEnemy> CreateKedama(int KedamaColor, VECTOR2_f SetPos)
	{
		//毛玉クラスの誰を作るかを決める
		//ユニークポインターはコピーして渡すことができないので、make_uniqueで新しいユニークポインターを作り、中身をクラスなどで作るようにする
		//毛玉はエネミーを継承しているので返すことができる
		return std::make_unique<CKedama>(KedamaColor, SetPos);
	}
	//エネミーの種類が増えたら下に追加
	//inlineとは、この関数を呼び出したところ(ヘッダーなど)で直接動作させるようなもので、処理負荷が少ない	(今のエネミーファクトリーだとinlineは必要ない)

	//妖精クラスを作成する関数																			
	static inline std::unique_ptr<CEnemy> CreateFairy(int FairyColor, VECTOR2_f SetPos, int MoveType, int MoveOp_1, int MoveOp_2)
	{
		return std::make_unique<CFairy>(FairyColor, SetPos, MoveType, MoveOp_1, MoveOp_2);
	}

	//陰陽玉クラスを作成する関数
	static inline std::unique_ptr<CEnemy> CreateYinYangBall(int YinYangColor, VECTOR2_f SetPos)
	{
		return std::make_unique<CYinYangBall>(YinYangColor, SetPos);
	}
private:

};