#pragma once
#include <Windows.h>
#include "MyMath.h"
#include "Global.h"
#include <CGame/CCollisionDetection/CCollisionDetection.h>
class CCollisionDetection;

class CMouseInput
{
private:
	int Size;//ゲーム中に変える可能性のある為コンストではない
	static constexpr int StertSize=64;//ゲーム開始時のサイズ
	static constexpr int IMGSize = 40;
public:
	
	~CMouseInput();

	//マウスの入力を記録
	static void Update() { GetInstance()->update(); };
	
	//マウスの左を取得,Pushは押したか離しているか Momentは押し続けているか押した瞬間か（trueは押した瞬間）
	static bool GetMouseLeft(bool Push,bool Moment) { return GetInstance()->getMouseLeft(Push,Moment); };


	//マウスの右を取得,Pushは押したか離しているか Momentは押し続けているか押した瞬間か（trueは押した瞬間）
	static bool GetMouseRight(bool Push, bool Moment) { return GetInstance()->getMouseRight(Push, Moment); };


	//マウスの真ん中の押込みを取得,Pushは押したか離しているか Momentは押し続けているか押した瞬間か（trueは押した瞬間）
	static bool GetMouseWheel(bool Push, bool Moment) { return GetInstance()->getMouseWheel(Push, Moment); };//マウスホイール
	

	//マウス画像描画
	static void Draw() { GetInstance()->draw(); };

	//マウスの当たり判定を取得
	static CCollisionDetection::ObjectInfo  GetcollisionMouse() {
		CCollisionDetection::ObjectInfo i;
			i.x = GetInstance()->MousePos.x - GetInstance()->Size / 2;
		i.y = GetInstance()->MousePos.y - GetInstance()->Size / 2;
		i.xw= GetInstance()->Size;
		i.yh = GetInstance()->Size;
		return i;
	}
	static int GetcollisionMouseSize() {
		return GetInstance()->Size;
	}
	static void ColorChange() {
		GetInstance()->MouseColorChange = true;
	}
	static CMouseInput* GetInstance()
	{
		//唯一のインスタンスを作成する
		//※staticで作成されたので２回目以降は、下の１行は無視される
		static CMouseInput* s_Instance=new CMouseInput();	//s_:static

		return s_Instance;
	};
	//初期設定
	static void InitialSettings(HWND hWnd);

	//マウスの座標を取得
	static VECTOR2_f GetMousePos();
	static VECTOR2_f GetMousePosCamera(std::unique_ptr<CCamera>& m_pCamera) {
		return m_pCamera->CalcToPositionInStage(&GetInstance()->MousePos);
	}
	//
	static void SetMousePos(VECTOR2_f pos) {
		GetInstance()->MousePos = pos;
	}
private:

	//そのまま関数を呼び出すと全てにゲットインスタンスを描く必要があるので中継する
	void update();
	bool getMouseLeft(bool Push, bool Moment);
	bool getMouseRight(bool Push, bool Moment);
	bool getMouseWheel(bool Push, bool Moment);
	void draw();

	//外部からコンストラクタへのアクセスを禁止する
	CMouseInput();

	//コピーコンストラクタによるコピーを禁止する
	//=deleteで関数の定義を削除できる
	CMouseInput(const CMouseInput& rhs) = delete;

	//代入演算子によるコピーを禁止する
	//operator(オペレーター);演算子のオーバーロードで演算の中身を拡張できる
	CMouseInput& operator=(CMouseInput& rhs) = delete;

	bool MouseColorChange;
	 HWND m_hWnd;
	 bool MouseLeft[2] ;
	 bool MouseRight[2];
	 bool MouseWheel[2];
	 VECTOR2_f MousePos;
};

