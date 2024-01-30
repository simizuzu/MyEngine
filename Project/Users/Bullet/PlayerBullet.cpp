#include "PlayerBullet.h"
#include "Numbers.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

/**
 * @file PlayerBullet.cpp
 * @brief Playerの弾に関するの処理が書かれてあります
 */

void PlayerBullet::Initialize(ObjObject3d* obj,const MyMath::Vector3& position, const MyMath::Vector3 velocity)
{

	//モデルを代入
	bulletObj_ = obj;
	bulletObj_->SetModel("box");

	//ワールドトランスフォームの初期化
	bulletTrans_.Initialize();
	bulletTrans_.translation_ = position;

	//速度をメンバ変数に代入
	velocity_ = velocity;

	//衝突属性を設定
	SetCollisionAttribute(collisionAttributePlayer);
	//衝突対象を自分の属性以外に設定(ビット反転)
	SetCollisionMask(~collisionAttributePlayer);
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
	//弾モデルの描画
	if ( !isDead_ )
	{
		bulletObj_->Draw(&bulletTrans_);
	}
}

bool PlayerBullet::IsDead() const
{
	return isDead_;
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

MyMath::Vector3 PlayerBullet::GetCenterPosition() const
{
	//ワールド座標に変換
	MyMath::Vector3 worldPos = MyMath::GetWorldPosition(bulletTrans_);
	return worldPos;
}

