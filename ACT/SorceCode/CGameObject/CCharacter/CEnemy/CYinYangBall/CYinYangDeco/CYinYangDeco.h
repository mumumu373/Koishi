#pragma once
#include "CGameObject/CGameObject.h"	//ゲームオブジェクトクラス
#include "Global.h"

/*************************************************************************
*	陰陽玉の周りを飛ぶ謎の物クラス	(装飾)
*/
class CYinYangDeco
	:public CGameObject
{
public:			//ポジション			X速度			Y速度		X方向の距離		Y方向の距離		//開始角度
	CYinYangDeco(VECTOR2_f Pos, double X_Speed, double Y_Speed, double X_Range, double Y_Range, double StartAngle);
	~CYinYangDeco();

	void Update() override {};
	void Update(VECTOR2_f YinYangPos);
	void Draw(std::unique_ptr<CCamera>& pCamera)	override;

private:
	VECTOR2_f m_Speed;	//速度

	VECTOR2_f m_MasterPosition;	//呼び出されたときの位置を記憶しておく
	VECTOR2_f m_NowPosition;	//現在の陰陽玉の位置

	VECTOR2_f m_Angle;		//まわる角度
	VECTOR2_f m_Range;		//キャラからの距離(半径で)

	VECTOR2_f m_FrameSize;	//元画像サイズ

	int m_Alpha;			//透明度
	int m_Delection;		//角度
};