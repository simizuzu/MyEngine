#pragma once
#include "Camera.h"
#include "Input.h"
#include "LevelLoader.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "Collider.h"
#include "ObjObject3d.h"
#include "Reticle3D.h"
#include "BulletManager.h"

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

private:
	/// <summary>
	/// カメラの回転処理
	/// </summary>
	void RotateCamera();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision() override;

	/// <summary>
	/// レールカメラの処理
	/// </summary>
	void RailCamera();

	/// <summary>
	/// プレイヤーの攻撃処理
	/// </summary>
	void Attack();

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
	//レベルエディタ(ベジェ曲線)
	Keyframe* keyframeData;
	MyMath::Vector3 playerTranslate;
	MyMath::Quaternion playerRotate;
public:

	float animTime = 1.0f / 60.0f;

private:
	Input* input = nullptr;

	//バレットマネージャ
	BulletManager* bulletManager_ = nullptr;
	//弾の速度
	const float bulletSpeed = 1.0f;
	MyMath::Vector3 velocity = { 0.0f,0.0f,bulletSpeed };
	//弾のモデル
	std::unique_ptr<ObjObject3d> bulletObj;

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

	std::unique_ptr<Reticle3D> reticle3d;
	WorldTransform reticleTrans;

private:
	//代入演算子削除
	Player& operator=(const Player&) = delete;
	//コピーコンストラクタ削除
	Player(const Player&) = delete;
};

