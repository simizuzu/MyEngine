#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(ObjModel* model, ObjObject3d* obj, const MyMath::Vector3& position)
{
	//代入チェック
	assert(model);

	//モデルを代入
	bulletModel_ = model;
	bulletObj_ = obj;

	//ワールドトランスフォームの初期化
	bulletTrans_.Initialize();
	bulletTrans_.translation_ = position;

}

void PlayerBullet::Update(Camera* camera)
{
	bulletTrans_.Update(camera);
}

void PlayerBullet::Draw()
{
	//モデルの描画
	bulletObj_->Draw(&bulletTrans_);
}
