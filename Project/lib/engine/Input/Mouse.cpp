#include "Mouse.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

void Mouse::Initialize(IDirectInput8* dinput)
{
	HRESULT result;


	result = dinput->CreateDevice(GUID_SysMouse, &mouseDev, NULL);
	assert(SUCCEEDED(result));

	result = mouseDev->SetDataFormat(&c_dfDIMouse2);
	assert(SUCCEEDED(result));

	result = mouseDev->SetCooperativeLevel(WinApp::GetInstance()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void Mouse::Update()
{
	HRESULT result;


	result = mouseDev->Acquire();
	oldMouseState = mouseState;
	result = mouseDev->GetDeviceState(sizeof(mouseState), &mouseState);

	float width = static_cast<float>(WinApp::GetInstance()->window_width);
	float height = static_cast<float>(WinApp::GetInstance()->window_height);

	POINT point;
	LPPOINT p = &point;

	GetCursorPos(p);

	ScreenToClient(WinApp::GetInstance()->GetHwnd(), p);

	worldMousePos.x = static_cast<float>(p->x);
	mousePos.x = MyMathUtility::Clamp(worldMousePos.x, 0.0f, width);
	worldMousePos.y = static_cast<float>(p->y);
	mousePos.y = MyMathUtility::Clamp(worldMousePos.y, 0.0f, height);
}

bool Mouse::MouseButtonTrigger(MouseButton button)
{
	return (!oldMouseState.rgbButtons[button] && mouseState.rgbButtons[button]);
}

bool Mouse::MouseButtonOffTrigger(MouseButton button)
{
	return (oldMouseState.rgbButtons[button] && !mouseState.rgbButtons[button]);
}

bool Mouse::MouseButtonInput(MouseButton button)
{
	return (bool)(mouseState.rgbButtons[button]);
}

const MyMath::Vector2 Mouse::GetMousePos() const
{
	return mousePos;
}

const MyMath::Vector2 Mouse::GetWorldMousePos() const
{
	return worldMousePos;
}

const MyMath::Vector3 Mouse::GetMouseMove() const
{
	MyMath::Vector3 result = { static_cast<float>(mouseState.lX), static_cast<float>(mouseState.lY), static_cast<float>(mouseState.lZ) };
	return result;
}
