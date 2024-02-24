#pragma once
#include "../Component.h"
#include "GameProgCpp/Math.h"

class CircleCollisionComponent : public Component
{
public:
	CircleCollisionComponent(class Actor* owner);

	void SetRadius(float radius) { mRadius = radius; }
	float GetRadius() const;

	const Vector3& GetCenter() const;

private:
	float mRadius;

};
bool Intersect(const CircleCollisionComponent& a, const CircleCollisionComponent& b);