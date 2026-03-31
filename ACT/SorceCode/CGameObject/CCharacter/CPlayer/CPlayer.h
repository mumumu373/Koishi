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
	static constexpr double MAX_FALLING_SPEAD = 25;//落下速度の最大値
	static constexpr double JUMP_POWER = 12;//ジャンプ力
	static constexpr int DashcountMAX = 30;//ダッシュの受付時間
	static constexpr int TurnAroundSpeed = 10;//向きを変える速さ
	static constexpr double AvoidanceDistance = 144;//回避の距離
	static constexpr int AvoidanceTime = 6;//回避にかかる時間
	static constexpr int AvoidancecoolTime = 90;//回避のクールタイム
public:
	//アクション状態
	enum enActionState
	{
		None,			//何の状態でもない
		Avoidance,		//回避
		AirAvoidance,	//空中回避
		Guard,			//ガード
		Attack,			//攻撃
		Damage,			//ダメージ
		WireShot,		//ワイヤー発射
		WirePointCatch,	//ワイヤーポイントを掴んだ
		WireObjectCatch,//ワイヤーで物を掴んだ(敵やオブジェクト)
	};

	int enActionState = enActionState::None;
public:
	bool GroundStand = false;		//地面に立っています
	bool OldGroundStand = false;		//前フレームの地面に立っている状態
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
	void WireShotStato(bool tyu) {
		if (tyu) {
			enActionState = enActionState::WireShot;
		}
		else if (enActionState == enActionState::WireShot) {
			enActionState = enActionState::None;
		};
	}
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
	void AvoidanceEnd();
	void KyeInput();
	void AirKeyInput();
	//プレイヤーの動きの制御
	void MovePlayer();
	void MovePlayerJump();
	void MovePlayerGround();
	//プレイヤーのジャンプ制御
	void JumpPlayer();

	void Dash();
private:
	void AirAvoidanceVECTSet();

	bool m_Jumping;			//ジャンプしてます！
	const double m_JumpPower;		//ジャンプ力
	double m_JumpAcc;		//ジャンプ力を加算させる
	bool m_JumpRemove;		//ジャンプボタンを離したぞ！
	int m_JumpRemoveCo;		//ジャンプボタンを押した時間を図る
	bool m_WireShot;		//ワイヤーを撃つぞ！
	bool m_WireShotCan;		//ワイヤーを撃てるか
	VECTOR2_f m_Acceleration;	//ワイヤーから脱出するスピード(プレイヤーの加速度)
	bool m_leftkey[2];	//左キーの入力を記録する配列
	bool m_rightkey[2];	//右キーの入力を記録する配列
	int m_Ldashcount;	
	int m_Rdashcount;
	bool m_Ldash;		//左ダッシュ中
	bool m_Rdash;		//右ダッシュ中
	int AvoidanceCount;	//回避にかかる時間を図る
	int AvoidanceCoolCount;//回避のクールタイムを図る
	VECTOR2_f AirAvoidanceVECT;	//空中回避のベクトル
};