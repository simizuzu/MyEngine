#include "Collider.h"

void Collider::OnCollision()
{
	
}

float Collider::GetRadius()
{
	return radius_;
}
void Collider::SetRadius(float radius)
{
	radius_ = radius;
}

uint32_t Collider::GetCollisionAttribute()
{
	return collisionAttribute_;
}

void Collider::SetCollisionAttribute(uint32_t collisionAttribute)
{
	collisionAttribute_ = collisionAttribute;
}

uint32_t Collider::GetCollisionMask()
{
	return collisionMask_;
}

void Collider::SetCollisionMask(uint32_t collisionMask)
{
	collisionMask_ = collisionMask;
}