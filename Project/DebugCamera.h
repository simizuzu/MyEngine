#pragma once

#include "Camera.h"
#include "Input.h"

class DebugCamera : public Camera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Input* input,Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	Camera* camera_ = nullptr;
	Input* input_ = nullptr;

	float distance_ = 20;

	float scaleX_ = 1.0f;
	float scaleY_ = 1.0f;

	MyMath::Vector3 rotation_ = { 0,0,0 };
	MyMath::Vector3 translation_ = {0,0,-50};
	MyMath::Matrix4 matrot;
};

