#include "CPlayer.h"

CPlayer::CPlayer()
	: m_Jumping(false)
	, m_JumpPower(12.0)
	, m_JumpAcc(0)
	, m_JumpRemove(false)
	, m_JumpRemoveCo(0)
{
	//初期設定でデフォルトにする
	m_Color = enColor::NoColor;
	//自分はプレイヤーです
	m_MyCharacter = enMyCharacter::Player;
	//プレイヤー陣営です
	m_MyCamp = enMyCamp::PlayerCamp;

	StartSetting();
}

CPlayer::~CPlayer()
{
}

void CPlayer::StartSetting()
{
	m_State = enState::Living;

	m_FrameSize = { 64,64 };
	m_Framesplit = { 0,0,144,144 };
	m_Position = { 0,0 };

	//実際の当たり判定
	m_RealFrameSplit = { 104,104 };

	m_Speed = { 10,10 };

	m_OldPosition = m_Position;
}

void CPlayer::Update()
{
	//プレイヤーの動きの制御
	MovePlayer();

	//プレイヤーのジャンプの制御
	//JumpPlayer();
	//デバッグ用動作
	if (GetAsyncKeyState('W') & 0x8000) {
		m_Position.y -= m_Speed.y;
	}
	else if (GetAsyncKeyState('S') & 0x8000) {
		m_Position.y += m_Speed.y;
	}

	KyeInput();
}

void CPlayer::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	//CImageManagerがシングルトン化しているので、サウンドのように使える
	CImageManager::SelectImg(CImageManager::enImgList::IMG_Player)->TransAlBlendRotation(
		DispPos.x,				//表示位置x座標
		DispPos.y,				//表示位置y座標
		m_Framesplit.w,			//画像幅
		m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		m_Alpha, m_Delection);					//透明度、角度
}

void CPlayer::Animation()
{
}

void CPlayer::EnemyHit(int Damage)
{
	m_Position = { 0,0 };
}

void CPlayer::KyeInput()
{
	if (GetAsyncKeyState('A') & 0x8000)
	{
		m_MoveState = enMoveState::MoveLeft;
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		m_MoveState = enMoveState::MoveRight;
	}
	//無操作状態
	else {
		m_MoveState = enMoveState::Wait;
	}
}

void CPlayer::MovePlayer()
{
	switch (m_MoveState) {
	case enMoveState::Wait:
		break;
	case enMoveState::MoveLeft:
		m_Position.x -= m_Speed.x;
		break;
	case enMoveState::MoveRight:
		m_Position.x += m_Speed.x;
		break;
	}
}

void CPlayer::JumpPlayer()
{
	//ジャンプボタンをまだ押していなければ
	if (m_JumpRemove == false) {
		if (GetAsyncKeyState('W') & 0x8000) {
			m_Jumping = true;	//ジャンプ中

			m_JumpAcc = m_JumpPower;
			m_Position.y -= m_JumpPower;

			//押している時間を図る
			if (m_JumpRemoveCo >= 10) {
				m_JumpRemove = true;	//強制的にジャンプボタンを離すようにする
				m_JumpRemoveCo = 0;
			}
			else {
				m_JumpRemoveCo++;
			}
		}
		//ジャンプボタンを離したなら
		else {
			m_JumpRemove = true;
			m_JumpRemoveCo = 0;
		}
	}
	else {
		m_JumpAcc -= Gravity;
		m_Position.y -= m_JumpAcc;
	}
}
