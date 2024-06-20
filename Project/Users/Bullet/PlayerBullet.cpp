#include "PlayerBullet.h"

void PlayerBullet::Initialize(const std::string& modelName,const MyMath::Vector3& pos,const MyMath::Vector3 velocity)
{
	bulletObj_.reset(ObjObject3d::Create());
	//モデルをセット
	bulletObj_->SetModel(modelName);

	//ワールドトランスフォームの初期化
	bulletTrans_.Initialize();
	bulletTrans_.translation_ = pos;

	//速度をメンバ変数に代入
	velocity_ = velocity;

	//衝突属性を設定
	SetCollisionAttribute(collisionAttributePlayer);
	//衝突対象を自分の属性以外に設定(ビット反転)
	SetCollisionMask(~collisionAttributePlayer);
}

void PlayerBullet::Update(Camera* camera)
{
	const int8_t death = 0;

	//弾の更新
	bulletTrans_.Update(camera);

	//時間経過でデスフラグをtrueに
	if ( --deathTimer_ <= death )
	{
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

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

MyMath::Vector3 PlayerBullet::GetCenterPosition() const
{
	return MyMath::Vector3();
}

bool PlayerBullet::IsDead() const
{
	return false;
}
