#include "NormalBullet.h"
#include "Numbers.h"

void NormalBullet::Initialize(ObjObject3d* obj,const std::string filePath,const MyMath::Vector3& pos,const MyMath::Vector3 velocity,MyMath::Vector3& target,float bulletSpeed)
{
	//モデルを代入
	bulletObj_ = obj;
	bulletObj_->SetModel(filePath);

	//ワールドトランスフォームの初期化
	bulletTrans_.Initialize();
	bulletTrans_.SetTranslation(pos);

	//速度をメンバ変数に代入
	velocity_ = velocity;

	bulletSpeed_ = bulletSpeed;

	target_ = target;

	//弾の大きさを設定
	bulletTrans_.SetScale({ 3.0f,3.0f,3.0f });

	//衝突属性を設定
	SetCollisionAttribute(collisionAttributeEnemy);
	//衝突対象を自分の属性以外に設定(ビット反転)
	SetCollisionMask(~collisionAttributeEnemy);
}

void NormalBullet::Update(Camera* camera)
{
	const float bulletSpeed = 2.0f;

	//敵弾のホーミング
	//敵弾から自キャラへのベクトルを計算
	MyMath::Vector3 toTarget = target_ - MyMath::GetWorldPosition(bulletTrans_);
	//ベクトルを正規化する
	toTarget = MyMathUtility::MakeNormalize(toTarget);
	velocity_ = MyMathUtility::MakeNormalize(velocity_);
	//球面線形補間により、今の速度と自キャラのベクトルを内挿し、新たな速度とする
	velocity_ = MyMathUtility::Slerp(velocity_,toTarget,0.6f) * bulletSpeed;

	bulletTrans_.rotation_.y = std::atan2(velocity_.x,velocity_.z);
	MyMath::Vector3 tmp = velocity_;
	tmp.y = 0.0f;
	bulletTrans_.rotation_.x = std::atan2(-velocity_.y,tmp.length());

	bulletTrans_.translation_ += velocity_;
	bulletTrans_.Update(camera);

	//時間経過でデスフラグをtrueに
	if ( --deathTimer_ <= zero )
	{
		OnDead();
	}
}

void NormalBullet::Draw()
{
	//弾モデルの描画
	if ( !IsDead() )
	{
		bulletObj_->Draw(&bulletTrans_);
	}
}
