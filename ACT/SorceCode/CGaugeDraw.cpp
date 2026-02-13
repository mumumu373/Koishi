#include "CGaugeDraw.h"

CGaugeDraw::CGaugeDraw()
	: m_pImg(nullptr)
	, m_MemoryGauge()
	, m_FrameSizeY()
{
}

CGaugeDraw::~CGaugeDraw()
{
	m_pImg = nullptr;
}

//ゲージ描画を作る
void CGaugeDraw::GaugeDrawStart(int GaugeNo)
{
	m_GaugeKinds = GaugeNo;		//配列の数でゲージの種類を決める
	m_MemoryGauge = 256;		//作られたときに256を入れておく
}

void CGaugeDraw::Update()
{
	switch (m_GaugeKinds) {
	case enGaugeKinds::Main:
		m_FrameSizeY = 256;		//メインと黒の背景は最大値で良い
		break;
	case enGaugeKinds::MainGauge:
		//下から描画するために256 - ゲージサイズをしている
		m_FrameSizeY = 256 - m_FrameSizeY;
		break;
	case enGaugeKinds::SubGauge:
		//下から描画するために256 - ゲージサイズをしている
		m_FrameSizeY = 256 - m_FrameSizeY;

		//ゲージがメモリーゲージよりも多いなら
		//ゲージを使ったときに徐々に減るようにする
		if (m_MemoryGauge <= m_FrameSizeY) {		
			m_FrameSizeY = m_MemoryGauge;
		}
		//そうでないならゲージの値に合わせる
		else {
			m_MemoryGauge = m_FrameSizeY;
		}
		break;
	case enGaugeKinds::Black:
		m_FrameSizeY = 256;		//順番的に256になるようにしている
		break;
	}
}

void CGaugeDraw::Draw(CCamera* pCamera, VECTOR3 GaugePos, CImage::FRAMESPLIT FrameSplit, int Alpha)
{

	switch (m_GaugeKinds) {
	case enGaugeKinds::Main:
	case enGaugeKinds::Black:
		//メインとブラックは固定なので分ける
		m_pImg->TransAlBlend(
			GaugePos.x,				//表示位置x座標
			GaugePos.y,				//表示位置y座標
			FrameSplit.w,			//画像幅
			FrameSplit.h + m_FrameSizeY,		//高さ+大きくなった分を増やしていく
			FrameSplit.x,			//元画像x座標
			FrameSplit.y,			//元画像y座標
			255);					//透明度
		break;
	case enGaugeKinds::MainGauge:
	case enGaugeKinds::SubGauge:
		//動的なゲージはこっちで描画
		m_pImg->TransAlBlend(
			GaugePos.x,				//表示位置x座標
			GaugePos.y + m_FrameSizeY,				//表示位置y座標	(256の位置から始まるようにするため)
			FrameSplit.w,			//画像幅
			FrameSplit.h + (256 - m_FrameSizeY),		//高さ+(描画の高さ - ゲージサイズ)ゲージサイズは256からなので - でOK
			FrameSplit.x,			//元画像x座標
			FrameSplit.y,			//元画像y座標
			Alpha);					//透明度
		break;
	}
}

void CGaugeDraw::UseToGauge(int Gauge)
{
	m_FrameSizeY = 256 - Gauge;
	m_MemoryGauge = m_FrameSizeY;
}