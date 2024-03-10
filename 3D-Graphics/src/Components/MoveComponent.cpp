#include "MoveComponent.h"

#include "../Actor.h"
#include "../LevelLoader.h"

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
	if (!Math::NearZero(mForwardSpeed) || !Math::NearZero(mStrafeSpeed))
	{
		Vector3 position = mOwner->GetPosition();
		position += mOwner->GetForward() * mForwardSpeed * deltatime;
		// Update position based on strafe
		position += mOwner->GetRight() * mStrafeSpeed * deltatime;

		mOwner->SetPosition(position);
	}
}

void MoveComponent::LoadProperties(const rapidjson::Value& inObj)
{
	Component::LoadProperties(inObj);

	JsonHelper::GetFloat(inObj, "angularSpeed", mAngularSpeed);
	JsonHelper::GetFloat(inObj, "forwardSpeed", mForwardSpeed);
	JsonHelper::GetFloat(inObj, "strafeSpeed", mStrafeSpeed);
}

void MoveComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	Component::SaveProperties(alloc, inObj);

	JsonHelper::AddFloat(alloc, inObj, "angularSpeed", mAngularSpeed);
	JsonHelper::AddFloat(alloc, inObj, "forwardSpeed", mForwardSpeed);
	JsonHelper::AddFloat(alloc, inObj, "strafeSpeed", mStrafeSpeed);
}
