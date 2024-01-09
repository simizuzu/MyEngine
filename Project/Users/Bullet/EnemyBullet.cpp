#include "EnemyBullet.h"
#include "Numbers.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <cassert>
MYENGINE_SUPPRESS_WARNINGS_END

void EnemyBullet::Initialize(ObjModel* model,ObjObject3d* obj,const MyMath::Vector3& pos,const MyMath::Vector3 velocity)
{
	assert(model);

	//モデルを代入
	bulletModel_ = model;
	bulletObj_ = obj;
	bulletObj_->SetModel(bulletModel_);

	//ワールドトランスフォームの初期化
	bulletTrans_.Initialize();
	bulletTrans_.translation_ = pos;

	//速度をメンバ変数に代入
	velocity_ = velocity;

	//衝突属性を設定
	SetCollisionAttribute(collisionAttributeEnemy);
	//衝突対象を自分の属性以外に設定(ビット反転)
	SetCollisionMask(~collisionAttributeEnemy);
}

void EnemyBullet::Update(Camera* camera)
{
	bulletTrans_.translation_ += velocity_;
	bulletTrans_.SetScale({ 1.8f,1.8f,1.8f });
	bulletTrans_.Update(camera);

	//時間経過でデスフラグをtrueに
	if ( --deathTimer_ <= zero )
	{
		isDead_ = true;
	}
}

void EnemyBullet::Draw()
{
	//弾モデルの描画
	if ( !isDead_ )
	{
		bulletObj_->Draw(&bulletTrans_);
	}
}

void EnemyBullet::OnCollision()
{
	isDead_ = true;
}

MyMath::Vector3 EnemyBullet::GetCenterPosition() const
{
	//ワールド座標に変換
	MyMath::Vector3 worldPos = MyMath::GetWorldPosition(bulletTrans_);
	return worldPos;
}

bool EnemyBullet::IsDead() const
{
	return isDead_;
}
