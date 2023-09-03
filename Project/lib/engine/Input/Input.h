#pragma once
#include <memory>
#include "Keyboard.h"
#include "Controller.h"
#include "Mouse.h"

using namespace DirectX;

class Input
{
private:
	// テンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<IDirectInput8> directInput;
	Keyboard* keyboard_ = nullptr;
	Controller* controller_ = nullptr;
	Mouse* mouse_ = nullptr;

public:
	void Initialize();
	void Update();

	//キーボード
	unsigned char GetKeyTime(BYTE keyName);
	bool PushKey(BYTE keyNum);
	bool ReleaseKey(BYTE keyNum);
	bool TriggerPushKey(BYTE keyNum);
	bool TriggerReleaseKey(BYTE keyNum);

	//コントローラー
	bool TriggerButton(ControllerButton button);
	bool TriggerStick(ControllerStick stickInput, const float& deadZoneRange = 0.3f, const MyMath::Vector2& deadZoneInitial = { 1.0f,1.0f });
	bool PushButton(ControllerButton button);
	bool InputStick(ControllerStick stickInput, const float& deadZoneRange = 0.3f, const MyMath::Vector2& deadZoneInitial = { 1.0f,1.0f });
	bool ReleaseTriggerButton(ControllerButton button);
	bool ReleaseTriggerStick(ControllerStick stickInput, const float& deadZoneRange = 0.3f, const MyMath::Vector2& deadZoneInitial = { 1.0f,1.0f });
	void ShakeController(const float& power, const int& flame);
	MyMath::Vector2 GetLeftStickVec(const MyMath::Vector2& deadZoneRange = { 1.0f,1.0f });
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


	static Input* GetInstance();
private:
	BYTE oldkey[256] = {};
	BYTE key[256] = {};

	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;
};

