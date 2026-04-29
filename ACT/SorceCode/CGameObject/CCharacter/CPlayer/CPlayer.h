#pragma once
#include "CGameObject/CCharacter/CCharacter.h"
#include "CGameObject/CCharacter/CPlayer/CHeart/CHeart.h"

#include "CWire/CWire.h"
#include "CNormalAttack/CNormalAttack.h"
/*****************************************************************************************************
*		プレイヤークラス
*/

class CPlayer
	:public CCharacter
{
public:
	//最大落下速度
	static constexpr double MAX_FALLING_SPEED = 25;//落下速度の最大値
	static constexpr double JUMP_POWER = 10;//ジャンプ力
	static constexpr int DashcountMAX = 30;//ダッシュの受付時間
	static constexpr int TurnAroundSpeed = 10;//向きを変える速さ
	static constexpr double AvoidanceDistance = 300;//回避の距離
	static constexpr int AvoidanceTime = 10;//回避にかかる時間
	static constexpr int AvoidancecoolTime = 30;//回避のクールタイム
	static constexpr double PlayerGrobtyi = 0.4;//プレイヤーの重力
	static constexpr double ImageSize = 64;
	static constexpr  double AirPower = 0.1;//空中移動力
	static constexpr  int AirSpeedMAX = 15;//空中最大速度
	static constexpr int	AnimationSpeed = 5;//何フレームに一回動くか
	static constexpr  double	NoSpeed = 10;//ノーマルスピード
	static constexpr  double	cathiSpeed = 5;//物持ち中のスピード
	static constexpr int	AvoidanceCan = 3;//空中回避回数


	static constexpr int m_HitBackCoMAX = 30;//ヒットバック時間
	static constexpr int NoHitAttackTime = 120;	//攻撃があたらない時間
	static constexpr int m_HitBackCoPware = 25;//ノックバックパワー
	bool EVENT_HIT;		//イベントブロックに当たったら
	VECTOR2_f EVENT_START_POS;		//イベントが始まったときの位置

	bool DAMAGE_KEDAMA_HIT;			//ダメージ毛玉に当たったら
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
	void Turnaround(VECTOR2_f Pos);

	void DrawH(HDC c, HWND h, std::unique_ptr<CCamera>& pCamera);//後で消す
	CPlayer();
	~CPlayer();
	//ワイヤーポイントを掴む状態にする
	void StartWirePointCatch();
	void StartSetting() override;

	int GetStete() { return m_State; }
	void Attackmove();
	void Update() override {};
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
	bool GetWireShot() { return m_WireShot; }
	void ShotWire() { enActionState = enActionState::WireShot; }
	void SetPosition(VECTOR2_f pos) { m_Position = pos; }
	void WireEnd(VECTOR2_f Spead);
	void WireEndEnemi();
	double GetWireStartSpeed();
	void SetCamera(CCamera* m_pCamer) {
		m_pCamera = m_pCamer;
	}

	//それぞれ個別に渡す
	void SetPos_X(double Pos_X) { m_Position.x = Pos_X; }
	void SetPos_Y(double Pos_Y) { m_Position.y = Pos_Y; }

	void Update(std::vector<std::unique_ptr<CBullet>>& upBullet) override;
	//ムービーシーンのアップデート
	void MovieSceneUpdate();

	//プレイヤーのハートを描画する
	void PlayerHeartDraw() { m_upHeart->Draw(); }

	void StaratEnemiWire();
	void EndEnemiWire();
private:
	void Animation() override;

	//ステージとの当たり判定
	void StageCollision(double OffsetPos_X, double OffsetPos_Y) override;
public://パブリック
	//エネミーに触れたなら
	void EnemyHit(VECTOR2_f Pos, int Damage);

	//バレットに触れたなら
	void BulletHit(VECTOR2_f Pos, int Color, int Damage, bool NazrinBullet);

	//カメラの当たり判定(ボスバトルなどの)
	void CameraCollision(VECTOR2_f CameraPos, double OffsetPos_X, double OffsetPos_Y);

	bool MoveSafeWrier(VECTOR2_f pos);
	void SetWireTopPos(VECTOR2_f TopPos);

	void PlayerDamegEriaHit();
	void PlayerMyHit(VECTOR2_f Pos);
public:
	std::unique_ptr<CNormalAttack> &GetNormalAttack_p() { return NormalAttack; }
	int GetAlpha() { return m_Alpha; }
	bool GetMyHit() {if (m_HitBackBack|| m_HitBack) {return true;}return false;}
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
	//プレイヤーの属性変更
	void PlayerColorChange();

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
	int AvoidanceCanCount;//回避の残り回数
	VECTOR2_f AirAvoidanceVECT;	//空中回避のベクトル

	bool m_ChangeColor;		//属性を変更したか

	std::unique_ptr<CHeart> m_upHeart;	//プレイヤーのハート(体力)クラス

	std::unique_ptr<CNormalAttack> NormalAttack;
	CCamera* m_pCamera;
	int m_AnimationCount;
	int m_OldSteta;
	int OldenActionState;
	double m_MoveSpeed;	//移動速度を入れる

	VECTOR2_f WireTopPos;
	


	bool m_HitBack;			//攻撃を受けたときのヒットバック中かを見る
	int m_HitBackCo;		//ヒットバック中のカウント

	bool m_HitBackBack;
	int m_HitBackBackCount;

	VECTOR2_f m_HitBackSpeed;	//ヒットバックする速度

	int NoHitAttackCo = 0;	//攻撃があたらない時間をカウント

};