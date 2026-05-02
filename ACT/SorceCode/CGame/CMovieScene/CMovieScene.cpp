#include "CMovieScene.h"

CMovieScene::CMovieScene()
{
	StartSetting();
}

CMovieScene::~CMovieScene()
{
}

void CMovieScene::StartSetting()
{
	StartMovie = false;		//ムービーシーンが始まったか

	SceneSetComp = false;	//ムービーシーンのセットが完了したか

	DrawMassage = false;	//メッセージを表示中か

	m_SpeechBubblePos = { 0,0 };
	m_SpeechBubbleFrame = { 0,0,120,120 };
	m_SpeechBubbleDelection = 0;

	m_EmojiPos = { 0,0 };
	m_EmojiFrame = { 0,0,64,64 };

	//両方に0を入れている
	m_PlayerPos = m_BossPos = { 0,0 };

	m_MessageCo = 0;
}

void CMovieScene::Draw(std::unique_ptr<CCamera>& pCamera)
{
	VECTOR2_f DispPos_1 = pCamera->CalcToPositionInCamera(&m_SpeechBubblePos);

	//吹き出しを表示
	CImageManager::SelectImg(CImageManager::enImgList::IMG_SpeechBubble)->TransAlBlendRotation3(
		DispPos_1.x,			//表示位置x座標
		DispPos_1.y,			//表示位置y座標
		m_SpeechBubbleFrame.w,			//画像幅
		m_SpeechBubbleFrame.h,			//高さ	<-拡大して表示するサイズ
		m_SpeechBubbleFrame.x,			//元画像x座標
		m_SpeechBubbleFrame.y,			//元画像y座標
		32,								//元画像xサイズ		
		32,								//元画像yサイズ
		255,
		0, m_SpeechBubbleDelection, 0);	//透明度、角度	Y軸だけ回転させる

	VECTOR2_f DispPos_2 = pCamera->CalcToPositionInCamera(&m_EmojiPos);

	//絵文字を表示
	CImageManager::SelectImg(CImageManager::enImgList::IMG_Emoji)->TransAlBlendRotation(
		DispPos_2.x,				//表示位置x座標
		DispPos_2.y,				//表示位置y座標
		m_EmojiFrame.w,			//画像幅
		m_EmojiFrame.h,			//高さ	<-拡大して表示するサイズ		
		m_EmojiFrame.x,			//元画像x座標
		m_EmojiFrame.y,			//元画像y座標
		32,						//元画像サイズ
		32,						//元画像サイズ
		255,0);					//透明度
}

void CMovieScene::BossMovieSceneStart()
{
	//ムービーが始まった
	StartMovie = true;

	//メッセージ回数リセット
	m_MessageCo = 0;

	//こいしからスタートする
	m_SpeechBubblePos = m_PlayerPos;

	//位置をセット
	m_SpeechBubblePos.x += OffSetPos.x;
	m_SpeechBubblePos.y -= OffSetPos.y;

	//左からしゃべるように
	m_SpeechBubbleDelection = 180;

	// !マークから表示
	SetEmojiFrame(enEmojiKinds::Execution);

	//メッセージを表示中にする
	DrawMassage = true;

	//吹き出しのど真ん中にセット
	m_EmojiPos.x = m_SpeechBubblePos.x + (m_SpeechBubbleFrame.w / 2) - (m_EmojiFrame.w / 2);
	m_EmojiPos.y = m_SpeechBubblePos.y + (m_SpeechBubbleFrame.h / 2) - (m_EmojiFrame.h / 2);

	//メッセージの表示回数カウント
	m_MessageCo++;
}

void CMovieScene::BossMovieSceneUpdate()
{
	if (DrawMassage == false) {
		switch (m_MessageCo) {
		case 1:
			//ボスの位置から
			m_SpeechBubblePos = m_BossPos;

			m_SpeechBubblePos.x -= OffSetPos.x;
			m_SpeechBubblePos.y -= OffSetPos.y;

			//右から話しているように
			m_SpeechBubbleDelection = 0;

			// ?マーク表示
			SetEmojiFrame(enEmojiKinds::Question);

			//メッセージを表示中にする
			DrawMassage = true;
			break;
		case 2:
			//プレイヤーの位置から
			m_SpeechBubblePos = m_PlayerPos;

			m_SpeechBubblePos.x += OffSetPos.x;
			m_SpeechBubblePos.y -= OffSetPos.y;

			//左から話しているように
			m_SpeechBubbleDelection = 180;

			// マーク表示
			SetEmojiFrame(enEmojiKinds::Apple);

			//メッセージを表示中にする
			DrawMassage = true;
			break;
		case 3:
			//ボスの位置から
			m_SpeechBubblePos = m_BossPos;

			m_SpeechBubblePos.x -= OffSetPos.x;
			m_SpeechBubblePos.y -= OffSetPos.y;

			//右から話しているように
			m_SpeechBubbleDelection = 0;

			// マーク表示
			SetEmojiFrame(enEmojiKinds::Cross);

			//メッセージを表示中にする
			DrawMassage = true;
			break;
		case 4:
			//プレイヤーの位置から
			m_SpeechBubblePos = m_PlayerPos;

			m_SpeechBubblePos.x += OffSetPos.x;
			m_SpeechBubblePos.y -= OffSetPos.y;

			//左から話しているように
			m_SpeechBubbleDelection = 180;

			// マーク表示
			SetEmojiFrame(enEmojiKinds::Battle);

			//メッセージを表示中にする
			DrawMassage = true;
			break;
		case 5:
			//ボスの位置から
			m_SpeechBubblePos = m_BossPos;

			m_SpeechBubblePos.x -= OffSetPos.x;
			m_SpeechBubblePos.y -= OffSetPos.y;

			//右から話しているように
			m_SpeechBubbleDelection = 0;

			// マーク表示
			SetEmojiFrame(enEmojiKinds::Panic);

			//メッセージを表示中にする
			DrawMassage = true;

			//セット完了
			SceneSetComp = true;
			break;
		}

		//吹き出しのど真ん中にセット
		m_EmojiPos.x = m_SpeechBubblePos.x + (m_SpeechBubbleFrame.w / 2) - (m_EmojiFrame.w / 2);
		m_EmojiPos.y = m_SpeechBubblePos.y + (m_SpeechBubbleFrame.h / 2) - (m_EmojiFrame.h / 2);

		//メッセージの表示回数カウント
		m_MessageCo++;
	}
}

void CMovieScene::SetEmojiFrame(int Emoji)
{
	switch (Emoji) {
	case enEmojiKinds::Execution:
	case enEmojiKinds::Question:
	case enEmojiKinds::Cross:
	case enEmojiKinds::Battle:
	case enEmojiKinds::Angry:
	case enEmojiKinds::Panic:
	case enEmojiKinds::Apple:
		//絵文字の画像に合うように入れる
		m_EmojiFrame.x = 32 * Emoji;
		break;
	}
}
