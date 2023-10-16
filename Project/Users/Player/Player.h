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
	void Initialize(Camera* camera);

	void Update();

	void Draw();

	void Attack();

	void Move();

	void PlayerTarget();

	void UIDraw();

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	void Reticle3D();

	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	void Reticle2D();

	void ReticleMouse();

	//void Finalize();

	Player() = default;
	~Player();

private:
	MyMath::Vector3 rot;
	MyMath::Vector3 translation;

	MyMath::Vector2 mousePos;
	MyMath::Vector3 rayDir;

	float angle;

public:
	float timeRate;						//何％時間が進んだか
private:
	float maxTime = 120.0f;				//全体時間[s]
	
	float targetTimeRate;
	int8_t pad1[4 ];
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

private:
	//代入演算子削除
	Player& operator=(const Player&) = delete;
	//コピーコンストラクタ削除
	Player(const Player&) = delete;
};

