#pragma once
#include "Camera.h"
#include "Input.h"
#include "LevelLoader.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Collider.h"
#include "ObjObject3d.h"

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

private:
	MyMath::Vector3 rot;
	MyMath::Vector3 translation;

	MyMath::Vector3 mousePos;
	MyMath::Vector3 rayDir;

	MyMath::Vector2 UIReticleScale = { 0.2f,0.2f };
	float UIReticleRot = 0.0f;
	MyMath::Vector2 UIReticleAnchorpoint = {0.5f,0.5f};

	float angle;

	int8_t xPos_absolute,yPos_absolute;
	uint8_t xPos,yPos;
	uint32_t collisionMask;
	
	POINT mousePosition;
	MyMath::Vector2 windowWH;
	MyMath::Vector2 mouseMove;
	MyMath::Vector2 mouseMoved;
	MyMath::Vector3 gunmodelTranslation = {0.3f,-0.15f,1.5f};
	MyMath::Vector2 stickDeadZone;

private:
	//ダメージ用全体スプライト(赤色)
	MyMath::Vector4 color = {1,0,0,0};

	float targetTimeRate;
	bool bulletIntervalFlag = false;
	bool hitFlag = false;
	uint8_t bulletIntervalTimer = 6;
	int8_t pad1[ 1 ];

	size_t startIndex = 1;
	uint32_t startCount = 0;
	uint32_t nowCount = 0;
	uint32_t elapsedCount = 0;

	LevelData::CurveData curves;

private:
	LevelData* curveData;
	Input* input = nullptr;

	//カメラ
	std::unique_ptr<Camera> camera_;

	//プレイヤーのモデル
	std::unique_ptr<ObjObject3d> playerObj;

	//プレイヤー用ワールドトランスフォーム
	WorldTransform playerTrans;
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle;

	std::unique_ptr<Sprite> takenDamage;
	TextureData TexTakenDamage;

	float cameraHAngle;
	float cameraVAngle;

private:
	//代入演算子削除
	Player& operator=(const Player&) = delete;
	//コピーコンストラクタ削除
	Player(const Player&) = delete;
};

