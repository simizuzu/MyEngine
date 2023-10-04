#pragma once
#include "SuppressWarning.h"

#include "Vector2.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <Windows.h>
#include <Xinput.h>
MYENGINE_SUPPRESS_WARNINGS_END

#pragma comment (lib, "xinput.lib")

enum ControllerButton
{
	B = XINPUT_GAMEPAD_B,
	A = XINPUT_GAMEPAD_A,
	X = XINPUT_GAMEPAD_X,
	Y = XINPUT_GAMEPAD_Y,
	START = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK,
	LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	LT,
	RT
};

enum ControllerStick
{
	L_UP, L_DOWN, L_LEFT, L_RIGHT,
	R_UP, R_DOWN, R_LEFT, R_RIGHT, XBOX_STICK_NUM
};

class Controller
{
private:
	XINPUT_STATE xinputState{};
	XINPUT_STATE oldXinputState{};

	// コントローラー振動強さ
	float shakePower = 0.0f;
	// コントローラー振動長さ(フレーム数)
	int shakeTimer = 0;

	// デッドゾーンに入っているか(DeadRate : デッドゾーン判定の度合い)
	bool StickDeadZone(MyMath::Vector2& stick, const MyMath::Vector2& deadZoneInitial);

	// 最高入力値
	const float MAX_STICK_NUM = 32768.0f;

public:
	Controller() = default;
	~Controller() = default;

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// トリガーボタンの入力
	/// </summary>
	/// <param name="button">チェックするボタン</param>
	/// <returns></returns>
	bool TriggerButton(ControllerButton button);

	/// <summary>
	/// トリガースティックの入力
	/// </summary>
	/// <param name="stickInput">スティックの方向</param>
	/// <param name="deadZoneRange">デッドゾーンの範囲</param>
	/// <param name="deadZoneInitial">デッドゾーンの判定の初期値: 1.0f</param>
	/// <returns></returns>
	bool TriggerStick(ControllerStick stickInput, const float& deadZoneRange = 0.3f, const MyMath::Vector2& deadZoneInitial = { 1.0f,1.0f });

	/// <summary>
	/// プッシュボタンの入力
	/// </summary>
	/// <param name="button">チェックするボタン</param>
	/// <returns></returns>
	bool PushButton(ControllerButton button);

	/// <summary>
	/// スティックの入力
	/// </summary>
	/// <param name="stickInput">スティックの方向</param>
	/// <param name="deadZoneRange">デッドゾーンの範囲</param>
	/// <param name="deadZoneInitial">デッドゾーンの判定の初期値: 1.0f</param>
	/// <returns></returns>
	bool InputStick(ControllerStick stickInput, const float& deadZoneRange = 0.3f, const MyMath::Vector2& deadZoneInitial = { 1.0f,1.0f });

	/// <summary>
	/// ボタンを離したときの入力
	/// </summary>
	/// <param name="button">チェックするボタン</param>
	/// <returns></returns>
	bool ReleaseTriggerButton(ControllerButton button);

	/// <summary>
	/// スティックの離したときの入力
	/// </summary>
	/// <param name="stickInput">スティックの方向</param>
	/// <param name="deadZoneRange">デッドゾーンの範囲</param>
	/// <param name="deadZoneInitial">デッドゾーンの判定の初期値: 1.0f</param>
	/// <returns></returns>
	bool ReleaseTriggerStick(ControllerStick stickInput, const float& deadZoneRange = 0.3f, const MyMath::Vector2& deadZoneInitial = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーの振動させる処理
	/// </summary>
	/// <param name="power">振動の強さ: 0.0f~1.0f</param>
	/// <param name="flame">振動させる時間(フレーム数)</param>
	void ShakeController(const float& power, const int& flame);

	/// <summary>
	/// 左スティックのベクトル
	/// </summary>
	/// <param name="deadZoneRange">デッドゾーンの判定の初期値: 1.0f</param>
	/// <returns></returns>
	MyMath::Vector2 GetLeftStickVec(const MyMath::Vector2& deadZoneRange = { 1.0f,1.0f });

	/// <summary>
	/// 右スティックのベクトル
	/// </summary>
	/// <param name="deadZoneRange">デッドゾーンの判定の初期値: 1.0f</param>
	/// <returns></returns>
	MyMath::Vector2 GetRightStickVec(const MyMath::Vector2& deadZoneRange = { 1.0f,1.0f });

private:
	//コピーコンストラクタ・代入演算子削除
	Controller& operator=(const Controller&) = delete;
	Controller(const Controller&) = delete;
};

