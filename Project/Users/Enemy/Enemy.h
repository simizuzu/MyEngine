#pragma once

#include "Camera.h"

/**
 * @class Enemy.h
 * @brief 敵キャラの基底クラス
 */

/// <summary>
/// エネミー
/// </summary>
class Enemy
{
protected:

	Enemy() = default;
	virtual ~Enemy() = default;

	virtual void Update() = 0;

	virtual void Draw() = 0;
};

