#include "CSoundManager.h"
#include "MyMacro.h"

CSoundManager::CSoundManager()
{
	//インスタンス生成
	for (int i = 0; i < SOUND_SE_MAX; i++) {
		for (int List = 0; List < enMultiSoundList::MultiSound_MAX; List++) {
			m_pSound_SE[i][List] = new CSound();
		}
	}

	for (int List = 0; List < enSingleSoundList::SingleSound_MAX; List++) {
		m_pSound_BGM[List] = new CSound();
	}
}

CSoundManager::~CSoundManager()
{
	Release();

	//インスタンス破棄
	//SE
	for (int i = 0; i < SOUND_SE_MAX; i++) {
		for (int List = 0; List < enMultiSoundList::MultiSound_MAX; List++) {
			SAFE_DELETE(m_pSound_SE[i][List]);
		}
	}
	//BGM
	for (int List = 0; List < enSingleSoundList::SingleSound_MAX; List++) {
		SAFE_DELETE(m_pSound_BGM[List]);
	}
}

//サウンドデータ読込関数
bool CSoundManager::Load(HWND hWnd)
{
	struct MultiSoundList
	{
		int listNo;		//enList列挙型を設定
		char path[256];	//ファイルの名前(パス付)
		char alias[32];	//エイリアス名
	};
	//重複してなる物
	MultiSoundList SE_List[] =
	{
		{enMultiSoundList::SE_TitleSelect,"Data\\Sound\\SE\\SE_TitleSelect.mp3", "SE_TitleSelect",},

		{enMultiSoundList::SE_PlayerAttack,"Data\\Sound\\SE\\SE_PlayerAttack.mp3", "SE_PlayerAttack",},
		{enMultiSoundList::SE_AttackHit,"Data\\Sound\\SE\\SE_AttackHit.mp3", "SE_AttackHit",},
		{enMultiSoundList::SE_AttackCritkal,"Data\\Sound\\SE\\SE_AttackCritkal.mp3", "SE_AttackCritkal",},
		{enMultiSoundList::SE_Jump,"Data\\Sound\\SE\\SE_Jump.mp3", "SE_Jump",},
		{enMultiSoundList::SE_PlayerHeart,"Data\\Sound\\SE\\SE_PlayerHeart.mp3", "SE_PlayerHeart",},

		{enMultiSoundList::SE_WireCatch,"Data\\Sound\\SE\\SE_WireCatch.mp3", "SE_WireCatch",},
		{enMultiSoundList::SE_EnemyThrow,"Data\\Sound\\SE\\SE_EnemyThrow.mp3", "SE_EnemyThrow",},

		{enMultiSoundList::SE_AvoidancePlayer,"Data\\Sound\\SE\\SE_AvoidancePlayer.mp3", "SE_AvoidancePlayer",},
	};

	//配列の最大要素数を算出(配列全体のサイズ/配列１つ分のサイズ)
	//SE用(配列)に手を加えている
	int SE_List_max = sizeof(SE_List) / sizeof(SE_List[0]);
	for (int i = 0; i < SOUND_SE_MAX; i++) {
		for (int List = 0; List < SE_List_max; List++) {
			if (m_pSound_SE[i][SE_List[List].listNo]->SEOpen(
				SE_List[List].path,
				SE_List[List].alias,
				hWnd,i) == false)
			{
				return false;
			}
		}
	}

	struct SingleSoundList
	{
		int listNo;		//enList列挙型を設定
		char path[256];	//ファイルの名前(パス付)
		char alias[32];	//エイリアス名
	};
	//単体で鳴らすもの
	SingleSoundList BGM_List[] =
	{
		{enSingleSoundList::BGM_NoSound,"Data\\Sound\\BGM\\BGM_NoSound.mp3", "BGM_NoSound"},

		{enSingleSoundList::BGM_TitleStart,"Data\\Sound\\BGM\\BGM_TitleStart.mp3", "BGM_TitleStart",},
		{enSingleSoundList::BGM_Title_1,"Data\\Sound\\BGM\\BGM_Title.mp3", "BGM_Title_1",},
		{enSingleSoundList::BGM_Title_2,"Data\\Sound\\BGM\\BGM_Title.mp3", "BGM_Title_2",},

		{enSingleSoundList::BGM_Stage1_1,"Data\\Sound\\BGM\\BGM_Stage1.mp3", "BGM_Stage1_1",},
		{enSingleSoundList::BGM_Stage1_2,"Data\\Sound\\BGM\\BGM_Stage1.mp3", "BGM_Stage1_2",},

		{enSingleSoundList::BGM_Boss1_1,"Data\\Sound\\BGM\\BGM_Boss1.mp3", "BGM_Boss1_1",},
		{enSingleSoundList::BGM_Boss1_2,"Data\\Sound\\BGM\\BGM_Boss1.mp3", "BGM_Boss1_2",},

		{enSingleSoundList::SE_Decision,"Data\\Sound\\SE\\SE_Decision.mp3", "SE_Decision",},

		{enSingleSoundList::SE_PlayerHit,"Data\\Sound\\SE\\SE_PlayerHit.mp3", "SE_PlayerHit",},

		{enSingleSoundList::SE_BossDead,"Data\\Sound\\SE\\SE_BossDead.mp3", "SE_BossDead",},
		{enSingleSoundList::SE_Dead,"Data\\Sound\\SE\\SE_Dead.mp3", "SE_Dead",},
		{enSingleSoundList::SE_Clear,"Data\\Sound\\SE\\SE_Clear.mp3", "SE_Clear",},
	};

	//配列の最大要素数を算出(配列全体のサイズ/配列１つ分のサイズ)
	int BGM_List_max = sizeof(BGM_List) / sizeof(BGM_List[0]);
	for (int i = 0; i < BGM_List_max; i++) {
		if (m_pSound_BGM[BGM_List[i].listNo]->Open(
			BGM_List[i].path,
			BGM_List[i].alias,
			hWnd) == false)
		{
			return false;
		}
	}
	return true;
}

//サウンドデータ開放処理
void CSoundManager::Release()
{
	//開いた時と逆順で閉じる
	//SE
	for (int i = 0; i < SOUND_SE_MAX; i++) {
		for (int List = 0; List < enMultiSoundList::MultiSound_MAX; List++) {
			if (m_pSound_SE[i][List] != nullptr) {
				m_pSound_SE[i][List]->Close();
			}
		}
	}
	//BGM
	for (int List = 0; List < enSingleSoundList::SingleSound_MAX; List++) {
		if (m_pSound_BGM[List] != nullptr) {
			m_pSound_BGM[List]->Close();
		}
	}
}
