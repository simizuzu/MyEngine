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

enum MouseButton
{
	LEFT,
	RIGHT,
	CENTER
};


class Mouse
{
private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouseDev;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 oldMouseState = {};

	MyMath::Vector2 mousePos;
	MyMath::Vector2 worldMousePos;
	MyMath::Vector3 mouseMove;

public:

	void Initialize(IDirectInput8* dinput);

	void Update();

	bool MouseButtonTrigger(MouseButton button);

	bool MouseButtonOffTrigger(MouseButton button);


	bool MouseButtonInput(MouseButton button);

	const MyMath::Vector2 GetMousePos()const;

	const MyMath::Vector2 GetWorldMousePos()const;

	const MyMath::Vector3 GetMouseMove()const;
};

