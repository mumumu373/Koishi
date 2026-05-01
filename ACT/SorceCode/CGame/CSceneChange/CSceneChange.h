#pragma once
#include "CGameObject/CGameObject.h"	//ゲームオブジェクトクラス

/***********************************************************************************
*		シーンチェンジクラス
*/
class CSceneChange
{
public:
	enum enSceneType
	{
		FadeStart = 0,	//フェードする
		FadeFinish,		//フェード解除
		Up,				//下から上へ
		Down,			//上から下へ
		Right,			//左から右へ
		Left,			//右から左へ
	};

	int m_SceneType = enSceneType::FadeStart;
public:
	bool SceneChangeStart = false;	//シーンチェンジを開始するか

	bool SceneSetComp = false;		//シーンセットが完了したか
	bool NextSceneSetComp = false;	//シーンを終了する方のセットが完了したか

	bool SceneWait = false;			//シーンが待ち状態なら

	bool UseWhiteIMG = false;		//白画面で映すか

	bool BlackScreenTime = false;		//画面が完全に覆いかぶさったか
public:
	CSceneChange();
	~CSceneChange();

	void Update();
	void Draw();

	//何のシーンタイプをやるか、速度、待機時間を決める
	void SetSceneChangeType(int SceneType, double Speed, int WaitTime, bool WhiteIMG);

	//覆いかぶさったところで入力で解除したい時の関数
	void StopScene(bool InputKey) { m_StopScene = InputKey; }

private:
	VECTOR2_f m_Position;

	double m_Speed;		//速度

	int m_WaitTime;		//待つ時間
	int m_WaitTimeCo;	//待つ時間をカウント

	int m_Alpha;		//透明度

	bool m_StopScene;	//シーンをストップさせる
};