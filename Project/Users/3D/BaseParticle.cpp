#include "BaseParticle.h"
#include <cassert>

/**
 * @file BaseParticle.cpp
 * @brief パーティクル基底クラス
 */

void BaseParticle::Initialize(ObjModel* model,Camera* camera)
{
	assert(model);
	assert(camera);
	model_ = model;
	camera_ = camera;
}

bool BaseParticle::GetIsDead() const
{
	return isDead;
}
