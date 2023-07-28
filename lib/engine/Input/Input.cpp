#include "Input.h"

void Input::Initialize()
{
	HRESULT result;
	WinApp* winApp = WinApp::GetInstance();

	//DirectInputの初期化
	result = DirectInput8Create(winApp->GetHinstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	// キーボード初期化
	keyboard_ = new Keyboard();
	keyboard_->Initialize(directInput.Get());

	// コントローラー初期化
	controller_ = new Controller();
}

void Input::Update() 
{
	// キーボード処理
	keyboard_->Update();
	// コントローラー処理
	controller_->Update();
}

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

#pragma region キーボード
unsigned char Input::GetKeyTime(BYTE keyName)
{
	if (PushKey(keyName))
	{
		key[keyName]++;
		if (key[keyName] >= 255)
		{
			key[keyName] = 254;
		}
	}
	else
	{
		key[keyName] = 0;
	}
	return key[keyName];
}

bool Input::PushKey(BYTE keyNum) { // 押した状態
	return keyboard_->PushKey(keyNum);
}

bool Input::ReleaseKey(BYTE keyNum) { // 離した状態
	return keyboard_->ReleaseKey(keyNum);
}

bool Input::TriggerPushKey(BYTE keyNum) { // 押した瞬間
	return keyboard_->TriggerPushKey(keyNum);
}

bool Input::TriggerReleaseKey(BYTE keyNum) { // 離した瞬間
	return keyboard_->TriggerReleaseKey(keyNum);
}
#pragma endregion

#pragma region コントローラー
bool Input::TriggerButton(ControllerButton button)
{
	return controller_->TriggerButton(button);
}
bool Input::TriggerStick(ControllerStick stickInput, const float& deadZoneRange, const MyMath::Vector2& deadZoneInitial)
{
	return controller_->TriggerStick(stickInput,deadZoneRange,deadZoneInitial);
}
bool Input::PushButton(ControllerButton button)
{
	return controller_->PushButton(button);
}
bool Input::InputStick(ControllerStick stickInput, const float& deadZoneRange, const MyMath::Vector2& deadZoneInitial)
{
	return controller_->InputStick(stickInput, deadZoneRange, deadZoneInitial);
}
bool Input::ReleaseTriggerButton(ControllerButton button)
{
	return controller_->ReleaseTriggerButton(button);
}
bool Input::ReleaseTriggerStick(ControllerStick stickInput, const float& deadZoneRange, const MyMath::Vector2& deadZoneInitial)
{
	return controller_->ReleaseTriggerStick(stickInput, deadZoneRange, deadZoneInitial);
}
void Input::ShakeController(const float& power, const int& flame)
{
	controller_->ShakeController(power, flame);
}
MyMath::Vector2 Input::GetLeftStickVec(const MyMath::Vector2& deadZoneRange)
{
	return controller_->GetLeftStickVec(deadZoneRange);
}
MyMath::Vector2 Input::GetRightStickVec(const MyMath::Vector2& deadZoneRange)
{
	return controller_->GetRightStickVec(deadZoneRange);
}
#pragma endregion
