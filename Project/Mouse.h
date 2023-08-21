#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInputのバージョン指定
#include <dinput.h>
#include <wrl.h>
#include "WinApp.h"
#include "DirectX12Math.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

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
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouseDev;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 oldMouseState = {};
	//マウスのゲーム空間内でのレイ
	MyMath::Vector2 mousePos;
	MyMath::Vector2 worldMousePos;
	MyMath::Vector3 mouseMove;

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
	/// マウスの位置
	/// </summary>
	/// <returnsマウスの位置></returns>
	const MyMath::Vector2 GetMousePos()const;

	/// <summary>
	/// マウスの位置
	/// </summary>
	/// <returnsマウスの位置></returns>
	const MyMath::Vector2 GetWorldMousePos()const;

	/// <summary>
	/// マウスの位置
	/// </summary>
	/// <returnsマウスの位置></returns>
	const MyMath::Vector3 GetMouseMove()const;
};

