#pragma once
#include "DirectX12Math.h"
#include "Camera.h"
#include "Input.h"

class GameCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Input* input,Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Update(Camera* camera);

	/// <summary>
	/// 各変数のリセット関数
	/// </summary>
	void Reset();
private:
	//初期座標
	MyMath::Vector3 startPoint;
	//終点座標
	MyMath::Vector3 endPoint;

	//現在の時間(フレーム)
	float nowFlame;
	//終了する時間(フレーム)
	float endFlame;

	MyMath::Vector3 cameraTarget_;

	bool ReleaseKeyFlag = false;

	Input* input_;
};

