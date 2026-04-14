#include "CPlayer.h"

#include "CMouseInput//CMouseInput.h"
#include "CGameObject/CStage/CStageCollision/CStageCollision.h"	//ステージ当たり判定クラス

void CPlayer::DrawH(HDC c,HWND h,  std::unique_ptr<CCamera>& pCamera)
{
	NormalAttack->DrawColion(c,h, pCamera);
}

#include<iostream>

constexpr float PlayerCollisionW = 60.f;
constexpr float PlayerCollisionH = 100.0f;


CPlayer::CPlayer()
	: m_Jumping(false)
	, m_JumpPower(JUMP_POWER)
	, m_JumpAcc(0)
	, m_JumpRemove(false)
	, m_JumpRemoveCo(0)
	, m_WireShot(false)
	, m_WireShotCan(false)
	, m_Acceleration({ 0,0 })
	, AvoidanceCount(0)
	, AvoidanceCoolCount(0)
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
	NormalAttack= std::make_unique<CNormalAttack>();

	//ハートクラス作成
	m_upHeart = std::make_unique<CHeart>();
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
	m_Position = { 50,50 };

	//実際の当たり判定
	m_RealFrameSplit = { 104,104 };

	m_Speed = { 10,10 };

	m_OldPosition = m_Position;

	//属性を変えたかを確認
	m_ChangeColor = false;
}

void CPlayer::Attackmove()
{
	if (enActionState== enActionState::Attack) {
		if (NormalAttack->GetAttack() == false) {
			enActionState == enActionState::None;
		}

	}
	else {
		if (NormalAttack->GetAttack() == false) {
			if (CMouseInput::GetMouseLeft(true, true)) {
				NormalAttack->Strat(GetDelectionVect(GetCenterPosition(), CMouseInput::GetMousePosCamera(m_pCamera)), GetCenterPosition());
				enActionState == enActionState::Attack;
			}

		}
	}

		
}

void CPlayer::Update()
{
	NormalAttack->Update();

	m_State = enState::Living;
	m_WireShot = false;

	//過去の自分

	m_OldPosition = m_Position;
	OldGroundStand = GroundStand;

	//常にfalseにする
	GroundStand = false;
	//ワイヤーポイントを掴んでいないなら
	if (enActionState !=enActionState::WirePointCatch) {
		//プレイヤーのジャンプの制御
		JumpPlayer();
		Attackmove();//攻撃
		
		if (AvoidanceCoolCount > 0) {

			AvoidanceCoolCount--;
		}
		else {
			if (enActionState != enActionState::Avoidance&& OldGroundStand==true) {
				KyeInput();
			}
			else {
				if (enActionState != enActionState::AirAvoidance && OldGroundStand == false) {
					AirKeyInput();
				}
			}
			
		}

		
		Dash();
		if (OldGroundStand ==true) {
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
	AvoidanceEnd();

}

void CPlayer::Draw(std::unique_ptr<CCamera>& pCamera)
{
	int i = 0;
	if (enActionState == enActionState::Avoidance) {
		i = 50;
	}
	//アニメーション処理
	Animation();
	m_Delection+=50;

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);
	//CImageManager::SelectImg(CImageManager::enImgList::IMG_Koishi)->TransAlBlendRotation3(
	//	DispPos.x,				//表示位置x座標
	////	m_Framesplit.y,			//元画像y座標
	//	m_Framesplit.w,			//画像幅
	//	m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
	////	m_Alpha, m_Delection);					//透明度、角度
	CImageManager::SelectImg(CImageManager::enImgList::IMG_Koishi)->TransAlBlendRotation3(
		DispPos.x,				//表示位置x座標
		DispPos.y,				//表示位置y座標
		m_Framesplit.h,			//画像幅
		m_Framesplit.w,			//高さ	<-拡大して表示するサイズ		
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		m_Alpha, i, m_Delection, 0);					//透明度、角度

	NormalAttack->Draw(pCamera);

	VECTOR2_f offsetPos = { 40.f,40.f };

	RECT rect;
	rect.left = DispPos.x + offsetPos.x;
	rect.top = DispPos.y + offsetPos.y;
	rect.right = DispPos.x + PlayerCollisionW + offsetPos.x;
	rect.bottom = DispPos.y + PlayerCollisionH + offsetPos.y	;

	CStageCollisionDraw::GetInstance()->CollisionDraw(rect);


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

void CPlayer::Update(std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	//プレイヤーの動きの制御
	//MovePlayer();

	//プレイヤーのジャンプの制御
	//JumpPlayer();

	//プレイヤーの属性変更制御
	PlayerColorChange();

	//KyeInput();

}

void CPlayer::Animation()
{
	switch (m_MoveState) {
	case enMoveState::Wait:
		break;
	case enMoveState::MoveLeft:
		if (m_Delection <= 0) {
			m_Delection = 0;
		}
		else {
			m_Delection -= TurnAroundSpeed;
		}
		break;
	case enMoveState::MoveRight:
		if (m_Delection >= 180) {
			m_Delection = 180;
		}
		else {
			m_Delection += TurnAroundSpeed;
		}
		break;
	}
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
		//-------------------------妖精-------------------------
	case enMyCharacter::Fairy:
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
		//-------------------------陰陽玉-------------------------
	case enMyCharacter::YinYangBall:
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

		m_Acceleration = { 0,0 };//空中の加速度をリセットする
	}
}

void CPlayer::AvoidanceEnd()
{
	if (AvoidanceCount > 0) {
		AvoidanceCount -= 1;
	
			

	}
	else {
		if (AvoidanceCount == 0) {
			AvoidanceCount = -1;//回避状態を終わらせる
			enActionState = enActionState::None;
			m_MoveState = enMoveState::Wait;
			AvoidanceCoolCount = AvoidancecoolTime;//回避のクールタイムを開始する

			m_Acceleration = { 0,0 };//空中の加速度をリセットする
		}
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
		//シフトキーを押しているなら回避状態にする
		if (enActionState != enActionState::WireShot && enActionState != enActionState::WirePointCatch) {
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {			
					enActionState = enActionState::Avoidance;
				AvoidanceCount = AvoidanceTime;
			}
		}
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

		//シフトキーを押しているなら回避状態にする
		if (enActionState != enActionState::WireShot && enActionState != enActionState::WirePointCatch) {
			if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {		
					enActionState = enActionState::Avoidance;
				AvoidanceCount = AvoidanceTime;
			}
		}
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

void CPlayer::AirKeyInput()
{
	if (enActionState != enActionState::WireShot&& enActionState != enActionState::WirePointCatch) {
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {

			enActionState = enActionState::AirAvoidance;
			AirAvoidanceVECTSet();
			AvoidanceCount = AvoidanceTime;
		}
	}
	//ワイヤー発射指示
	if (CMouseInput::GetMouseRight(true, false) && m_WireShotCan) {
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
	if(enActionState == enActionState::AirAvoidance) {
		VECTOR2_f SPin;
		SPin.x = AirAvoidanceVECT.x * AvoidanceDistance / AvoidanceTime;
		SPin.y = AirAvoidanceVECT.y * AvoidanceDistance / AvoidanceTime;
		MoveSafe(SPin.x, 0);
		MoveSafe(0, SPin.y);
	}
	else {
		MoveSafe(m_Acceleration.x, 0);
		MoveSafe(0, m_Acceleration.y);
		m_Acceleration.x *= 0.999;

		if (m_Acceleration.x<1 && m_Acceleration.x>-1) {
			m_Acceleration.x = 0;
		}

	}
}

void CPlayer::MovePlayerGround()
{
	if (enActionState == enActionState::Avoidance) {
		if (m_Acceleration.x <= 0) {
			//左回避状態なら
			float moveX = -(AvoidanceDistance / AvoidanceTime);
			MoveSafe(moveX, 0);
		}
		else {
			float moveX = (AvoidanceDistance / AvoidanceTime);
			MoveSafe(moveX, 0);
		}
		MoveSafe(0, m_Acceleration.y);
	}
	else {
		MoveSafe(m_Acceleration.x, m_Acceleration.y);
	}


}

void CPlayer::JumpPlayer()
{
	//ジャンプボタンをまだ押していなければ
	if (m_JumpRemove == false) {
		if (enActionState != enActionState::Avoidance) {
			if (GetAsyncKeyState('W') & 0x8000) {
				m_Jumping = true;	//ジャンプ中

				m_JumpAcc = m_JumpPower;
				//oveSafe(0, -m_JumpPower);


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

	}
	
		//空中回避状態なら
		if (enActionState != enActionState::AirAvoidance) {
			m_JumpAcc -= Gravity;
			MoveSafe(0, -m_JumpAcc);
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

// 座標を更新する専用の関数
void CPlayer::MoveSafe(float moveX, float moveY)
{
	VECTOR2_f offsetPos = { 40.f, 40.f };

	// X軸移動
	if (moveX != 0.0f) 
	{
		VECTOR2_f nextPosX = m_Position;
		nextPosX.x += moveX;
		if (!CStageCollision::GetInstance()->IsHit(nextPosX, 60, 100, 48, 48, offsetPos)) 
		{
			m_Position.x = nextPosX.x;
		}
		else 
		{
			m_Acceleration.x = 0; // 壁に当たったら速度を殺す
		}
	}

	// Y軸移動
	if (moveY != 0.0f) 
	{
		VECTOR2_f nextPosY = m_Position;
		nextPosY.y += moveY;
		if (!CStageCollision::GetInstance()->IsHit(nextPosY, 60, 100, 48, 48, offsetPos))
		{
			m_Position.y = nextPosY.y;
		}
		else
		{
			// 地面判定
			if (moveY > 0) 
			{
				GroundStand = true;
				m_Jumping = false;
				m_JumpRemove = false;
				m_JumpAcc = 0;
			}

			m_Acceleration.y = 0;
		}
	}
}

void CPlayer::MoveSafeWrier(VECTOR2_f pos)
{
	VECTOR2_f offsetPos = { 40.f, 40.f };

	// X軸移動
	//if (moveX != 0.0f) 
	{
		VECTOR2_f nextPosX = pos;
		nextPosX.y = m_Position.y;
		if (!CStageCollision::GetInstance()->IsHit(nextPosX, 60, 100, 48, 48, offsetPos))
		{
			m_Position.x = nextPosX.x;
		}
		else
		{
			//m_Acceleration.x = 0; // 壁に当たったら速度を殺す
		}
	}

	// Y軸移動
	//if (moveY != 0.0f) 
	{
		VECTOR2_f nextPosY = pos;
		nextPosY.x = m_Position.x;
		if (!CStageCollision::GetInstance()->IsHit(nextPosY, 60, 100, 48, 48, offsetPos))
		{
			m_Position.y = nextPosY.y;
		}
		else
		{

			//m_Acceleration.y = 0;
		}
	}
}


void CPlayer::PlayerColorChange()
{
	//属性を変更する
	if (GetAsyncKeyState('Q') & 0x8000) {
		//色が変わっていたらそれ以降は変えない
		if (m_ChangeColor == false) {
			m_ChangeColor = true;

			m_Color--;

			//変えたときNoColorなら
			if (m_Color < enColor::NoColor) {
				//Blue(最後の列挙)にする
				m_Color = enColor::Blue;
			}

			//ハートのほうの属性も変える
			m_upHeart->ChangeHeartColor(m_Color);
		}
	}
	else if (GetAsyncKeyState('E') & 0x8000) {
		if (m_ChangeColor == false) {
			m_ChangeColor = true;

			m_Color++;

			if (m_Color > enColor::Blue) {
				m_Color = enColor::NoColor;
			}

			m_upHeart->ChangeHeartColor(m_Color);
		}
	}
	//離したら
	else {
		m_ChangeColor = false;
	}
}

void CPlayer::AirAvoidanceVECTSet()
{
	AirAvoidanceVECT = { 0,0 };

	if (GetAsyncKeyState('A') & 0x8000) {
		AirAvoidanceVECT.x = -1;
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		AirAvoidanceVECT.x = 1;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		AirAvoidanceVECT.y = 1;
	}
	if (GetAsyncKeyState('W') & 0x8000) {
		AirAvoidanceVECT.y = -1;
	}
	AirAvoidanceVECT = NormalVector(AirAvoidanceVECT);

}
