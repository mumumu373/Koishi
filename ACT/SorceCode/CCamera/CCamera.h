#pragma once			
#include "MyMath.h"			//VECTOR2用
#include "CImage/CImageManager.h"

/************************************************************
*		カメラクラス
**/
class CCamera
{
public:
	bool m_CameraZoom = false;		//カメラをズームする際にtrueにする
public:
	CCamera();
	~CCamera();
	
	void Update();

	//ステージ上の座標をカメラ上の座標に変換
	VECTOR2_f CalcToPositionInCamera(const VECTOR2_f* Pos);
	//ステージの表示座標を算出
	VECTOR2_f CalcToStagePosition();

	//カメラを揺らす
	void SnakeCamera(int x, int y);

	//カメラのサイズと位置をリセット
	void CameraReset();

	//ステージの幅高さをセットする
	void SetStageSize(double Width, double Heigth) { m_StageSize = { Width ,Heigth }; }
	//セットした物の位置を中心にカメラが動くようにする
	void SetPosition(VECTOR2_f PlayerPos) 
	{ 
		m_CameraMasterPos.x = PlayerPos.x;
		m_CameraMasterPos.y = PlayerPos.y;
	}

private:
	//移動範囲
	void MoveRange();

private:
	const VECTOR2_f m_Center;				//中央位置調整用

	VECTOR2_f m_Position;					//位置(中央基準の座標)
	VECTOR2_f m_CameraMasterPos;			//カメラの中心にしたい物の位置
	VECTOR2_f m_CameraSpeed;				//カメラの移動速度
	CImage::FRAMESPLIT m_Framesplit;		//描画位置とサイズ

	VECTOR2 m_FrameSize;		//元画像のサイズ
	int m_Alpha;				//透明度
	double m_Delection;			//角度

	VECTOR2_f m_FranceSize;		//画像をx,y方向に拡大できる

	VECTOR2_f m_StageSize;		//ステージの大きさ
};
