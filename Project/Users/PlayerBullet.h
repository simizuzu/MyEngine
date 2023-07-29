#pragma once
#include "ObjObject3d.h"
#include "ObjModel.h"
#include "WorldTransform.h"

class PlayerBullet
{
public:
	void Initialize(ObjModel* model, ObjObject3d* obj, const MyMath::Vector3& position);

	void Update(Camera* camera);

	void Draw();

private:
	ObjModel* bulletModel_ = nullptr;
	ObjObject3d* bulletObj_ = nullptr;

	WorldTransform bulletTrans_;
};

