#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END
#include "Keyboard.h"
#include "Controller.h"
#include "Mouse.h"

/**
 * @class Input.h
 * @brief 操作関連をまとめたクラス
 */

/// <summary>
/// インプット
/// </summary>
class Input
{
private:

	Microsoft::WRL::ComPtr<IDirectInput8> directInput;
	Keyboard* keyboard_ = nullptr;
	Controller* controller_ = nullptr;
	Mouse* mouse_ = nullptr;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:
	//キーボード
	/// <summary>
	/// どれだけ押しているか
	/// </summary>
	unsigned char GetKeyTime(BYTE keyName);

	/// <summary>
	/// 押し続けているか
	/// </summary>
	bool PushKey(BYTE keyNum);

	/// <summary>
	/// 離しているか
	/// </summary>
	bool ReleaseKey(BYTE keyNum);

	/// <summary>
	/// 一瞬クリックしたか
	/// </summary>
	bool TriggerPushKey(BYTE keyNum);

	/// <summary>
	/// 一瞬離したか
	/// </summary>
	bool TriggerReleaseKey(BYTE keyNum);


	//コントローラー
	/// <summary>
	/// 一瞬押したか
	/// </summary>
	bool TriggerButton(ControllerButton button);

	/// <summary>
	/// 一瞬倒したか
	/// </summary>
	bool TriggerStick(ControllerStick stickInput, const float& deadZoneRange = 0.3f, const MyMath::Vector2& deadZoneInitial = { 1.0f,1.0f });

	/// <summary>
	/// 押し続けているか
	/// </summary>
	bool PushButton(ControllerButton button);

	/// <summary>
	/// 倒し続けているか
	/// </summary>
	bool InputStick(ControllerStick stickInput, const float& deadZoneRange = 0.3f, const MyMath::Vector2& deadZoneInitial = { 1.0f,1.0f });

	/// <summary>
	/// 一瞬離したか
	/// </summary>
	bool ReleaseTriggerButton(ControllerButton button);

	/// <summary>
	/// 一瞬離したか
	/// </summary>
	bool ReleaseTriggerStick(ControllerStick stickInput, const float& deadZoneRange = 0.3f, const MyMath::Vector2& deadZoneInitial = { 1.0f,1.0f });

	/// <summary>
	/// コントローラーを震わす
	/// </summary>
	/// <param name="power">パワー</param>
	/// <param name="flame">時間</param>
	void ShakeController(const float& power, const int& flame);

	/// <summary>
	/// 左スティックのベクトル
	/// </summary>
	MyMath::Vector2 GetLeftStickVec(const MyMath::Vector2& deadZoneRange = { 1.0f,1.0f });

	/// <summary>
	/// 右スティックのベクトル
	/// </summary>
	MyMath::Vector2 GetRightStickVec(const MyMath::Vector2& deadZoneRange = { 1.0f,1.0f });


	//マウス
	/// <summary>
	/// マウスボタンの入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したかどうか</returns>
	bool MouseButtonTrigger(MouseButton button);

	/// <summary>
	/// マウスボタンの離した瞬間
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>離したか</returns>
	bool MouseButtonOffTrigger(MouseButton button);

	/// <summary>
	/// マウスボタンの入力
	/// </summary>
	/// <param name="button">チェックしたいボタン</param>
	/// <returns>押したか</returns>
	bool MouseButtonInput(MouseButton button);

	/// <summary>
	/// マウスの位置を取得
	/// </summary>
	/// <returns>マウスの位置</returns>
	const MyMath::Vector2 GetMousePos()const;

	/// <summary>
	/// マウスの位置を取得
	/// </summary>
	/// <returns>マウスの位置</returns>
	const MyMath::Vector2 GetWorldMousePos()const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const MyMath::Vector3 GetMouseMove()const;

	//シングルトン
	static Input* GetInstance();
private:
	BYTE oldkey[256] = {};
	BYTE key[256] = {};

	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;
};

