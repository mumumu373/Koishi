#include "CEnemy.h"

CEnemy::CEnemy()
	: m_PlayerPos()
	, m_AnimationCo(0)
	, m_AnimationTime(20)	//アニメーションを変更する時間を指定
	, m_HitBack(false)
	, m_HitBackCo(0)
	, m_HitBackSpeed(0,0)	//ヒットバックする速度
	, m_ThrowVect(0,0)
	, m_HitStopCo(0)
	, m_Vector(0,0)
	, m_ThrowTimeCo(0)
	, m_DeadAnimCo(0)
{
}

void CEnemy::ThrowEnemy() {
	//エネミーを死亡させる時間まで投げられたら
	if (m_ThrowTimeCo >= ThrowEnemyDeadTime) {
		//エネミーが死んだときの処理
		EnemyIsDead();
	}
	else {
		//ヒットストップしていないなら
		if (HitStop == false) {
			m_Position.x += m_ThrowVect.x;
			m_Position.y -= m_ThrowVect.y;

			m_ThrowVect.y -= Gravity;

			//投げられた時間をカウント
			m_ThrowTimeCo++;
		}
		else {
			//ヒットストップする時間まで止める
			if (m_HitStopCo >= HitEnemyStopTime) {
				m_HitStopCo = 0;
				HitStop = false;
			}
			else {
				m_HitStopCo++;
			}
		}
	}
}

void CEnemy::ThrowEnemyHit(int Damage, VECTOR2_f ThrowEnemyPos)
{
	//HPを減らす
	HP -= Damage;

	//攻撃が当たった
	m_AttackHit = true;

	//攻撃が当たらない時間のカウントをセット
	NoHitAttackCo = 0;

	//半透明に
	m_Alpha = 200;

	//ヒットバック準備
	m_HitBack = true;
	m_HitBackCo = 0;

	//ヒットバックする速度を入れる
	if (ThrowEnemyPos.x >= GetCenterPosition().x) {
		//左に
		m_HitBackSpeed.x = -10;
	}
	else {
		//右に
		m_HitBackSpeed.x = 10;
	}
	m_HitBackSpeed.y = 15;

	//妖精と陰陽玉用にセット
	m_Vector = m_HitBackSpeed;
	//ベクトルを反対方向にする
	m_Vector.y *= -1;

	//クリティカルヒット音をだす
	CSoundManager::PlaySE(CSoundManager::enMultiSoundList::SE_AttackCritkal);
}

void CEnemy::EnemyIsDead()
{
	//死亡させる
	m_State = enState::Dying;
	//アニメーション用のサイズに
	m_FrameSize = { 32,32 };
	//向きを元に戻す
	m_Delection = { 0,0,0 };
	//元画像の描画位置をセット
	m_Framesplit.x = 0;
	m_Framesplit.y = 0;
	//見えるように
	m_Alpha = 255;
}

void CEnemy::DeadAnimationDraw(std::unique_ptr<CCamera>& pCamera)
{
	if (m_DeadAnimCo >= DeadAnimChangeTime) {
		//カウントリセット
		m_DeadAnimCo = 0;

		//次のアニメーションに
		m_Framesplit.x += 32;
		//デッドアニメーションの最後に行ったら
		if (m_Framesplit.x > 32 * 3) {
			//エネミーを消す
			m_State = enState::Dead;
		}
	}
	else {
		m_DeadAnimCo++;
	}

	VECTOR2_f DispPos = pCamera->CalcToPositionInCamera(&m_Position);

	CImageManager::SelectImg(CImageManager::enImgList::IMG_DeadEffect)->TransAlBlendRotation3(
		DispPos.x,				//表示位置x座標
		DispPos.y,				//表示位置y座標
		m_Framesplit.w,			//画像幅
		m_Framesplit.h,			//高さ	<-拡大して表示するサイズ
		m_Framesplit.x,			//元画像x座標
		m_Framesplit.y,			//元画像y座標
		m_FrameSize.x,			//元画像xサイズ		
		m_FrameSize.y,			//元画像yサイズ
		m_Alpha,
		m_Delection.x, m_Delection.y, m_Delection.z);	//透明度、角度
}
