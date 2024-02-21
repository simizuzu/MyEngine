#pragma once
#include "Camera.h"
#include "Collider.h"
#include "EnemyBullet.h"
#include "CollisionPrimitive.h"

/**
 * @class Enemy.h
 * @brief 敵キャラの基底クラス
 */

/// <summary>
/// エネミー
/// </summary>
class BaseEnemy : public Collider
{
public:
	enum EnemyType
	{
		NONE,
	};

public:
	BaseEnemy() = default;
	virtual ~BaseEnemy() = default;

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual bool IsDead() const = 0;

	virtual void OnCollision() override = 0;

	const Sphere& GetSphereCenter() const;

public:
	//中心座標を取得
	virtual MyMath::Vector3 GetCenterPosition() const override = 0;

	virtual const std::list<EnemyBullet*>& GetBullets() const = 0;

protected:
	std::list<EnemyBullet*> bullets;

private:
//代入演算子削除
	BaseEnemy& operator=(const BaseEnemy&) = delete;
	//コピーコンストラクタ削除
	BaseEnemy(const BaseEnemy&) = delete;
};

