#include "BaseBullet.h"

bool BaseBullet::IsDead() const
{
    return isDead_;
}

void BaseBullet::OnDead()
{
	isDead_ = true;
}
