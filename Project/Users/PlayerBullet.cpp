#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(ObjModel* model, ObjObject3d* obj, const MyMath::Vector3& position)
{
	//代入チェック
	assert(model);

	bulletModel_ = model;
	bulletObj_ = obj;
	bulletTrans_.Initialize();
	bulletTrans_.translation_ = position;

}

void PlayerBullet::Update(Camera* camera)
{
	bulletTrans_.Update(camera);
}

void PlayerBullet::Draw()
{
	
}
