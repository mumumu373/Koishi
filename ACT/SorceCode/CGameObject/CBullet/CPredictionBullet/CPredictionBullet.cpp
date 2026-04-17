#include "CPredictionBullet.h"

CPredictionBullet::CPredictionBullet(int Camp, VECTOR2_f Pos, int Color, double Speed, double StartAngle, int Size, int ShotTime)
{
	//警告からバレットを出すので、セットから
	m_State = enState::Set;

	//陣営セット
	m_MyCamp = Camp;

	//位置をセット
	m_Position = Pos;

	//属性セット
	m_Color = Color;

	//速さをセット
	m_Speed = { Speed,Speed };

	m_Framesplit = { 0,64,320,128 };

	//元のサイズ
	m_FrameSize = { 64,32 };

	//少し透明にして表示する
	m_Alpha = 150;

	//そのキャラの真ん中から出るようにする
	m_Position.x -= (m_Framesplit.w / 2);
	m_Position.y -= (m_Framesplit.h / 2);
	//元のポジションをセットする
	m_MasterPosition = m_Position;

	//角度を保存しておく
	m_StartAngle = StartAngle;
	//角度を入れる
	m_Delection = StartAngle;

	//バレットを撃ちだす角度を計算
	m_Vector.x = cos((m_StartAngle) / 180 * M_PI) * m_Speed.x;
	m_Vector.y = sin((m_StartAngle) / 180 * M_PI) * m_Speed.y;

	//バレットを撃つタイミングを初期化
	m_BulletShotCo = 0;
	m_BulletShotTime = ShotTime * 60;

	//警告線を出して登場するのでtrue
	m_WarningLineCo = 1;
	m_WarningTime = true;;

	//カメラポジションをリセット
	m_CameraPos = { 0,0 };
}

CPredictionBullet::~CPredictionBullet()
{
}

void CPredictionBullet::Update()
{
	//警告を3回出してから透明になって、バレットが出る
	if (m_WarningLineCo <= 3 || m_WarningTime == false) {
		//警告回数は3回で固定で、その猶予時間をカウントする
		if (m_BulletShotTime / 6 <= m_BulletShotCo) {
			//カウントリセット
			m_BulletShotCo = 0;
			//警告線が出ているなら消す
			if (m_WarningTime == true) {
				//透明にする
				m_Alpha = 0;

				//警告線は出ていません
				m_WarningTime = false;
			}
			else {
				//警告線を出した回数カウント
				m_WarningLineCo++;
				//同じif分だが、3回目の時は通らないようにする
				if (m_WarningLineCo <= 3) {
					//見えるようにする
					m_Alpha = 150;
				}

				//警告線が出ています
				m_WarningTime = true;
			}
		}
		else {
			m_BulletShotCo++;
		}
	}
	else {
		//バレットを打ち出す初期設定
		StartBulletShot();

		//ベクトル分移動していく
		m_Position.x += m_Vector.x;
		m_Position.y += m_Vector.y;
	}
}

void CPredictionBullet::Draw(std::unique_ptr<CCamera>& pCamera)
{
	//アニメーション処理
	Animation();

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	//CImageManagerがシングルトン化しているので、サウンドのように使える
	CImageManager::SelectImg(CImageManager::enImgList::IMG_Bullet)->TransAlBlendRotation(
		DispPos.x,				//表示位置x座標
		DispPos.y,				//表示位置y座標
		m_Framesplit.w,			//画像幅
		m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		m_Alpha, m_Delection);					//透明度、角度

	//カメラのポジションを記憶する
	m_CameraPos = pCamera->GetCameraPos();
}

void CPredictionBullet::CharacterHit()
{
}

void CPredictionBullet::Animation()
{
}

void CPredictionBullet::StartBulletShot()
{
	if (HowStartBulletShot == false) {
		//通りました
		HowStartBulletShot = true;
		//見えるように
		m_Alpha = 255;
		//生存中に
		m_State = enState::Living;
		//大きさと形を変更
		m_Framesplit = { 0,32,128,96 };
	}
}
