#include "Controller.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

bool Controller::StickDeadZone(MyMath::Vector2& stick, const MyMath::Vector2& deadZoneInitial)
{
    bool x = false;
    bool y = false;

    if ((stick.x < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * deadZoneInitial.x &&
        stick.x > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * deadZoneInitial.x))
    {
        stick.x = 0.0f;
        x = true;
    }
    if ((stick.y < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * deadZoneInitial.y &&
        stick.y > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE * deadZoneInitial.y))
    {
        stick.y = 0.0f;
        y = true;
    }
    if (x && y)
    {
        return true;
    }

    return false;
}

void Controller::Update()
{
    oldXinputState = xinputState;
    ZeroMemory(&xinputState, sizeof(XINPUT_STATE));

    // コントローラーの取得
    DWORD inputGetState = XInputGetState(0, &xinputState);

    if (inputGetState == ERROR_SUCCESS)
    {
        shakeTimer--;
        XINPUT_VIBRATION vibration;
        ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

        if (0 < shakeTimer)
        {
            vibration.wLeftMotorSpeed = static_cast<WORD>(0.0f);
            vibration.wRightMotorSpeed = static_cast<WORD>(0.0f);
        }
        else
        {
            vibration.wLeftMotorSpeed = static_cast<WORD>(65535.0f * shakePower);
            vibration.wRightMotorSpeed = static_cast<WORD>(65535.0f * shakePower);
        }

        XInputSetState(inputGetState, &vibration);
    }
    else
    {

    }
}

bool Controller::TriggerButton(ControllerButton button)
{
    //トリガー
    if (button == LT)
    {
        return oldXinputState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && PushButton(button);
    }
    else if (button == RT)
    {
        return oldXinputState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && PushButton(button);
    }
    else
    {
        return !(oldXinputState.Gamepad.wButtons & button) && PushButton(button);
    }
}

bool Controller::TriggerStick(ControllerStick stickInput, const float& deadZoneRange, const MyMath::Vector2& deadZoneInitial)
{
    MyMath::Vector2 oldVec;
    MyMath::Vector2 vec;
    bool isLeftStick = stickInput <= L_RIGHT;
    if (isLeftStick)
    {
        oldVec = MyMath::Vector2(oldXinputState.Gamepad.sThumbLX, oldXinputState.Gamepad.sThumbLY);
        vec = MyMath::Vector2(xinputState.Gamepad.sThumbLX, xinputState.Gamepad.sThumbLY);
    }
    else
    {
        oldVec = MyMath::Vector2(oldXinputState.Gamepad.sThumbRX, oldXinputState.Gamepad.sThumbRY);
        vec = MyMath::Vector2(xinputState.Gamepad.sThumbRX, xinputState.Gamepad.sThumbRY);
    }

    if (!StickDeadZone(oldVec, deadZoneInitial))
    {
        return false;
    }

    if (StickDeadZone(vec, deadZoneInitial))
    {
        return false;
    }

    bool result = false;

    if (stickInput % 4 == L_UP)
    {
        result = !(deadZoneRange < (oldVec.y / MAX_STICK_NUM)) && deadZoneRange < (vec.y / MAX_STICK_NUM);
    }
    else if (stickInput % 4 == L_DOWN)
    {
        result = !(oldVec.y / MAX_STICK_NUM < -deadZoneRange) && vec.y / MAX_STICK_NUM < -deadZoneRange;
    }
    else if (stickInput % 4 == L_RIGHT)
    {
        result = !(deadZoneRange < (oldVec.x / MAX_STICK_NUM)) && deadZoneRange < (vec.x / MAX_STICK_NUM);
    }
    else if (stickInput % 4 == L_LEFT)
    {
        result = !(oldVec.x / MAX_STICK_NUM < -deadZoneRange) && vec.x / MAX_STICK_NUM < -deadZoneRange;
    }
    else
    {
        assert(0);
    }

    return result;
}

bool Controller::PushButton(ControllerButton button)
{
    if (button == LT)
    {
        return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bLeftTrigger;
    }
    else if (button == RT)
    {
        return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < xinputState.Gamepad.bRightTrigger;
    }
    else
    {
        return xinputState.Gamepad.wButtons & button;
    }
}

bool Controller::InputStick(ControllerStick stickInput, const float& deadZoneRange, const MyMath::Vector2& deadZoneInitial)
{
    MyMath::Vector2 vec;
    bool isLeftStick = stickInput <= L_RIGHT;

    if (isLeftStick)
    {
        vec = MyMath::Vector2(xinputState.Gamepad.sThumbLX, xinputState.Gamepad.sThumbLY);
    }
    else
    {
        vec = MyMath::Vector2(xinputState.Gamepad.sThumbRX, xinputState.Gamepad.sThumbRY);
    }

    if (StickDeadZone(vec, deadZoneInitial))return false;

    if (stickInput % 4 == L_UP)
    {
        return deadZoneRange < (vec.y / MAX_STICK_NUM);
    }
    else if (stickInput % 4 == L_DOWN)
    {
        return  vec.y / MAX_STICK_NUM < -deadZoneRange;
    }
    else if (stickInput % 4 == L_RIGHT)
    {
        return deadZoneRange < (vec.x / MAX_STICK_NUM);
    }
    else if (stickInput % 4 == L_LEFT)
    {
        return  vec.x / MAX_STICK_NUM < -deadZoneRange;
    }

    assert(0);
    return false;
}

bool Controller::ReleaseTriggerButton(ControllerButton button)
{
    //トリガー
    if (button == LT)
    {
        return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < oldXinputState.Gamepad.bLeftTrigger && !PushButton(button);
    }
    else if (button == RT)
    {
        return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < oldXinputState.Gamepad.bRightTrigger && !PushButton(button);
    }
    //ボタン
    else
    {
        return (oldXinputState.Gamepad.wButtons & button) && !PushButton(button);
    }
}

bool Controller::ReleaseTriggerStick(ControllerStick stickInput, const float& deadZoneRange, const MyMath::Vector2& deadZoneInitial)
{
    MyMath::Vector2 oldVec;
    MyMath::Vector2 vec;
    bool isLeftStick = stickInput <= L_RIGHT;

    if (isLeftStick)
    {
        oldVec = MyMath::Vector2(oldXinputState.Gamepad.sThumbLX, oldXinputState.Gamepad.sThumbLY);
        vec = MyMath::Vector2(xinputState.Gamepad.sThumbLX, xinputState.Gamepad.sThumbLY);
    }
    else
    {
        oldVec = MyMath::Vector2(oldXinputState.Gamepad.sThumbRX, oldXinputState.Gamepad.sThumbRY);
        vec = MyMath::Vector2(xinputState.Gamepad.sThumbRX, xinputState.Gamepad.sThumbRY);
    }

    if (!StickDeadZone(oldVec, deadZoneInitial))
    {
        return false;
    }

    if (StickDeadZone(vec, deadZoneInitial))
    {
        return false;
    }

    bool result = false;

    if (stickInput % 4 == L_UP)
    {
        result = deadZoneRange < (oldVec.y / MAX_STICK_NUM) && !(deadZoneRange < (vec.y / MAX_STICK_NUM));
    }
    else if (stickInput % 4 == L_DOWN)
    {
        result = oldVec.y / MAX_STICK_NUM < -deadZoneRange && !(vec.y / MAX_STICK_NUM < -deadZoneRange);
    }
    else if (stickInput % 4 == L_RIGHT)
    {
        result = deadZoneRange < (oldVec.x / MAX_STICK_NUM) && !(deadZoneRange < (vec.x / MAX_STICK_NUM));
    }
    else if (stickInput % 4 == L_LEFT)
    {
        result = oldVec.x / MAX_STICK_NUM < -deadZoneRange && !(vec.x / MAX_STICK_NUM < -deadZoneRange);
    }
    else
    {
        assert(0);
    }

    return result;
}

void Controller::ShakeController(const float& power, const int& flame)
{
    if (!(0 < power && power <= 1.0f))
    {
        assert(0);
    }

    shakePower = power;
    shakeTimer = flame;
}

MyMath::Vector2 Controller::GetLeftStickVec(const MyMath::Vector2& deadZoneRange)
{
    MyMath::Vector2 result(static_cast<float>(xinputState.Gamepad.sThumbLX), static_cast<float>(-xinputState.Gamepad.sThumbLY));
    StickDeadZone(result, deadZoneRange);
    return result / MAX_STICK_NUM;
}

MyMath::Vector2 Controller::GetRightStickVec(const MyMath::Vector2& deadZoneRange)
{
    MyMath::Vector2 result(static_cast<float>(xinputState.Gamepad.sThumbRX), static_cast<float>(-xinputState.Gamepad.sThumbRY));
    StickDeadZone(result, deadZoneRange);
    return result / MAX_STICK_NUM;
}
