#pragma once
#include "SuppressWarning.h"
#include "Collider.h"
#include "DirectX12Math.h"

#include "ObjModel.h"
#include "CollisionPrimitive.h"

MYENGINE_SUPPRESS_WARNINGS_BEGIN
#include <list>
#include <memory>
MYENGINE_SUPPRESS_WARNINGS_END

 /**
 * @class CollisionManager.h
 * @brief 当たり判定を総括するクラス
 */
class CollisionManager
{
private:
	//コライダー
	std::list<Collider*> colliders_;

public:
	CollisionManager() = default;
	~CollisionManager() = default;

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	//全ての当たり判定をチェック
	void CheckAllCollisions();

	void AddCollider(Collider* collider);

	//リセット
	void Reset();

public:
	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="distance">距離</param>
	/// <param name="inter">交点</param>
	/// <returns>交差しているか否か</returns>
	bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, MyMath::Vector3* inter = nullptr);

private:
	//コピーコンストラクタ・代入演算子削除
	CollisionManager& operator=(const CollisionManager&) = delete;
	CollisionManager(const CollisionManager&) = delete;
};

