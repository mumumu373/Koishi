#pragma once
#include "CGameObject/CCharacter/CCharacter.h"

#include "CWire/CWire.h"
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
	//ワイヤーポイントを掴む状態にする
	void StartWirePointCatch();
	void StartSetting() override;

	int GetStete() { return m_State; }

	void Update() override;
	void Draw(std::unique_ptr<CCamera>& pCamera) override;
	//ワイヤーを撃てるかセット
	void SetWireShotCan(bool can) { m_WireShotCan = can; };
	//trueならワイヤーを撃てる
	bool GetWireShot() { return m_WireShot; };
	void ShotWire() { enActionState = enActionState::WireShot; };
	void SetPosition(VECTOR2_f pos) { m_Position = pos; };
	void WireEnd(VECTOR2_f Spead);
	double GetWireStartSpeed();
private:
	void Animation() override;
public://パブリック
	//エネミーに触れたなら
	void EnemyHit(int Enemy, int Color);

private:

	void KyeInput();
	//プレイヤーの動きの制御
	void MovePlayer();
	void MovePlayerWireOutSped();
	//プレイヤーのジャンプ制御
	void JumpPlayer();
private:
	bool m_Jumping;			//ジャンプしてます！
	const double m_JumpPower;		//ジャンプ力
	double m_JumpAcc;		//ジャンプ力を加算させる
	bool m_JumpRemove;		//ジャンプボタンを離したぞ！
	int m_JumpRemoveCo;		//ジャンプボタンを押した時間を図る
	bool m_WireShot;		//ワイヤーを撃つぞ！
	bool m_WireShotCan;		//ワイヤーを撃てるか
	VECTOR2_f m_WireOutSped;	//ワイヤーから脱出するスピード
};