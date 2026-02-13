#pragma once
#include "CSound.h"		//サウンドクラス

/********************************************************
*			サウンドマネージャークラス
*			Manager(マネージャー):管理者
*/
class CSoundManager
{
public:
	static const int SOUND_SE_MAX = 10;		//SEのサウンドの最大配列数
public:
	//単体で鳴らす音
	enum enSingleSoundList
	{
		BGM_NoSound,		//無音

		BGM_Title,			//タイトル
		SE_BattleSelect,	//バトルorCPUバトル選択
		SE_HowToPlaySelect,	//遊び方選択

		SE_StartFanfare,	//開始演出の音声
		SE_CountGo,			//Go!
		SE_Count1,			//カウントダウン1
		SE_Count2,			//2
		SE_Count3,			//3
		SE_BattleStart,		//ゲームスタート合図

		BGM_GameMain,		//対戦中BGM
		BGM_TimeUp,			//タイムアップBGM
		SE_Revival,			//復活時
		SE_PlayerBlowAway,	//バリアが割れてぶっ飛ばされた時
		SE_TimeUpWarning,	//タイムアップ10秒前
		SE_TimeUpDamage,	//タイムアップのダメージ

		BGM_Result,			//リザルト
		SE_WinPlayer,		//勝利SE

		//音が増えたら「ここ」に追加
		SingleSound_MAX,						//最大数
	};

	//重複させる音
	enum enMultiSoundList
	{
		SE_Select,				//カーソル選択

		SE_Shot,			//ショット
		SE_ContactPlayer,	//プレイヤーに触れた
		SE_TackleToPlayer,	//タックルでプレイヤーを吹っ飛ばした時
		SE_WallHit,			//ウォールにぶつかった
		SE_WallBreak,		//ウォールが割れた
		SE_Step,			//ステップ
		SE_Tackle,			//タックル
		SE_BulletHit,		//弾にヒットしたら

		//音が増えたら「ここ」に追加
		MultiSound_MAX,		//最大数
	};

public:
	//インスタンス取得(唯一のアクセス経路)
	//※関数の前にstaticをつけることでインスタンス生成しなくても使える

	static CSoundManager* GetInstance()
	{
		//唯一のインスタンスを作成する
		//※staticで作成されたので２回目以降は、下の１行は無視される
		static CSoundManager s_Instance;	//s_:static
		return &s_Instance;
	}
	~CSoundManager();

	//サウンドデータ読込関数
	bool Load(HWND hWnd);
	//サウンドデータ開放処理
	void Release();

	//重複するSEを再生する
	static void PlaySE(enMultiSoundList list)
	{
		//SEが最大数を超えて鳴らそうとしたとき、0番目を止めて最初から流すようにする
		CSoundManager::GetInstance()->SEUpdate();

		bool j = true;

		for (int i = 0; i < SOUND_SE_MAX; i++) {
			//SEはBGMと違って、サウンドの中の関数を使う
			//音が止まっているなら
			if (GetInstance()->m_pSound_SE[i][list]->IsStopped() == true) {		//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
				//最初から流す
				GetInstance()->m_pSound_SE[i][list]->PlaySE();

				j = false;
				break;
			}
		}
		if (j == true) {
			//最初から流す
			GetInstance()->m_pSound_SE[0][list]->Stop();
			GetInstance()->m_pSound_SE[0][list]->PlaySE();
		}
	}
	//重複しないSEを鳴らす
	static void PlaySE_NoDuplication(enSingleSoundList list)
	{
		CSoundManager::GetInstance()->m_pSound_BGM[list]->PlaySE();
	}
	//ループ再生する
	static void PlayLoop(enSingleSoundList list)
	{
		CSoundManager::GetInstance()->m_pSound_BGM[list]->PlayLoop();
	}
	//停止する
	static void Stop(enSingleSoundList list)
	{
		CSoundManager::GetInstance()->m_pSound_BGM[list]->Stop();
	}
	//音が止まっているならを返す
	static bool SingleSoundIsStopped(enSingleSoundList List)
	{
		//曲が止まっているなら
		return CSoundManager::GetInstance()->m_pSound_BGM[List]->IsStopped();
	}
	//すべての音を止める
	static void AllSoundStopped()
	{
		for (int i = 0; i < enSingleSoundList::SingleSound_MAX; i++) {
			CSoundManager::GetInstance()->m_pSound_BGM[i]->Stop();
		}

		for (int i = 0; i < SOUND_SE_MAX; i++) {
			for (int s = 0; s < enMultiSoundList::MultiSound_MAX; s++) {
				CSoundManager::GetInstance()->m_pSound_SE[i][s]->Stop();
			}
		}
	}

	//SEが止まっているなら開始から流す関数
	static void SEUpdate()
	{
		for (int SE = 0; SE < SOUND_SE_MAX; SE++) {
			for (int List = 0; List < enMultiSoundList::MultiSound_MAX; List++) {
				//曲が止まっているなら
				if (CSoundManager::GetInstance()->m_pSound_SE[SE][List]->IsStopped() == true) {
					//SEを止める
					GetInstance()->m_pSound_SE[SE][List]->Stop();
				}
			}
		}
	}

private:

	//外部からコンストラクタへのアクセスを禁止する
	CSoundManager();

	//コピーコンストラクタによるコピーを禁止する
	//=deleteで関数の定義を削除できる
	CSoundManager(const CSoundManager& rhs) = delete;

	//代入演算子によるコピーを禁止する
	//operator(オペレーター);演算子のオーバーロードで演算の中身を拡張できる
	CSoundManager& operator=(CSoundManager& rhs) = delete;

private:
	CSound* m_pSound_SE[SOUND_SE_MAX][enMultiSoundList::MultiSound_MAX];

	CSound* m_pSound_BGM[enSingleSoundList::SingleSound_MAX];
};