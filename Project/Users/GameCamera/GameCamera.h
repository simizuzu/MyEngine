#pragma once
#include "DirectX12Math.h"
#include "Camera.h"
#include "Input.h"
#include "WorldTransform.h"
#include "LevelLoader.h"


 /**
 * @class GameCamera.h
 * @brief GameCameraのクラス
 */

/// <summary>
/// ゲームカメラ
/// </summary>
class GameCamera
{
public:
	GameCamera() = default;
	~GameCamera() = default;
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera, Input* input);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Update();

	/// <summary>
	/// 各変数のリセット関数
	/// </summary>
	void Reset();

	/// <summary>
	/// startとendをfor文で作る処理 {start,start,P1…Pn,end,end}
	/// </summary>
	/// <param name="curvePoint">カーブのデータ</param>
	void SplinePointLineUp(std::vector<LevelData::CurveData> curvePoint);

	float timeRate;						//何％時間が進んだか
	int8_t pad1[4 ];

	MyMath::Vector3 pos;
	MyMath::Vector3 target;
private:
	//レベルエディタ(ベジェ曲線)
	LevelData* curveData;

	//初期座標
	MyMath::Vector3 startPoint;
	//終点座標
	MyMath::Vector3 endPoint;

	//カメラのトランスフォーム
	WorldTransform cameraTrans;

	float maxTime = 0.5f;				//全体時間[s]

	//1秒60フレーム
	const float oneSecondFrame = 60.0f;
	//90度
	const float degree90 = 90.0f;
	//レート最大数[0~1]
	const float maxRate = 1.0f;

	float targetTimeRate;
	int8_t pad3[4 ];
	size_t startIndex = 1;
	uint32_t startCount = 0;
	uint32_t nowCount = 0;
	uint32_t elapsedCount = 0;
	int8_t pad2[ 4 ];

	std::vector<LevelData::CurveData> points;

	Input* input_ = nullptr;
	Camera* camera_ = nullptr;

	WorldTransform worldTransform_;
	MyMath::Quaternion quaternion;

private:
	/// <summary>
	/// カメラの回転角を取得
	/// </summary>
	/// <param name="rotation"></param>
	void SetCameraRot(MyMath::Vector3& rotation);
private:
	//代入演算子削除
	GameCamera& operator=(const GameCamera&) = delete;
	//コピーコンストラクタ削除
	GameCamera(const GameCamera&) = delete;
};

