#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include <DirectXMath.h>
#include <array>
#include <wrl.h>
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

#include "WinApp.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

/**
 * @class Keyboard.h
 * @brief キーボードを設定するクラス
 */

class Keyboard
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directInput">:DirectInputオブジェクト</param>
	void Initialize(IDirectInput8* directInput);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーを押した状態の処理
	/// </summary>
	/// <param name="keyNum">:キーナンバー</param>
	/// <returns>キーナンバー</returns>
	bool PushKey(uint32_t keyNum);
	/// <summary>
	/// キーを離した状態の処理
	/// </summary>
	/// <param name="keyNum">:キーナンバー</param>
	/// <returns>キーナンバー</returns>
	bool ReleaseKey(uint32_t keyNum);
	/// <summary>
	/// キーを押した瞬間の処理
	/// </summary>
	/// <param name="keyNum">:キーナンバー</param>
	/// <returns>キーナンバー</returns>
	bool TriggerPushKey(uint32_t keyNum);
	/// <summary>
	/// キーを離した瞬間の処理
	/// </summary>
	/// <param name="keyNum">:キーナンバー</param>
	/// <returns>キーナンバー</returns>
	bool TriggerReleaseKey(uint32_t keyNum);

private:

	WinApp* winApp_ = nullptr;

	Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard;

	BYTE oldkey[256] = {};
	BYTE key[256] = {};
};

