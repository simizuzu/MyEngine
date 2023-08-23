#pragma once
#include "PlayerBullet.h"
#include "Input.h"
#include "ObjObject3d.h"
#include "ObjModel.h"

class Player
{
public:
	void Initialize();

	void Update();

	void Draw();

	void Attack();

private:
	PlayerBullet* bullet = nullptr;
	Input* input = nullptr;
	ObjObject3d* bulletObj = nullptr;
	ObjModel* bulletModel = nullptr;
};

