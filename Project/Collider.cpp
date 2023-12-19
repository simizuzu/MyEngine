#include "Collider.h"

void Collider::OnCollision()
{
	
}

void Collider::Initialize()
{
	worldTransform_.Initialize();
}

float Collider::GetRadius()
{
	return radius_;
}
void Collider::SetRadius(float radius)
{
	radius_ = radius;
}
