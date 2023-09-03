#pragma once
#include "PlayerBullet.h"
#include "Input.h"
#include "LevelLoader.h"
#include "Camera.h"
#include <list>
#include "Sprite.h"
#include "TextureManager.h"

class Player
{
public:
	void Initialize(Camera* camera);

	void Update();

	void Draw();

	void Attack();

	void Move();

	void UIDraw();

private:
	MyMath::Vector3 rot;
	MyMath::Vector3 translation;

	MyMath::Vector2 mousePos;
	MyMath::Vector3 mouseVec;
	MyMath::Vector3 mouseAngle;

private:
	//LevelData* curveData2;

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

public:
	~Player();
};

