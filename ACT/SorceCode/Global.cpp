#include "Global.h"

double GetRadian(VECTOR2_f Vect)
{
	return sqrt(Vect.x * Vect.x + Vect.y * Vect.y);
}

VECTOR2_f NormalVector(VECTOR2_f Vect)
{
	VECTOR2_f Normalize;
	double Long;
	//取得した値の半径を求める
	Long = GetRadian(Vect);
	if (Long > 0) {
		Normalize.x = Vect.x / Long;
		Normalize.y = Vect.y / Long;
	}

	//移動方向の大きさを1にして返す
	return Normalize;
}

double GetDelectionVect(VECTOR2_f Pos_1, VECTOR2_f Pos_2)
{
	VECTOR2_f Normalize;
	double Long;
	//取得した値の半径を求める
	{
		Long = GetRadian({ Pos_1.x - Pos_2.x, Pos_1.y - Pos_2.y });
	}
	//正規化と同じ
	if (Long > 0) {
		Normalize.x = (Pos_1.x - Pos_2.x) / Long;
		Normalize.y = (Pos_1.y - Pos_2.y) / Long;
	}
	
	//方向にして返す
	return atan2(Normalize.y, Normalize.x);
}

double GetHowToLong(VECTOR2_f Pos_1, VECTOR2_f Pos_2)
{
	//キャラのポジションの距離を計る
	return GetRadian({ Pos_1.x - Pos_2.x, Pos_1.y - Pos_2.y });
}
