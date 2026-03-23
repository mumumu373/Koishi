#include "CPlayer.h"


#include "CMouseInput//CMouseInput.h"

CPlayer::CPlayer()
	: m_Jumping(false)
	, m_JumpPower(12.0)
	, m_JumpAcc(0)
	, m_JumpRemove(false)
	, m_JumpRemoveCo(0)
	, m_WireShot(false)
	, m_WireShotCan(false)
	, m_WireOutSped({ 0,0 })
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

void CPlayer::StartWirePointCatch()
{
	enActionState = enActionState::WirePointCatch;
	
	m_JumpAcc = 0;
	m_WireOutSped.y = 0;
	m_MoveState = enActionState::None;
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
	m_State = enState::Living;
	m_WireShot = false;

	//過去の自分
	m_OldPosition = m_Position;
	//プレイヤーの動きの制御
	MovePlayer();

	//常にfalseにする
	GroundStand = false;

	if (enActionState !=enActionState::WirePointCatch) {
		//プレイヤーのジャンプの制御
		JumpPlayer();


		KyeInput();
		MovePlayerWireOutSped();
	}
	

	//仮置き地面
	if (m_Position.y > 900 - m_Framesplit.h) {
		m_Position.y = 900 - m_Framesplit.h;

		GroundStand = true;
		//地面に着いたらまたジャンプできるように
		m_JumpRemove = false;
		m_Jumping = false;
		m_JumpAcc = 0;
		m_WireOutSped.y = 0;
	}
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

void CPlayer::WireEnd(VECTOR2_f Spead)
{
	enActionState = enActionState::WireShot;
	m_WireOutSped = Spead;
}

double CPlayer::GetWireStartSpeed()
{
	double Speed = m_Position.y  - m_OldPosition.y;

	if(Speed>0){
		return Speed;
	}
	return 0;
}

void CPlayer::Animation()
{
}

void CPlayer::EnemyHit(int Enemy, int Color)
{
	//当たったエネミーが誰か
	switch (Enemy) {
		//-------------------------毛玉--------------------------
	case enMyCharacter::Kedama:
		switch (Color) {
		case enColor::NoColor:
		case enColor::Red:
		case enColor::Yellow:
		case enColor::Green:
		case enColor::Blue:
			m_Position = { 0,0 };
			break;
		}
		break;
	}
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
	//ワイヤー発射指示
	if (CMouseInput::GetMouseRight(true, false)&& m_WireShotCan) {
		m_WireShot = true;
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

void CPlayer::MovePlayerWireOutSped()
{
	m_Position.x += m_WireOutSped.x;
	m_Position.y += m_WireOutSped.y;
	m_WireOutSped.x *= 0.9;
	
	if (m_WireOutSped.x<1&& m_WireOutSped.x-1) {
		m_WireOutSped.x = 0;
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
