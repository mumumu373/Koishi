#pragma once
#include "CGameObject/CCharacter/CCharacter.h"

/*****************************************************************************************************
*		プレイヤークラス
*/
class CPlayer
	:public CCharacter
{
public:
	//最大落下速度
	static constexpr double MAX_FALLING_SPEAD = 25;
public:
	//アクション状態
	enum enActionState
	{
		None,			//何の状態でもない
		Attack,			//攻撃
		Damage,			//ダメージ
		WireShot,		//ワイヤー発射
		WirePointCatch,	//ワイヤーポイントを掴んだ
		WireObjectCatch,//ワイヤーで物を掴んだ(敵やオブジェクト)
	};

	int enActionState = enActionState::None;
public:
	bool GroundStand = false;		//地面に立っています
public:
	CPlayer();
	~CPlayer();

	void StartSetting() override;

	void Update() override {};
	void Draw(std::unique_ptr<CCamera>& pCamera) override;

	void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override;
private:
	void Animation() override;
public://パブリック
	//エネミーに触れたなら
	void EnemyHit(int Enemy, int Color);

private:

	void KyeInput();
	//プレイヤーの動きの制御
	void MovePlayer();
	//プレイヤーのジャンプ制御
	void JumpPlayer();
private:
	bool m_Jumping;			//ジャンプしてます！
	const double m_JumpPower;		//ジャンプ力
	double m_JumpAcc;		//ジャンプ力を加算させる
	bool m_JumpRemove;		//ジャンプボタンを離したぞ！
	int m_JumpRemoveCo;		//ジャンプボタンを押した時間を図る
};