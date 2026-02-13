#include "CCamera.h"
#include "Global.h"

CCamera::CCamera()
	: m_Center(WND_W/2,WND_H/2)			//constなので以降は値の変更不可
	, m_Position(0,0)
	, m_CameraMasterPos(0,0)
	, m_CameraSpeed(0,0)
	, m_Framesplit(0,0,WND_W, WND_H)
	, m_FrameSize(WND_W, WND_H)
	, m_Alpha(255)
	, m_Delection(0)
	, m_FranceSize()
{
	
}

CCamera::~CCamera()
{
}

//ステージ上の座標をカメラ上の座標に変換
VECTOR2_f CCamera::CalcToPositionInCamera(
	const VECTOR2_f* Pos)
{
	VECTOR2_f pos(0, 0);

	//カメラ内(ウィンドウ内)でのプレイヤー位置
	//カメラの場所が動けばすべての物が動くようにする
	pos.x = Pos->x + m_Center.x - m_Position.x;
	pos.y = Pos->y + m_Center.y - m_Position.y;


	return pos;
}

VECTOR2_f CCamera::CalcToStagePosition()
{
	VECTOR2_f pos(0, 0);

	MoveRange();

	//ステージ表示位置
	//ステージ時の表示位置は左基準なので、カメラ中央値をカメラ位置にプラスする必要がある
	pos.x = m_Center.x - m_Position.x;
	pos.y = m_Center.y - m_Position.y;


	return pos;
}

void CCamera::SnakeCamera(int x, int y)
{
	m_Position.x = x;
	m_Position.y = y;
}

//カメラリセット
void CCamera::CameraReset()
{
	//ズームした位置
	m_Framesplit.x = 0;
	m_Framesplit.y = 0;
	//ズーム距離
	m_FranceSize.x = 0;
	m_FranceSize.y = 0;
	//位置
	m_Position.x = 0;
	m_Position.y = 0;
}

void CCamera::MoveRange()
{
	//カメラ移動制限(ステージ左側)
	if (m_Position.x < m_Center.x) {
		m_Position.x = m_Center.x;
	}
	//カメラ移動制限(ステージ右側)
	if (m_Position.x > m_StageSize.x - m_Center.x) {
		m_Position.x = m_StageSize.x - m_Center.x;
	}

	//カメラ移動制限(ステージ上側)
	if (m_Position.y < m_Center.y) {
		m_Position.y = m_Center.y;
	}
	//カメラ移動制限(ステージ下側)
	if (m_Position.y > m_StageSize.y - m_Center.y) {
		m_Position.y = m_StageSize.y - m_Center.y;
	}
}

void CCamera::Update()
{
	//カメラスピードを一度リセットしてから
	m_CameraSpeed = { 0,0 };

	//カメラの中央から離れるほどスピードが上がる
	m_CameraSpeed.x = (m_CameraMasterPos.x - m_Position.x) / 15;
	m_CameraSpeed.y = (m_CameraMasterPos.y - m_Position.y) / 15;

	//スピード分移動
	m_Position.x += m_CameraSpeed.x;
	m_Position.y += m_CameraSpeed.y;
}