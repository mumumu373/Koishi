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

	//ハートクラス作成
	m_upHeart = std::make_unique<CHeart>();
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

	//属性を変えたかを確認
	m_ChangeColor = false;
}

void CPlayer::Update()
{
}

void CPlayer::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	//CImageManagerがシングルトン化しているので、サウンドのように使える
	CImageManager::SelectImg(CImageManager::enImgList::IMG_Koishi)->TransAlBlendRotation(
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

void CPlayer::Update(std::vector<std::unique_ptr<CBullet>>& upBullet)
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

	//プレイヤーの属性変更制御
	PlayerColorChange();

	KyeInput();
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
