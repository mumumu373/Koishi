#pragma once
#include "CGameObject/CGameObject.h"	//ゲームオブジェクトクラス

/***********************************************************************************
*		ムービーシーンの制御クラス
*/
class CMovieScene
{
public:
	//絵文字の種類を入れる
	enum enEmojiKinds
	{
		Execution = 0,	// !
		Question,		// ?
		Cross,			// ×
		Battle,			// 決闘マーク
		Angry,			// 怒りマーク
		Panic,			// 焦りマーク
		Apple,			// リンゴ
	};

	int m_enEmojiKinds = enEmojiKinds::Execution;
public:
	bool StartMovie = false;	//ムービーが始まったか

	bool SceneSetComp = false;	//シーンセットが完了したか

	bool DrawMassage = false;	//メッセージを表示中か
	const int MassageDrawTime = 60 * 10;	//メッセージを表示しておく時間


	const VECTOR2 OffSetPos = { 60,72 };		//シーン(ナズーリンとこいしの会話)オフセット
public:
	CMovieScene();
	~CMovieScene();

	void StartSetting();

	void Draw(std::unique_ptr<CCamera>& pCamera);

	//ボスのムービーシーン突入時の最初のメッセージ
	void BossMovieSceneStart();
	//ボスシーン突入時の会話シーンの動作
	void BossMovieSceneUpdate();

	//次のメッセージに移る
	void NextMessage() { DrawMassage = false; }

	//会話させたいキャラの位置を取得しておく
	void SetPlayerPos(VECTOR2_f PlayerPos) { m_PlayerPos = PlayerPos; }
	void SetBossPos(VECTOR2_f BossPos) { m_BossPos = BossPos; }

public:
	//絵文字の種類にあったフレームをセットする
	void SetEmojiFrame(int Emoji);
private:
	VECTOR2_f m_SpeechBubblePos;	//吹き出しの位置
	CImage::FRAMESPLIT m_SpeechBubbleFrame;		//サイズ
	int m_SpeechBubbleDelection;//吹き出しの角度

	VECTOR2_f m_EmojiPos;			//絵文字の位置
	CImage::FRAMESPLIT m_EmojiFrame;//絵文字の大きさ

	VECTOR2_f m_PlayerPos;
	VECTOR2_f m_BossPos;

	int m_MessageCo;				//メッセージのカウント
};