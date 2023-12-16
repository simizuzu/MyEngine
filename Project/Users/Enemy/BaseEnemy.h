#pragma once
#include "Camera.h"
#include "Collider.h"

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

public:
	//中心座標を取得
	virtual MyMath::Vector3 GetCenterPosition() const override = 0;

private:
//代入演算子削除
	BaseEnemy& operator=(const BaseEnemy&) = delete;
	//コピーコンストラクタ削除
	BaseEnemy(const BaseEnemy&) = delete;
};

