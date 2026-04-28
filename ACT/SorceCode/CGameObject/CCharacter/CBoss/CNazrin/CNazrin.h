#pragma once
#include "CGameObject/CCharacter/CBoss/CBoss.h"	//ボスクラス(継承)
#include "CGame/CBulletFactory/CBulletFactory.h"//バレットを作るクラス

/**********************************************************************************************************
*		ナズーリン(1面ボス)
*/
class CNazrin
	:public CBoss
{
public:
	bool NextSetPosBlock = false;		//次の所定位置まで動く

	const int Phase2_HP_Decrease = 50;	//フェーズ2の時に減らせるHPを設定

	bool GetingApple = false;			//倒した時のリンゴがとれるようにする設定
public:
	CNazrin();
	~CNazrin();

	void StartSetting() override;
	void Update() override {};
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override;

	//プレイヤーの攻撃がヒットした時の動作
	void PlayerAttackHit(int Damage) override;

	//ボスとの戦闘フラグを踏んだ
	void BossBattleFlag(VECTOR2_f SetPos) override;

	//会話などのムービーシーンの時に動作する関数
	void MovieSceneUpdate() override;
private:
	void Animation() override;

	//ステージとの当たり判定
	void StageCollision(double OffsetPos_X, double OffsetPos_Y) override;

	//ボスムーブ1のアップデート
	void BossMove_1Update(int BossPhase, std::vector<std::unique_ptr<CBullet>>& upBullet);
	//ボスムーブ2のアップデート
	void BossMove_2Update(int BossPhase, std::vector<std::unique_ptr<CBullet>>& upBullet);

	//ボスのムーブを戻す動作
	void RetuanMoveSet();
private:
	bool m_Jumping;			//今ジャンプしているか
	int m_JumpingCo;		//ジャンプするタイミングをカウント
	double m_JumpPower;		//ジャンプ力

	bool m_BulletShot;		//バレットを撃つ
	int m_BulletShotCo;		//撃つまでのカウント
	int m_HowShotBullet;	//バレットを何発撃った？

	double m_FallingSpeed;	//落下速度
	bool m_GroundStand;		//地面に立っているか	

	int m_AttackMoveChangeCo;	//攻撃のパターンを変えるタイミングをカウント
	int m_AttackMoveCo;			//攻撃の動作をカウントする

	VECTOR2_f m_MemoryPos;		//ポジションを記憶しておく
	VECTOR2_f m_BattleStartPos;	//ボスバトルのスタート位置

	int m_PhaseChangeCo;		//フェーズを変えるタイミングをカウントする
	bool m_NextPhaseSetting;	//フェーズを変える準備

	bool m_ReturnMove;			//ムーブを戻す
	int m_DecreaseHP;			//減ったHPを数える

	VECTOR2_f m_Vector;			//ベクトル

	int m_GetAppleCo;			//リンゴが取れるようになるまでの時間
private:
	int m_AttackAnimCo;			//攻撃アニメーションのカウント
	bool m_AttackAnimTime;		//攻撃アニメーション中

	bool m_PhaseChangeAniTime;	//フェーズチェンジ中のアニメーション
};