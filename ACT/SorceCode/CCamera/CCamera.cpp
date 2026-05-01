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
	, m_SnakeCamera(false)
	, m_SnakeCameraSpeed(0,0)
	, m_MemorySnakeSpeed(0,0)
{
	
}

CCamera::~CCamera()
{
}

//ステージ上の座標をカメラ上の座標に変換
VECTOR2_f CCamera::CalcToPositionInCamera(const VECTOR2_f* Pos)
{
	VECTOR2_f pos(0, 0);

	//カメラ内(ウィンドウ内)でのプレイヤー位置
	//カメラの場所が動けばすべての物が動くようにする
	pos.x = Pos->x + m_Center.x - m_Position.x;
	pos.y = Pos->y + m_Center.y - m_Position.y;


	return pos;
}

VECTOR2_f CCamera::CalcToPositionInCamera(const VECTOR2_f Pos)
{
	VECTOR2_f pos(0, 0);

	//カメラ内(ウィンドウ内)でのプレイヤー位置
	//カメラの場所が動けばすべての物が動くようにする
	pos.x = Pos.x + m_Center.x - m_Position.x;
	pos.y = Pos.y + m_Center.y - m_Position.y;


	return pos;
}

VECTOR2_f CCamera::CalcToPositionInStage(const VECTOR2_f* Pos)
{
	VECTOR2_f pos(0, 0);
	pos.x = Pos->x - m_Center.x+ m_Position.x;
	pos.y = Pos->y - m_Center.y+ m_Position.y;
	return  pos;
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
	//揺れるスピードを入れる
	m_SnakeCameraSpeed.x = x;
	m_SnakeCameraSpeed.y = y;

	//スピードを記憶させる
	m_MemorySnakeSpeed = m_SnakeCameraSpeed;

	//カメラを揺らす
	m_SnakeCamera = true;
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

void CCamera::SetBossBattleCamera_Nazrin(VECTOR2_f PlayerPos)
{
	//カメラポジションのベスト位置
	m_CameraMasterPos.x = PlayerPos.x + (WND_W / 2) - 180;
	m_CameraMasterPos.y = PlayerPos.y - (WND_H / 2) + 192 + 1;	//スタック対策
}

void CCamera::SetChangeBossStageCamera(VECTOR2_f* PlayerPos, VECTOR2_f* BossPos)
{
	//カメラポジションのベスト位置と同じようにする
	m_CameraMasterPos.y = (WND_H + 144) - (WND_H / 2) + 1;
	//X座標は、ステージが始まる直前の場所にセットする
	m_CameraMasterPos.x = WND_W;

	PlayerPos->x = PlayerPos->x - (m_Position.x - m_CameraMasterPos.x);
	PlayerPos->y = PlayerPos->y - (m_Position.y - m_CameraMasterPos.y);

	BossPos->x = BossPos->x - (m_Position.x - m_CameraMasterPos.x);
	BossPos->y = BossPos->y - (m_Position.y - m_CameraMasterPos.y);

	m_Position.x = m_CameraMasterPos.x;
	m_Position.y = m_CameraMasterPos.y;
}

void CCamera::BossPhase2Camera()
{
	//カメラの中心点を強制移動させる
	m_CameraMasterPos.x += 2;
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
	m_CameraSpeed.x = (m_CameraMasterPos.x - m_Position.x) / 10;
	m_CameraSpeed.y = (m_CameraMasterPos.y - m_Position.y) / 10;

	//スピード分移動
	m_Position.x += m_CameraSpeed.x;
	m_Position.y += m_CameraSpeed.y;

	//カメラを揺らすなら
	if (m_SnakeCamera == true) {
		//スピード分カメラを移動させる
		m_Position.x += m_SnakeCameraSpeed.x;
		m_Position.y += m_SnakeCameraSpeed.y;

		//スピードが１より大きいなら
		if (m_SnakeCameraSpeed.x > 1 && m_SnakeCameraSpeed.y > 1) {
			//-の数字にする
			m_SnakeCameraSpeed.x -= m_MemorySnakeSpeed.x * 2;
			m_SnakeCameraSpeed.y -= m_MemorySnakeSpeed.y * 2;

			//速度を減らす
			m_SnakeCameraSpeed.x += 1;
			m_SnakeCameraSpeed.y += 1;

			//変わった速度を渡す
			m_MemorySnakeSpeed.x = m_SnakeCameraSpeed.x * -1;
			m_MemorySnakeSpeed.y = m_SnakeCameraSpeed.y * -1;
		}
		else if (m_SnakeCameraSpeed.x < -1 && m_SnakeCameraSpeed.y < -1) {
			//＋の数字にする
			m_SnakeCameraSpeed.x += m_MemorySnakeSpeed.x * 2;
			m_SnakeCameraSpeed.y += m_MemorySnakeSpeed.y * 2;

			//速度を減らす
			m_SnakeCameraSpeed.x -= 1;
			m_SnakeCameraSpeed.y -= 1;

			//変わった速度を渡す
			m_MemorySnakeSpeed.x = m_SnakeCameraSpeed.x;
			m_MemorySnakeSpeed.y = m_SnakeCameraSpeed.y;
		}
		//速度が1未満になったら
		else {
			//初期化
			m_SnakeCamera = false;

			m_SnakeCameraSpeed = { 0,0 };
			m_MemorySnakeSpeed = { 0,0 };
		}
	}
}