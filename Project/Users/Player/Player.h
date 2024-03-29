#pragma once
#include "Camera.h"
#include "Input.h"
#include "LevelLoader.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Collider.h"
#include "ObjObject3d.h"

#include "LevelLoader.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <list>
MYENGINE_SUPPRESS_WARNINGS_END

 /**
 * @class Player.h
 * @brief Playerのクラス
 */

/// <summary>
/// プレイヤー
/// </summary>
class Player : public Collider
{
private:
	float rotationSpeed = 0.05f;
	const float maxRotate = 1.3f;
	const float maxValueRotate = 1.5f;
	//1秒60フレーム
	const float oneSecondFrame = 60.0f;
	//90度
	const float degree90 = 90.0f;
	//レート最大数[0~1]
	const float maxRate = 1.0f;

public:
	//コンストラクタ・デストラクタ
	Player()= default;
	~Player() = default;

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
	/// 描画
	/// </summary>
	void Draw();

	//プレイヤーのワールド座標を取得する
	MyMath::Vector3 GetCenterPosition() const override;

private:
	/// <summary>
	/// カメラの回転処理
	/// </summary>
	void RotateCamera();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	void SetParent(const WorldTransform* parent);

	/// <summary>
	/// startとendをfor文で作る処理 {start,start,P1…Pn,end,end}
	/// </summary>
	/// <param name="curvePoint">カーブのデータ</param>
	void SplinePointLineUp(std::vector<LevelData::CurveData> curvePoint);

	/// <summary>
	/// レールカメラの処理
	/// </summary>
	void RailCamera();

public:
	float timeRate;//何％時間が進んだか

private:
	MyMath::Vector3 rot;
	MyMath::Vector3 translation;
	
	MyMath::Vector3 gunmodelTranslation;
	MyMath::Vector2 stickDeadZone;

private:

	
	
	float maxTime = 0.5f;	//全体時間[s]
	uint32_t elapsedCount = 0;
	uint8_t bulletIntervalTimer = 6;	
	bool bulletIntervalFlag = false;
	int8_t pad[ 6 ];

	size_t startIndex = 1;
	uint32_t startCount = 0;
	uint32_t nowCount = 0;

private:
	//レベルエディタ(ベジェ曲線)
	LevelData* curveData;
	std::vector<LevelData::CurveData> points;
	Input* input = nullptr;

	//カメラ
	std::unique_ptr<Camera> camera_;

	//プレイヤーのモデル
	std::unique_ptr<ObjObject3d> playerObj;

	//プレイヤー用ワールドトランスフォーム
	WorldTransform playerTrans;
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle;

private:
	//代入演算子削除
	Player& operator=(const Player&) = delete;
	//コピーコンストラクタ削除
	Player(const Player&) = delete;
};

