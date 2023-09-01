#pragma once
#include "PlayerBullet.h"
#include "Input.h"
#include "LevelLoader.h"
#include "Camera.h"
#include <list>

class Player
{
public:
	void Initialize(Camera* camera);

	void Update();

	void Draw();

	void Attack();

private:
	MyMath::Vector3 rot;
	MyMath::Vector3 translation;

private:
	//LevelData* curveData2;

	std::list<PlayerBullet*> bullets;
	Input* input = nullptr;

	std::unique_ptr<Camera> camera_;

	std::unique_ptr<ObjObject3d> playerObj;
	std::unique_ptr<ObjModel> playerModel;

	std::unique_ptr<ObjObject3d> bulletObj;
	std::unique_ptr<ObjModel> bulletModel;

	WorldTransform playerTrans;

public:
	~Player();
};

