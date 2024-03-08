#pragma once
#include "SuppressWarning.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
MYENGINE_SUPPRESS_WARNINGS_END

#include "WinApp.h"
#include "DirectX12Math.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

/**
 * @class Mouse.h
 * @brief マウスを設定するクラス
 */

/// <summary>
/// マウス用enum
/// </summary>
enum MouseButton
{
	LEFT,
	RIGHT,
	CENTER
};

/// <summary>
/// マウス
/// </summary>
class Mouse
{
private:
	// マウスのデバイス
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouseDev;

	//マウスの状態
	DIMOUSESTATE2 mouseState = {};
	//オールドマウスの状態
	DIMOUSESTATE2 oldMouseState = {};

	//マウスの座標
	MyMath::Vector2 mousePos;
	//マウスのワールド座標
	MyMath::Vector2 worldMousePos;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dinput"></param>
	void Initialize(IDirectInput8* dinput);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// マウスのトリガーボタン
	/// </summary>
	/// <param name="button">LEFT,RIGHT,CENTER</param>
	/// <returns>押されているか</returns>
	bool MouseButtonTrigger(MouseButton button);

	/// <summary>
	/// マウスの押されていない状態
	/// </summary>
	/// <param name="button">LEFT,RIGHT,CENTER</param>
	/// <returns>押されていない</returns>
	bool MouseButtonOffTrigger(MouseButton button);

	/// <summary>
	/// マウスのインプット(あまり使わない)
	/// </summary>
	/// <param name="button">LEFT,RIGHT,CENTER</param>
	/// <returns>押されているか</returns>
	bool MouseButtonInput(MouseButton button);

	//getter
	const MyMath::Vector2 GetMousePos()const;
	const MyMath::Vector2 GetWorldMousePos()const;
	const MyMath::Vector3 GetMouseMove()const;
};

