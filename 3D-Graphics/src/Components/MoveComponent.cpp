#include "MoveComponent.h"
#include "../Actor.h"
#include "../WindowSize.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	: Component(owner, updateOrder),
	mAngularSpeed(0.0f),
	mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltatime)
{
	if (!Math::NearZero(mAngularSpeed))
	{
		Quaternion rotation = mOwner->GetRotation();
		float angle = mAngularSpeed * deltatime;
		// Create quaternion for incremental rotation
		// (Rotate about up axis)
		Quaternion inc(Vector3::UnitZ, angle);

		//Concate old and new quarternion
		rotation = Quaternion::Concatenate(rotation, inc);

		mOwner->SetRotation(rotation);
	}

	// Update Position based on forward speed
	// Convert from angle to a forward vector
	if (!Math::NearZero(mForwardSpeed))
	{
		Vector3 position = mOwner->GetPosition();
		position += mOwner->GetForward() * mForwardSpeed * deltatime;

		mOwner->SetPosition(position);
	}
}
