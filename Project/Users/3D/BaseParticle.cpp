#include "BaseParticle.h"
#include <cassert>

/**
 * @file BaseParticle.cpp
 * @brief パーティクル基底クラス
 */

bool BaseParticle::GetIsDead() const
{
	return isDead;
}
