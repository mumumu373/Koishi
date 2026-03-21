#include "CStage.h"
#include <stdio.h>		//ファイル入出力で必要
#include <crtdbg.h>		//ASSERTマクロで必要
#include <string.h>		//strtok_s関数で必要
#include <stdlib.h>		//atoi関数で必要	a to i


CStage::CStage()
	: m_pStageMainImg(nullptr)
	, m_pStageBG_1Img(nullptr)
	, m_pStageBG_2Img(nullptr)
	, m_Chip(0,0,48,48)	//48くらいの大きさを想定
	, m_MapMax()
	, m_ppData(nullptr)
{
	m_Position = { 0,0 };
	m_Framesplit = { 0,0,1600,720 };	//画像のサイズを入れる
}

CStage::~CStage()
{
	ReleaseData();

	m_pStageMainImg = nullptr;
	m_pStageBG_1Img = nullptr;
	m_pStageBG_2Img = nullptr;
}

void CStage::Update()
{
}

void CStage::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	//ステージメインの描画
	VECTOR2_f StagePos = pCamera->CalcToStagePosition();

	//ステージ背景2の描画
	CImageManager::SelectImg(CImageManager::enImgList::IMG_BuckGround2)->AlBlend(
		0,				//表示位置x座標
		0,				//表示位置y座標
		m_Framesplit.w,			//画像幅
		m_Framesplit.h,			//高さ	<-切り取り幅
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		255);					//透明度

	//ステージ背景1の描画
	//カメラは参照せずに動かすようにしたい
	CImageManager::SelectImg(CImageManager::enImgList::IMG_BuckGround1)->TransAlBlend(
		0,				//表示位置x座標
		0,				//表示位置y座標
		1280,			//画像幅
		m_Framesplit.h,			//高さ	<-切り取り幅
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		255);					//透明度


	//縦に15個描画
	for (int y = 0; y < m_MapMax.y; y++) {
		for (int x = 0; x < m_MapMax.x; x++) {

			//15番目の画像は透過職なので表示する必要がないためスキップする
			if (m_ppData[y][x] == 15)	continue;

			//外壁や乗れる床(データ依存)
			CImageManager::SelectImg(CImageManager::enImgList::IMG_StageMain)->TransBlt(
				x * m_Chip.w + StagePos.x,	//x方向にチップ幅分ずらして1つずつ表示
				y * m_Chip.h + StagePos.y,	//y方向にチップ高さ分ずらして1つずつ表示
				m_Chip.w,		//切り取り幅
				m_Chip.h,
				m_ppData[y][x] * m_Chip.w,		//データ依存で読込チップ変更
				0);							//元画像のy座標の位置
		}
	}
}

//マップデータ読込
bool CStage::LoadData(const char* fileName)
{
	FILE* pf;
	errno_t error;

	//ファイルを開く
	error = fopen_s(&pf, fileName, "r");
	if (error != 0)			//もし開けなければ
	{
		_ASSERT_EXPR(false, L"マップデータ読込失敗");		//ASSERTでどのファイルの何行目で止まったかを知らせてくれる
		return false;
	}

	//1行分の最大取得文字数
	char line[512] = "";			//""を入れて0で初期化

	char delim[] = ",";			//デリミタ:区切り文字.csvデータなので[,]を指定	[, カンマを受け取る係]
	char* ctx = nullptr;			//内部使用
	char* pNext = nullptr;

	//CSVの読込最初の行にマップチップの「縦の個数」「横の個数」が入っているので特殊な処理を入れる
	if (fgets(line, 512, pf) == nullptr) {
		return false;		//読み込むデータがない
	}
	//一行目データを取り出し","1つ目までを取り出す
	pNext = strtok_s(line, delim, &ctx);
	//1行目のデータがマップデータの「縦の個数」「横の個数」の2種類なので2回処理する
	for (int cnt = 0; cnt < 2; cnt++)
	{
		switch (cnt) {
		case 0:m_MapMax.y = atoi(pNext); break;
		case 1:m_MapMax.x = atoi(pNext); break;
		default:															break;
		}
		pNext = strtok_s(nullptr, delim, &ctx);
	}

	//int型ポインタをマップデータの縦の個数分だけ動的メモリ確保
	m_ppData = new int* [m_MapMax.y]();


	//マップデータ縦の最大数分処理する
	for (int line_no = 0; line_no < m_MapMax.y; line_no++)
	{
		if (fgets(line, 512, pf) == nullptr)
		{
			break;			//読み込むデータが無くなれば終了
		}
		//カンマ区切りで1つデータを取り出す
		pNext = strtok_s(line, delim, &ctx);

		//int型をマップデータの横の個数分だけ動的メモリ確保
		m_ppData[line_no] = new int[m_MapMax.x]();

		//マップデータ横の最大数分処理する
		for (int x = 0; x < m_MapMax.x; x++)
		{
			if (pNext == nullptr)
			{
				break;		//データ無くなったら終了
			}
			m_ppData[line_no][x] = atoi(pNext);					//atoi関数:文字列をint型に変える
			pNext = strtok_s(nullptr, delim, &ctx);		//注意:第一引数は nullptr
		}
	}

	//ファイルを閉じる
	fclose(pf);

	return true;
}

//マップデータ開放
void CStage::ReleaseData()		//int**　関連
{
	//データが入っているか？
	if (m_ppData != nullptr) {
		//マップデータ縦の個数分処理する(配列の最後から順番)
		for (int line_no = m_MapMax.y - 1; line_no >= 0; line_no--)
		{
			//データが入っているか？
			if (m_ppData[line_no] != nullptr) {
				//マップデータ横の個数分を確保したメモリを破棄
				delete[] m_ppData[line_no];				//配列で確保したのでdelete[]を使う
				m_ppData[line_no] = nullptr;			//nullptrで初期化
			}
		}

		//マップデータ縦の個数分を確保したメモリを破棄
		delete[] m_ppData;
		m_ppData = nullptr;
	}

}

void CStage::Animation()
{
	//m_Framesplit = { 0,0,WND_W,WND_H };
}