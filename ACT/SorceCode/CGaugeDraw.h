#pragma once
#include "CGameObject.h"	//ゲームオブジェクトクラス		継承はしていないが、他で使っているのでここでOK
#include "Global.h"

/******************************************************************************
*		ゲージ描画クラス
*/
class CGaugeDraw
{
public:
	//列挙型の形のみもらうので変数はなし
	enum enPlayerKinds
	{
		Player1 = 0,
		Player2,
	};

	//ゲージの種類	前から描画でなく後ろから描画なので順番を逆に
	enum enGaugeKinds
	{
		Black = 0,			//メインの後ろの背景
		SubGauge,		//サブゲージ	(徐々に減るもの)
		MainGauge,		//メインゲージ
		Main,		//メイン描画

		Max,			//最大レイヤー数
	};

	int m_GaugeKinds = enGaugeKinds::Main;
public:
	CGaugeDraw();
	~CGaugeDraw();

	//ゲージ描画を作る
	void GaugeDrawStart(int GaugeNo);

	//それぞれの役割で動いてもらうためにUpdate
	void Update();

	//ゲージ動作のほうでポジションを決めるので、呼び出されるときにポジションと描画情報を入れる		ポジション、フレーム
	void Draw(CCamera* pCamera, VECTOR3 GaugePos, CImage::FRAMESPLIT FrameSplit, int Alpha);

	//ゲージを使った量を取得
	void UseToGauge(int Gauge);

	//マネージャーから渡されたイメージを持つようにする
	void SetImage(CImage* pImg) { m_pImg = pImg; }

	//ゲージから渡される値を持つようにする	(ゲージの値をサイズに変換する)
	void SetGauge(int Gauge) { m_FrameSizeY = Gauge; }

private:
	CImage* m_pImg;

	int m_MemoryGauge;		//こちら側でゲージをもっておく
	int m_FrameSizeY;		//常にゲージの値を入れる
};