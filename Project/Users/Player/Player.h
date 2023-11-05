#pragma once
#include "PlayerBullet.h"
#include "Input.h"
#include "LevelLoader.h"
#include "Camera.h"
#include "Sprite.h"
#include "TextureManager.h"

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
class Player
{
public:
	//コンストラクタ・デストラクタ
	Player() = default;
	~Player();

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

	/// <summary>
	/// レティクルの描画
	/// </summary>
	void UIDraw();

private:

	///</summary>
	/// 自機のワールド座標から3Dレティクルのワールド座標を計算
	///</summary>
	void Reticle3D();

	///</summary>
	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	///</summary>
	void Reticle2D();

	/// <summary>
	/// マウスで動くレティクルの処理
	/// </summary>
	void ReticleMouse();

	/// <summary>
	/// 攻撃したか
	/// </summary>
	void Attack();

	/// <summary>
	/// プレイヤーの挙動
	/// </summary>
	void Move();

	//void Finalize();

private:
	MyMath::Vector3 rot;
	MyMath::Vector3 translation;

	MyMath::Vector3 mousePos;
	MyMath::Vector3 rayDir;

	MyMath::Vector2 UIReticleScale = { 0.2f,0.2f };
	float UIReticleRot = 0.0f;
	MyMath::Vector2 UIReticleAnchorpoint = {0.5f,0.5f};

	float angle;

public:
	float timeRate;						//何％時間が進んだか
private:
	float maxTime = 120.0f;				//全体時間[s]
	
	float targetTimeRate;
	int8_t pad1[ 4 ];

	size_t startIndex = 1;
	uint32_t startCount = 0;
	uint32_t nowCount = 0;
	uint32_t elapsedCount = 0;

	LevelData::CurveData curves;

private:
	LevelData* curveData;

	//弾
	std::list<PlayerBullet*> bullets;
	Input* input = nullptr;

	//カメラ
	std::unique_ptr<Camera> camera_;

	//プレイヤーのモデル
	std::unique_ptr<ObjObject3d> playerObj;
	std::unique_ptr<ObjModel> playerModel;

	//弾のモデル
	std::unique_ptr<ObjObject3d> bulletObj;
	std::unique_ptr<ObjModel> bulletModel;

	//レティクルのモデル
	std::unique_ptr<ObjObject3d> reticleObj;
	std::unique_ptr<ObjModel> reticleModel;

	//プレイヤー用ワールドトランスフォーム
	WorldTransform playerTrans;
	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle;

	//2Dレティクル用スプライト
	std::unique_ptr<Sprite> sprite2DReticle;
	TextureData texReticle;

	float cameraHAngle;
	float cameraVAngle;

private:
	//代入演算子削除
	Player& operator=(const Player&) = delete;
	//コピーコンストラクタ削除
	Player(const Player&) = delete;
};

