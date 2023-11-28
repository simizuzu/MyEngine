#include "PlayerBullet.h"
#include "Numbers.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file PlayerBullet.cpp
 * @brief Playerの弾に関するの処理が書かれてあります
 */

void PlayerBullet::Initialize(ObjModel* model, ObjObject3d* obj, const MyMath::Vector3& position, const MyMath::Vector3 velosity)
{
	//代入チェック
	assert(model);

	//モデルを代入
	bulletModel_ = model;
	bulletObj_ = obj;
	bulletObj_->SetModel(bulletModel_);

	//ワールドトランスフォームの初期化
	bulletTrans_.Initialize();
	bulletTrans_.translation_ = position;

	//速度をメンバ変数に代入
	velocity_ = velosity;
}

void PlayerBullet::Update(Camera* camera)
{
	bulletTrans_.translation_ += velocity_;
	bulletTrans_.SetScale({0.8f,0.8f,0.8f});
	bulletTrans_.Update(camera);

	//時間経過でデスフラグをtrueに
	if (--deathTimer_ <= zero) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw()
{
	//モデルの描画
	bulletObj_->Draw(&bulletTrans_);
}

bool PlayerBullet::IsDead() const
{
	return isDead_;
}

