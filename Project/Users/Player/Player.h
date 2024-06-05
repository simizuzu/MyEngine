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
	float rotationSpeed = 0.02f;
	const float maxRotate = 1.3f;
	const float maxValueRotate = 1.5f;
	//1秒60フレーム
	const float oneSecondFrame = 70.0f;
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

	//死んでいるかどうか
	bool IsDead()const;

	MyMath::Matrix4 GetPlayerMatWorld()const;

private:
	/// <summary>
	/// カメラの回転処理
	/// </summary>
	void RotateCamera();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

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
	MyMath::Vector3 oldTranslation;
	
	MyMath::Vector3 gunmodelTranslation;
	MyMath::Vector2 stickDeadZone;

private:
	float maxTime = 0.5f;	//全体時間[s]
	uint32_t elapsedCount = 0;
	uint8_t bulletIntervalTimer = 6;
	uint8_t HP = 20;
	bool bulletIntervalFlag = false;
	bool isDead = false;

	size_t startIndex = 1;
	uint32_t startCount = 0;
	uint32_t nowCount = 0;

	const MyMath::Vector2 hpSize = { 0.1f,0.1f };

private:
	//レベルエディタ(ベジェ曲線)
	LevelData* curveData;
	std::vector<LevelData::CurveData> points;
	Input* input = nullptr;

	std::unique_ptr<Sprite> hpUI;
	TextureData texHp;

	//カメラ
	Camera* camera_ = nullptr;

	//プレイヤーのモデル
	std::unique_ptr<ObjObject3d> playerObj;
	std::unique_ptr<ObjObject3d> shuttleObj;

	//プレイヤー用ワールドトランスフォーム
	WorldTransform playerTrans;
	//スペースシャトル用ワールドトランスフォーム
	WorldTransform shuttleTrans;

private:
	//代入演算子削除
	Player& operator=(const Player&) = delete;
	//コピーコンストラクタ削除
	Player(const Player&) = delete;
};

