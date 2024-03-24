#include "EnemyBullet.h"
#include "Numbers.h"
#include "Player.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <cassert>
#include <imgui.h>
MYENGINE_SUPPRESS_WARNINGS_END

void EnemyBullet::Initialize(const std::string& modelName,const MyMath::Vector3& pos,const MyMath::Vector3 velocity)
{
	bulletObj_.reset(ObjObject3d::Create());
	//モデルを代入
	bulletObj_->SetModel(modelName);

	//ワールドトランスフォームの初期化
	bulletTrans_.Initialize();
	bulletTrans_.translation_ = pos;

	//速度をメンバ変数に代入
	velocity_ = velocity;
	//弾のワールドトランスフォーム
	bulletTrans_.SetScale({ 3.0f,3.0f,3.0f });

	//衝突属性を設定
	SetCollisionAttribute(collisionAttributeEnemy);
	//衝突対象を自分の属性以外に設定(ビット反転)
	SetCollisionMask(~collisionAttributeEnemy);
}

void EnemyBullet::Update(Camera* camera)
{
	const float bulletSpeed = 0.4f;

	//敵弾のホーミング
	//敵弾から自キャラへのベクトルを計算
	MyMath::Vector3 toPlayer = player_->GetCenterPosition() - MyMath::GetWorldPosition(bulletTrans_);
	//ベクトルを正規化する
	toPlayer = MyMathUtility::MakeNormalize(toPlayer);
	velocity_ = MyMathUtility::MakeNormalize(velocity_);
	//球面線形補間により、今の速度と自キャラのベクトルを内挿し、新たな速度とする
	velocity_ = MyMathUtility::Slerp(velocity_,toPlayer, 0.1f) * bulletSpeed;
	bulletTrans_.translation_ += velocity_;

	//ホーミングの角度を計算
	bulletTrans_.rotation_.y = std::atan2(velocity_.x,velocity_.z);
	MyMath::Vector3 tmp = velocity_;
	tmp.y = 0.0f;
	bulletTrans_.rotation_.x = std::atan2(-velocity_.y,tmp.length());

	//弾の更新
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
	isHit_ = true;
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

bool EnemyBullet::IsHit() const
{
	return isHit_;
}

void EnemyBullet::SetPlayer(Player* player)
{
	player_ = player;
}
