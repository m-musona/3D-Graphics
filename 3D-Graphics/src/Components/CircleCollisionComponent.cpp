#include "CircleCollisionComponent.h"
#include "../Actor.h"

CircleCollisionComponent::CircleCollisionComponent(class Actor* owner)
	:Component(owner),
	mRadius(0.0f)
{
}

float CircleCollisionComponent::GetRadius() const
{
	return mOwner->GetScale() * mRadius;
}

const Vector3& CircleCollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

bool Intersect(const CircleCollisionComponent& a, const CircleCollisionComponent& b)
{
	// Calculate distance squared
	Vector3 diff = a.GetCenter() - b.GetCenter();
	float distSq = diff.LengthSq();

	// Calculate sum of radii squared
	float radiiSq = a.GetRadius() + b.GetRadius();
	radiiSq *= radiiSq;

	return distSq <= radiiSq;
}
