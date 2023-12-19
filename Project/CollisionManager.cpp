#include "CollisionManager.h"

void CollisionManager::Initialize()
{
	model_.reset(ObjModel::LoadFromObj("skydome"));
}

void CollisionManager::CheckCollisionPair(Collider* colliderA,Collider* colliderB)
{
	//コライダーAの座標を取得
	MyMath::Vector3 posA = colliderA->GetCenterPosition();
	//コライダーBの座標を取得
	MyMath::Vector3 posB = colliderB->GetCenterPosition();
	//座標の差分ベクトル
	MyMath::Vector3 subtract = posB - posA;
	//座標AとBの距離を求める
	float distance = MyMathUtility::Vector3Length(subtract);
	float radius = std::sqrtf(colliderA->GetRadius() + colliderB->GetRadius());
	//球と球の交差判定
	if ( distance <= radius )
	{
		//コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		//コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}

void CollisionManager::CheckAllCollisions()
{
	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for ( ; itrA != colliders_.end(); ++itrA )
	{
		Collider* colliderA = *itrA;

		//イテレーターBはイテレーターAの次の要素から回す（重複判定を回避）
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for ( ; itrB != colliders_.end(); ++itrB )
		{
			Collider* colliderB = *itrB;

			//ペアの当たり判定
			CheckCollisionPair(colliderA,colliderB);
		}
	}
}

void CollisionManager::AddCollider(Collider* collider)
{
	colliders_.emplace_back(collider);
}

void CollisionManager::Reset()
{
	//リストを空っぽにする
	colliders_.clear();
}
