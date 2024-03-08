#include "CollisionManager.h"
#include <cmath>

void CollisionManager::CheckCollisionPair(Collider* colliderA,Collider* colliderB)
{
	//衝突フィルタリング
	if ( !( colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask() ) ||
		!( colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask() ) )
	{
		return;
	}

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

bool CollisionManager::CheckRay2Sphere(const Ray& ray,const Sphere& sphere)
{
	MyMath::Vector3 m = ray.start - sphere.center;
	float b = m.dot(ray.dir);
	float c = m.dot(m) - sphere.radius * sphere.radius;

	//Rayの視点がsphereの外側にあり(c>0)、Rayがsphereから離れていく方向を差している場合(b>0)、当たらない
	if ( c > 0.0f && b > 0.0f )
	{
		return false;
	}

	float discr = b*b-c;
	//負の判別式は例が球を外れていることに一致
	if ( discr < 0.0f )
	{
		return false;
	}

	//交差する最小の値tを計算
	float t = -b - std::sqrt(discr);
	//tが負である場合、レイは球の内側から開始しているのでtをゼロにクランプ
	if ( t < 0 )
	{
		t = 0.0f;
	}

	return true;
}
