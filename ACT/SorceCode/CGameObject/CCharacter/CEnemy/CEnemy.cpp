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
{
}

void CEnemy::ThrowEnemy() {
	//エネミーを死亡させる時間まで投げられたら
	if (m_ThrowTimeCo >= ThrowEnemyDeadTime) {
		//死亡させる
		m_State = enState::Dead;
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
	AttackHit = true;

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
}
