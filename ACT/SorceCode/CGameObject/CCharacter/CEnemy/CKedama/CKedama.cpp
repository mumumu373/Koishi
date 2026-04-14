#include "CKedama.h"

CKedama::CKedama(int Kinds, VECTOR2_f SetPos, double MoveSpeed, double JumpPower, int ChangeMoveTime, int JumpingTime)
{
	//ステージに配置する
	m_Position = SetPos;

	//色の種類
	m_Color = Kinds;
	//自分は毛玉キャラクターです
	m_MyCharacter = enMyCharacter::Kedama;
	//左に進みます
	m_MoveState = enMoveState::MoveLeft;
	//エネミー陣営です
	m_MyCamp = enMyCamp::EnemyCamp;

	//スピード
	m_Speed = { MoveSpeed, MoveSpeed };

	//ジャンプ力
	m_JumpPower = JumpPower;
	//ジャンプしているか
	m_Jumping = false;
	//ジャンプするタイミング
	m_JumpingTime = JumpingTime;
	m_JumpingCo = 0;

	//地面に立っているか
	m_GroundStand = false;
	//落下速度の初期化
	m_FallingSpeed = 0;

	StartSetting();

	//ムーブを変えるタイミングを決める
	m_ChangeMoveTime = ChangeMoveTime;
	m_ChangeMoveCo = 0;
}

CKedama::~CKedama()
{
}

void CKedama::StartSetting()
{
	m_State = enState::Living;

	switch (m_Color) {
	case enColor::NoColor:
		m_Framesplit = { 0,0,80,80 };
		break;
	case enColor::Red:
		m_Framesplit = { 32,0,100,100 };
		break;
	case enColor::Yellow:
		m_Framesplit = { 32,0,100,100 };
		break;
	case enColor::Green:
		m_Framesplit = { 32,0,100,100 };
		break;
	case enColor::Blue:
		m_Framesplit = { 0,0,200,200 };
		break;
	}
	//元画像サイズ
	m_FrameSize = { 32,32 };

	//当たり判定
	m_RealFrameSplit = { m_Framesplit.w,m_Framesplit.h };

	m_OldPosition = m_Position;
}

void CKedama::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	CImageManager::SelectImg(CImageManager::enImgList::IMG_Enemy)->TransAlBlendRotation(
		DispPos.x,				//表示位置x座標
		DispPos.y,				//表示位置y座標
		m_Framesplit.w,			//画像幅
		m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		m_Alpha, m_Delection);	//透明度、角度

	RECT rect;
	rect.left = DispPos.x;
	rect.top = DispPos.y ;
	rect.right = DispPos.x + m_RealFrameSplit.x ;
	rect.bottom = DispPos.y + m_RealFrameSplit.y;
	CStageCollisionDraw::GetInstance()->CollisionDraw(rect);

}

void CKedama::Update(std::vector<std::unique_ptr<CBullet>>& upBullet)
{
	m_OldPosition = m_Position;

	switch (m_Color) {
	case enColor::NoColor:
		break;
	case enColor::Red:
		break;
	case enColor::Yellow:
		break;
	case enColor::Green:
		break;
	case enColor::Blue:
		break;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		m_Position.x += 10;
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		m_Position.x -= 10;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		m_FallingSpeed = 0;
		m_Position.y -= 10;
	}

	switch (m_MoveState) {
	case enMoveState::Wait:
		break;
	case enMoveState::MoveLeft:
		//ステージに対してその場所に行けるか判定
		//m_Position.x -= m_Speed.x;
		break;
	case enMoveState::MoveRight:
		//m_Position.x += m_Speed.x;
		break;
	}

	//落下速度を計算
	if (m_FallingSpeed >= MAX_FALLING_SPEED) {
		m_FallingSpeed = MAX_FALLING_SPEED;
	}
	else {
		m_FallingSpeed += Gravity;
	}

	//ジャンプをさせないなら
	if (m_JumpingTime != 0) {
		//地面に立っているなら
		if (m_GroundStand == true) {
			//ジャンプするタイミングなら
			if (m_JumpingCo > m_JumpingTime) {
				//ジャンプする
				m_FallingSpeed = -m_JumpPower;
				m_JumpingCo = 0;

				//地面から離れた
				m_GroundStand = false;
			}
			else {
				m_JumpingCo++;
			}
		}
	}

	//落下するようにする
	m_Position.y += m_FallingSpeed;

	//ステージとの当たり判定を行う
	StageCollision(0,0);
}

void CKedama::Animation()
{
}

void CKedama::StageCollision(double OffsetPos_X, double OffsetPos_Y)
{
	VECTOR2_f offsetPos = { OffsetPos_X, OffsetPos_Y };

	//動いた距離
	double MovePosX = m_Position.x - m_OldPosition.x;
	double MovePosY = m_Position.y - m_OldPosition.y;

	//まだ横に動こうとしているなら
	if (MovePosX != 0.0f) {
		//X方向のブロックに当たったら
		if (CStageCollision::GetInstance()->IsHit(MovePosX,0, m_Framesplit.w, m_Framesplit.h, offsetPos) == true) {
			m_Position.x += MovePosX;
		}
	}

	//まだ縦に動こうとしているなら
	if (MovePosY != 0.0f) {
		//X方向のブロックに当たったら
		if (CStageCollision::GetInstance()->IsHit(MovePosY, 0, m_Framesplit.w, m_Framesplit.h, offsetPos) == true) {
			//m_Position.x += MovePosY;
		}
	}
}
