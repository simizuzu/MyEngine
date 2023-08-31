#pragma once
#include "PlayerBullet.h"
#include "Input.h"
#include "LevelLoader.h"
#include "Camera.h"

class Player
{
public:
	void Initialize(Camera* camera);

	void Update();

	void Draw();

	void Attack();

private:
	MyMath::Vector3 rot;

private:
	//LevelData* curveData2;

	PlayerBullet* bullet = nullptr;
	Input* input = nullptr;

	std::unique_ptr<Camera> camera_;

	std::unique_ptr<ObjObject3d> playerObj;
	std::unique_ptr<ObjModel> playerModel;

	WorldTransform playerTrans;
};

