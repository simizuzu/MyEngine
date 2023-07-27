#pragma once
#include <memory>
#include "Keyboard.h"
#include "Controller.h"
//
//#include <wrl.h>
//#include <cassert>

using namespace DirectX;

class Input
{
private:
	// テンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<IDirectInput8> directInput;
	Keyboard* keyboard_ = nullptr;
	Controller* controller_ = nullptr;

public:
	void Initialize();
	void Update();

	unsigned char GetKeyTime(int keyName);
	bool PushKey(BYTE keyNum);
	bool ReleaseKey(BYTE keyNum);
	bool TriggerPushKey(BYTE keyNum);
	bool TriggerReleaseKey(BYTE keyNum);

	bool TriggerButton(ControllerButton button);
	bool TriggerStick(ControllerStick stickInput, const float& deadZoneRange = 0.3f, const MyMath::Vector2& deadZoneInitial = { 1.0f,1.0f });
	bool PushButton(ControllerButton button);
	bool InputStick(ControllerStick stickInput, const float& deadZoneRange = 0.3f, const MyMath::Vector2& deadZoneInitial = { 1.0f,1.0f });
	bool ReleaseTriggerButton(ControllerButton button);
	bool ReleaseTriggerStick(ControllerStick stickInput, const float& deadZoneRange = 0.3f, const MyMath::Vector2& deadZoneInitial = { 1.0f,1.0f });
	void ShakeController(const float& power, const int& flame);
	MyMath::Vector2 GetLeftStickVec(const MyMath::Vector2& deadZoneRange = { 1.0f,1.0f });
	MyMath::Vector2 GetRightStickVec(const MyMath::Vector2& deadZoneRange = { 1.0f,1.0f });

	static Input* GetInstance();
private:
	BYTE oldkey[256] = {};
	BYTE key[256] = {};

	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;
};

