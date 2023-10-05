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

	int8_t pad1[ 4 ];

private:
	//コピーコンストラクタ削除
	DebugCamera& operator=(const DebugCamera&) = delete;
	//代入演算子削除
	DebugCamera(const DebugCamera&) = delete;
};

