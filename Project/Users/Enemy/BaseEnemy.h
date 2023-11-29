#pragma once

#include "Camera.h"

/**
 * @class Enemy.h
 * @brief 敵キャラの基底クラス
 */

/// <summary>
/// エネミー
/// </summary>
class BaseEnemy
{
public:
	enum class EnemyPhase
	{
		SHOT,
		WAIT,

	};


	BaseEnemy() = default;
	virtual ~BaseEnemy() = default;

	virtual void Update() = 0;

	virtual void Draw() = 0;
};

