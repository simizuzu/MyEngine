#include "Bullet.h"
#include "Numbers.h"

void Bullet::Set(const MyMath::Vector3& pos,const MyMath::Vector3 velocity)
{
	bulletTrans_.Initialize();
	bulletTrans_.SetTranslation(pos);
	velocity_ = velocity;
	isDead_ = false;
}

void Bullet::Update(Camera* camera)
{
	bulletTrans_.translation_ += velocity_;
	bulletTrans_.SetScale({ 0.8f,0.8f,0.8f });
	bulletTrans_.Update(camera);

	if ( !isDead_ )
	{
		//時間経過でデスフラグをtrueに
		if ( --deathTimer_ <= zero )
		{
			isDead_ = true;
		}
	}

	bulletTrans_.Update(camera);
}

void Bullet::Draw()
{
	if ( !isDead_ )
	{
		obj_->Draw(&bulletTrans_);
	}
}

bool Bullet::GetIsDead() const
{
	return isDead_;
}

void Bullet::SetIsDead(bool isDead)
{
	isDead_ = isDead;
}

void Bullet::OnCollision()
{
	isDead_ = true;
}

MyMath::Vector3 Bullet::GetWorldPos()
{
	//ワールド座標に変換
	MyMath::Vector3 worldPos = MyMath::GetWorldPosition(bulletTrans_);
	return worldPos;
}
