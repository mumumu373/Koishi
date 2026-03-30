#include "CPlayer.h"


#include "CMouseInput//CMouseInput.h"

CPlayer::CPlayer()
	: m_Jumping(false)
	, m_JumpPower(JUMP_POWER)
	, m_JumpAcc(0)
	, m_JumpRemove(false)
	, m_JumpRemoveCo(0)
	, m_WireShot(false)
	, m_WireShotCan(false)
	, m_Acceleration({ 0,0 })
{
	//初期設定でデフォルトにする
	m_Color = enColor::NoColor;
	//自分はプレイヤーです
	m_MyCharacter = enMyCharacter::Player;
	//プレイヤー陣営です
	m_MyCamp = enMyCamp::PlayerCamp;

	StartSetting();

	{
		m_leftkey[0] = false;
		m_rightkey[0] = false;
		m_leftkey[1] = false;
		m_rightkey[1] = false;
		m_Ldashcount = 0;
		m_Rdashcount = 0;
		m_Ldash = false;
		m_Rdash = false;
	}
}

CPlayer::~CPlayer()
{
}

void CPlayer::StartWirePointCatch()
{
	enActionState = enActionState::WirePointCatch;
	
	m_JumpAcc = 0;
	m_Acceleration = { 0,0 };
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
	

	//常にfalseにする
	GroundStand = false;
	//ワイヤーポイントを掴んでいないなら
	if (enActionState !=enActionState::WirePointCatch) {
		//プレイヤーのジャンプの制御
		JumpPlayer();


		KyeInput();
		Dash();
		if (m_Jumping==false) {
			m_Acceleration = { 0,0 };
			//プレイヤーの動きの制御
			MovePlayer();
		}
		if (m_Jumping) {
			MovePlayerJump();
		}
		else {
			MovePlayerGround();
		}

		
	}
	

	//仮置き地面
	if (m_Position.y > 900 - m_Framesplit.h) {
		m_Position.y = 900 - m_Framesplit.h;

		GroundStand = true;
		//地面に着いたらまたジャンプできるように
		m_JumpRemove = false;
		m_Jumping = false;
		m_JumpAcc = 0;
		m_Acceleration.y = 0;
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
	m_Acceleration = Spead;
	m_Jumping = true;

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
	m_leftkey[1] = m_leftkey[0];
	m_leftkey[0] = GetAsyncKeyState('A') & 0x8000;

	m_rightkey[1] = m_rightkey[0];
	m_rightkey[0] = GetAsyncKeyState('D') & 0x8000;


	if (m_leftkey[0])
	{
		m_MoveState = enMoveState::MoveLeft;
		if (m_leftkey[1] == false) {
			if (m_Ldashcount>0) {
				m_Ldash = true;
			}
			else {
				m_Ldashcount = DashcountMAX;
				
			}
		}
	}
	else if (m_rightkey[0])
	{
		m_MoveState = enMoveState::MoveRight;
		if (m_rightkey[1] ==false) {
			if (m_Rdashcount > 0) {
				m_Rdash = true;
			}
			else {
				m_Rdashcount = DashcountMAX;

			}
		}
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

		if (m_Ldash == true) {
			m_Acceleration.x -= m_Speed.x*2;
		}
		else {
			m_Acceleration.x -= m_Speed.x;
		}
		break;
	case enMoveState::MoveRight:
		if (m_Rdash == true) {
			m_Acceleration.x += m_Speed.x*2;
		}
		else {
			m_Acceleration.x += m_Speed.x;
		}

		break;
	}
}

void CPlayer::MovePlayerJump()
{
	m_Position.x += m_Acceleration.x;
	m_Position.y += m_Acceleration.y;
	m_Acceleration.x *= 0.999;
	
	if (m_Acceleration.x<1&& m_Acceleration.x>-1) {
		m_Acceleration.x = 0;
	}

}

void CPlayer::MovePlayerGround()
{
	m_Position.x += m_Acceleration.x;
	m_Position.y += m_Acceleration.y;
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

void CPlayer::Dash()
{
	//keyinputの次に動く
	if (m_Ldashcount > 0) {
		m_Ldashcount--;

	}
	if (m_Rdashcount > 0) {
		m_Rdashcount--;

	}
	switch (m_MoveState) {
	case enMoveState::Wait:
		break;
	case enMoveState::MoveLeft:
		m_Rdashcount = 0;
		m_Rdash = false;
		break;
	case enMoveState::MoveRight:
		m_Ldashcount = 0;
		m_Ldash = false;
		break;
	}
}
