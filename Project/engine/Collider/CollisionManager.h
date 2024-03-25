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
	/// <returns>当たっているか否か</returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphereA">球A</param>
	/// <param name="sphereB">球B</param>
	/// <returns>当たっているか否か</returns>
	static bool CheckSphre2Sphere(const Sphere& sphereA,const Sphere& sphereB);

private:
	//コピーコンストラクタ・代入演算子削除
	CollisionManager& operator=(const CollisionManager&) = delete;
	CollisionManager(const CollisionManager&) = delete;
};

