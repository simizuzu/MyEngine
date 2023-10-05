#pragma once
#include "DirectX12Math.h"
#include "Camera.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END

class FPSCamera
{
public:

	void Initialize(Camera* camera);

	void Update();

	void Move();

	/// <summary>
	/// 回転座標から3次元座標への変換
	/// </summary>
	void ConvertAngleToThreedimension();

private:
	/// <summary>
	/// HAngleの取得関数(水平方向)
	/// </summary>
	/// <param name="eye">始点</param>
	/// <param name="target">注視点</param>
	/// <returns>角度</returns>
	float GetHAngle(const MyMath::Vector3& eye, const MyMath::Vector3& target);

	/// <summary>
	/// VAngleの取得関数(垂直方向)
	/// </summary>
	/// <param name="eye">始点</param>
	/// <param name="target">注視点</param>
	/// <returns></returns>
	float GetVAngle(const MyMath::Vector3& eye, const MyMath::Vector3& target);

private:
	std::unique_ptr<Camera> camera_;

	float HAngle;
	float VAngle;

private:
	//コピーコンストラクタ・代入演算子削除
	FPSCamera& operator=(const FPSCamera&) = delete;
	FPSCamera(const FPSCamera&) = delete;
};

