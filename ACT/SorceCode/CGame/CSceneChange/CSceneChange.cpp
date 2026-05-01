#include "CSceneChange.h"

CSceneChange::CSceneChange()
	: m_SceneType()
	, m_Speed()
	, m_WaitTime()
	, m_WaitTimeCo()
	, m_Alpha(0)
{
}

CSceneChange::~CSceneChange()
{
}

void CSceneChange::Update()
{
	//シーンチェンジを始めるなら
	if (SceneChangeStart == true) {
		switch (m_SceneType) {
		case enSceneType::FadeStart:
		case enSceneType::FadeFinish:
			//最初のセットは終わっているので待つところから
			if (SceneSetComp == false || SceneWait == false) {
				if (SceneSetComp == false) {
					m_Alpha += m_Speed;
				}
				else {
					m_Alpha -= m_Speed;
				}

				if (SceneWait == true) {
					//不透明になったら
					if (m_Alpha >= 255) {
						m_Alpha = 255;

						SceneSetComp = true;
					}
				}
				else {
					//透明になったら
					if (m_Alpha <= 0) {
						m_Alpha = 0;

						//2回目のシーンセット完了
						NextSceneSetComp = true;
					}
				}
			}
			break;
		case enSceneType::Up:
			//最初のセットが完了していない || シーンの待ち時間が終了したなら
			if (SceneSetComp == false || SceneWait == false) {
				//下方向に移動
				m_Position.y -= m_Speed;

				if (SceneWait == true) {
					//1よりも大きくなったら
					if (m_Position.y <= 1) {
						//隠す
						m_Position.y = 0;

						//最初のシーンセット完了
						SceneSetComp = true;
					}
				}
				else {
					//左端へ行ったなら
					if (m_Position.y <= -WND_H) {
						m_Position.y = -WND_H;

						//2回目のシーンセット完了
						NextSceneSetComp = true;
					}
				}
			}
			break;
		case enSceneType::Down:
			//最初のセットが完了していない || シーンの待ち時間が終了したなら
			if (SceneSetComp == false || SceneWait == false) {
				//下方向に移動
				m_Position.y += m_Speed;

				if (SceneWait == true) {
					//1よりも大きくなったら
					if (m_Position.y >= 1) {
						//隠す
						m_Position.y = 0;

						//最初のシーンセット完了
						SceneSetComp = true;
					}
				}
				else {
					//左端へ行ったなら
					if (m_Position.y >= WND_H) {
						m_Position.y = WND_H;

						//2回目のシーンセット完了
						NextSceneSetComp = true;
					}
				}
			}
			break;
		case enSceneType::Right:
			//最初のセットが完了していない || シーンの待ち時間が終了したなら
			if (SceneSetComp == false || SceneWait == false) {
				//右方向に移動
				m_Position.x += m_Speed;

				if (SceneWait == true) {
					//1よりも大きくなったら
					if (m_Position.x >= 1) {
						//隠す
						m_Position.x = 0;

						//最初のシーンセット完了
						SceneSetComp = true;
					}
				}
				else {
					//左端へ行ったなら
					if (m_Position.x >= WND_W) {
						m_Position.x = WND_W;

						//2回目のシーンセット完了
						NextSceneSetComp = true;
					}
				}
			}
			break;
		case enSceneType::Left:
			//最初のセットが完了していない || シーンの待ち時間が終了したなら
			if (SceneSetComp == false || SceneWait == false) {
				//左方向に移動
				m_Position.x -= m_Speed;

				if (SceneWait == true) {
					//1よりも小さくなったら
					if (m_Position.x <= 1) {
						//隠す
						m_Position.x = 0;

						//最初のシーンセット完了
						SceneSetComp = true;
					}
				}
				else {
					//左端へ行ったなら
					if (m_Position.x <= -WND_W) {
						m_Position.x = -WND_W;

						//2回目のシーンセット完了
						NextSceneSetComp = true;
					}
				}
			}
			break;
		}

		//シーン待機状態なら
		if (SceneWait == true) {
			//最初のシーンセットが完了したなら
			if (SceneSetComp == true) {
				//時間になったら
				if (m_WaitTimeCo >= m_WaitTime) {
					//待機状態を解除
					SceneWait = false;
				}
				else {
					m_WaitTimeCo++;
				}
			}
		}
		//シーンの待機状態が解除されたら
		else {
			//2回目のシーンセットが完了したら
			if (NextSceneSetComp == true) {
				//動作をやめる
				SceneChangeStart = false;
			}
		}
	}
}

void CSceneChange::Draw()
{
	//白画面か黒画面を映す
	if (UseWhiteIMG == true) {
		CImageManager::SelectImg(CImageManager::enImgList::IMG_White)->TransAlBlend(
			m_Position.x,	//表示位置x座標
			m_Position.y,	//表示位置y座標
			WND_W,			//画像幅
			WND_H,			//高さ	<-拡大して表示するサイズ		
			0,				//元画像x座標
			0,				//元画像y座標
			m_Alpha);		//透明度
	}
	else {
		CImageManager::SelectImg(CImageManager::enImgList::IMG_Black)->TransAlBlend(
			m_Position.x,	//表示位置x座標
			m_Position.y,	//表示位置y座標
			WND_W,			//画像幅
			WND_H,			//高さ	<-拡大して表示するサイズ		
			0,				//元画像x座標
			0,				//元画像y座標
			m_Alpha);		//透明度
	}
}

void CSceneChange::SetSceneChangeType(int SceneType, double Speed, int WaitTime, bool WhiteIMG)
{
	//シーンセットが完了したか
	SceneSetComp = false;		
	//シーンを終了する方のセットが完了したか
	NextSceneSetComp = false;	

	//シーンを待ち状態にする
	SceneWait = true;		

	//シーンのタイプを入れる
	m_SceneType = SceneType;
	//速度
	m_Speed = Speed;
	//待つ時間
	m_WaitTime = WaitTime;
	m_WaitTimeCo = 0;

	//白画面かを入れる
	UseWhiteIMG = WhiteIMG;

	//透明度
	m_Alpha = 255;

	//初期位置
	m_Position = { 0,0 };

	switch (m_SceneType) {
	case enSceneType::FadeStart:
		//徐々に暗くする
		m_Alpha = 0;
		break;
	case enSceneType::FadeFinish:
		//徐々に明るくする
		break;
	case enSceneType::Up:
		//下から上へ
		m_Position.y = WND_H;
		break;
	case enSceneType::Down:
		//上から下へ
		m_Position.y = -WND_H;
		break;
	case enSceneType::Right:
		//左から右へ
		m_Position.x = -WND_W;
		break;
	case enSceneType::Left:
		//右から左へ
		m_Position.x = WND_W;
		break;
	}

	//シーンチェンジ開始！
	SceneChangeStart = true;
}
