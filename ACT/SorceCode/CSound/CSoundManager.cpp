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
#if 0
	//MIDIまたはmp3ファイルのオープン.
	if (m_pSound[enList::BGM_Title]->Open(
		"Data\\Sound\\BGM\\Title.mp3", "BGM_Title",
		hWnd) == false)
	{
		return false;
	}

	if (m_pSound[enList::SE_Shot]->Open(
		"Data\\Sound\\SE\\Shot.mp3", "SE_Shot",
		hWnd) == false)
	{
		return false;
	}

	if (m_pSound[enList::BGM_Result]->Open(
		"Data\\Sound\\BGM\\Result.mp3", "BGM_Result",
		hWnd) == false)
	{
		return false;
	}

	if (m_pSound[enList::BGM_Bonus]->Open(
		"Data\\Sound\\BGM\\BonusGameHouse.mp3", "BGM_Bonus",
		hWnd) == false) 
	{
		return false;
	}

	if (m_pSound[enList::SE_Jump]->Open(
		"Data\\Sound\\SE\\Jump.wav", "SE_Jump",
		hWnd) == false)
	{
		return false;
	}

	if (m_pSound[enList::SE_Clear]->Open(
		"Data\\Sound\\SE\\Clear.wav", "SE_Clear",
		hWnd) == false)
	{
		return false;
	}
#else
	struct MultiSoundList
	{
		int listNo;		//enList列挙型を設定
		char path[256];	//ファイルの名前(パス付)
		char alias[32];	//エイリアス名
	};
	//重複してなる物
	MultiSoundList SE_List[] =
	{
		{enMultiSoundList::SE_Select,"Data\\Sound\\SE\\SE_Select.mp3", "SE_Select",},
		{enMultiSoundList::SE_Shot,"Data\\Sound\\SE\\SE_Shot.mp3", "SE_Shot",},
		{enMultiSoundList::SE_ContactPlayer,"Data\\Sound\\SE\\SE_ContactPlayer.mp3", "SE_ContactPlayer",},
		{enMultiSoundList::SE_TackleToPlayer,"Data\\Sound\\SE\\SE_TackleToPlayer.mp3", "SE_TackleToPlayer",},
		{enMultiSoundList::SE_WallHit,"Data\\Sound\\SE\\SE_WallHit.mp3", "SE_WallHit",},
		{enMultiSoundList::SE_WallBreak,"Data\\Sound\\SE\\SE_WallBreak.mp3", "SE_WallBreak",},
		{enMultiSoundList::SE_Step,"Data\\Sound\\SE\\SE_Step.mp3", "SE_Step",},
		{enMultiSoundList::SE_Tackle,"Data\\Sound\\SE\\SE_Tackle.mp3", "SE_Tackle",},
		{enMultiSoundList::SE_BulletHit,"Data\\Sound\\SE\\SE_BulletHit.mp3", "SE_BulletHit",},
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

		{enSingleSoundList::BGM_Title,"Data\\Sound\\BGM\\BGM_Title.mp3", "BGM_Title"},
		{enSingleSoundList::SE_BattleSelect,"Data\\Sound\\SE\\SE_BattleSelect.mp3", "SE_BattleSelect",},
		{enSingleSoundList::SE_HowToPlaySelect,"Data\\Sound\\SE\\SE_HowToPlaySelect.mp3", "SE_HowToPlaySelect",},

		{enSingleSoundList::SE_StartFanfare,"Data\\Sound\\SE\\SE_StartFanfare.mp3", "SE_StartFanfare",},
		{enSingleSoundList::SE_Count3,"Data\\Sound\\SE\\SE_Count3.mp3", "SE_Count3",},
		{enSingleSoundList::SE_Count2,"Data\\Sound\\SE\\SE_Count2.mp3", "SE_Count2",},
		{enSingleSoundList::SE_Count1,"Data\\Sound\\SE\\SE_Count1.mp3", "SE_Count1",},
		{enSingleSoundList::SE_CountGo,"Data\\Sound\\SE\\SE_CountGo.mp3", "SE_CountGo",},
		{enSingleSoundList::SE_BattleStart,"Data\\Sound\\SE\\SE_BattleStart.mp3", "SE_BattleStart",},

		{enSingleSoundList::BGM_GameMain,"Data\\Sound\\BGM\\BGM_GameMain.mp3", "BGM_GameMain",},
		{enSingleSoundList::BGM_TimeUp,"Data\\Sound\\BGM\\BGM_TimeUp.mp3", "BGM_TimeUp",},
		{enSingleSoundList::SE_Revival,"Data\\Sound\\SE\\SE_Revival.mp3", "SE_Revival",},
		{enSingleSoundList::SE_PlayerBlowAway,"Data\\Sound\\SE\\SE_PlayerBlowAway.mp3", "SE_PlayerBlowAway",},
		{enSingleSoundList::SE_TimeUpWarning,"Data\\Sound\\SE\\SE_TimeUpWarning.mp3", "SE_TimeUpWarning",},
		{enSingleSoundList::SE_TimeUpDamage,"Data\\Sound\\SE\\SE_TimeUpDamage.mp3", "SE_TimeUpDamage",},

		{enSingleSoundList::BGM_Result,"Data\\Sound\\BGM\\BGM_Result.mp3", "BGM_Result",},
		{enSingleSoundList::SE_WinPlayer,"Data\\Sound\\SE\\SE_WinPlayer.mp3", "SE_WinPlayer",},
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
#endif
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
