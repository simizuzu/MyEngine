#pragma once
#define DIRECTINPUT_VERSION 0x0800 // DirectInput�̃o�[�W�����w��
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
/// �}�E�X
/// </summary>
class Mouse
{
private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> mouseDev;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 oldMouseState = {};
	//�}�E�X�̃Q�[����ԓ��ł̃��C
	MyMath::Vector2 mousePos;
	MyMath::Vector2 worldMousePos;
	MyMath::Vector3 mouseMove;

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="dinput"></param>
	void Initialize(IDirectInput8* dinput);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �}�E�X�{�^���̓���
	/// </summary>
	/// <param name="button">�`�F�b�N�������{�^��</param>
	/// <returns>���������ǂ���</returns>
	bool MouseButtonTrigger(MouseButton button);

	/// <summary>
	/// �}�E�X�{�^���̗������u��
	/// </summary>
	/// <param name="button">�`�F�b�N�������{�^��</param>
	/// <returns>��������</returns>
	bool MouseButtonOffTrigger(MouseButton button);

	/// <summary>
	/// �}�E�X�{�^���̓���
	/// </summary>
	/// <param name="button">�`�F�b�N�������{�^��</param>
	/// <returns>��������</returns>
	bool MouseButtonInput(MouseButton button);

	/// <summary>
	/// �}�E�X�̈ʒu
	/// </summary>
	/// <returns�}�E�X�̈ʒu></returns>
	const MyMath::Vector2 GetMousePos()const;

	/// <summary>
	/// �}�E�X�̈ʒu
	/// </summary>
	/// <returns�}�E�X�̈ʒu></returns>
	const MyMath::Vector2 GetWorldMousePos()const;

	/// <summary>
	/// �}�E�X�̈ʒu
	/// </summary>
	/// <returns�}�E�X�̈ʒu></returns>
	const MyMath::Vector3 GetMouseMove()const;
};

