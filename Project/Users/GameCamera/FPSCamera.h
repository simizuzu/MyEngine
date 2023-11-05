#pragma once
#include "DirectX12Math.h"
#include "Camera.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END

 /**
 * @class FPSCamera.h
 * @brief FPSCameraのクラス
 */

/// <summary>
/// FPSカメラ
/// </summary>
class FPSCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Initialize(Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// カメラの挙動
	/// </summary>
	void Move();

	/// <summary>
	/// 回転座標から3次元座標への変換
	/// </summary>
	void ConvertAngleToThreedimension(MyMath::Vector2 value);

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
	//全体のカメラの角度(360°)
	float cameraAngle = 360.0f;
	int8_t pad1[4 ];

	std::unique_ptr<Camera> camera_;

	MyMath::Vector3 initTarget = {0,0,0};

	float HAngle;
	float VAngle;
	int8_t pad2[ 4 ];

private:
	//コピーコンストラクタ・代入演算子削除
	FPSCamera& operator=(const FPSCamera&) = delete;
	FPSCamera(const FPSCamera&) = delete;
};

