#include "Keyboard.h"

void Keyboard::Initialize(IDirectInput8* directInput)
{
	HRESULT result;
	WinApp* winApp = WinApp::GetInstance();

	//キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Keyboard::Update()
{
	// 全キーの入力状態を取得する

	for (int i = 0; i < _countof(oldkey); i++)
	{
		oldkey[i] = key[i];
	}

	//キーボード情報の取得開始
	keyboard->Acquire();
	keyboard->GetDeviceState(sizeof(key), key);
}


bool Keyboard::PushKey(uint32_t keyNum) { // 押した状態
	return (bool)key[keyNum];
}

bool Keyboard::ReleaseKey(uint32_t keyNum) { // 離した状態
	return !key[keyNum] && !oldkey[keyNum];
}

bool Keyboard::TriggerPushKey(uint32_t keyNum) { // 押した瞬間
	return key[keyNum] && !oldkey[keyNum];
}

bool Keyboard::TriggerReleaseKey(uint32_t keyNum) { // 離した瞬間
	return !key[keyNum] && oldkey[keyNum];
}
