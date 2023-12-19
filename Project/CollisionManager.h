#pragma once
#include "SuppressWarning.h"
#include "Collider.h"
#include "DirectX12Math.h"

#include "ObjModel.h"

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

	std::unique_ptr<ObjModel> model_;

public:
	CollisionManager() = default;
	~CollisionManager() = default;

	//初期化
	void Initialize();

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

private:
	//コピーコンストラクタ・代入演算子削除
	CollisionManager& operator=(const CollisionManager&) = delete;
	CollisionManager(const CollisionManager&) = delete;
};

