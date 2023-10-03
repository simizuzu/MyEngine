#pragma once
#include "DirectX12Math.h"
#include "Camera.h"
#include "Input.h"
#include "WorldTransform.h"
#include "LevelLoader.h"

class GameCamera
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Update();

	/// <summary>
	/// 各変数のリセット関数
	/// </summary>
	void Reset();

	float timeRate;						//何％時間が進んだか

private:
	//レベルエディタ(ベジェ曲線)
	LevelData* curveData;

	//初期座標
	MyMath::Vector3 startPoint;
	//終点座標
	MyMath::Vector3 endPoint;

	//カメラのトランスフォーム
	WorldTransform cameraTrans;

	float maxTime = 5.0f;				//全体時間[s]
	
	float targetTimeRate;
	size_t startIndex = 1;
	uint32_t startCount = 0;
	uint32_t nowCount = 0;
	uint32_t elapsedCount = 0;

	std::vector<LevelData::CurveData> points;

	MyMath::Vector3 pos;
	MyMath::Vector3 target;

	Input* input_;
	Camera* camera_;
};

