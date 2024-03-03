#include "BoxCollisionComponent.h"

#include "../Actor.h"
#include "../Game.h"
#include "../PhysWorld.h"

BoxCollisionComponent::BoxCollisionComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder),
	mObjectBox(Vector3::Zero, Vector3::Zero),
	mWorldBox(Vector3::Zero, Vector3::Zero),
	mShouldRotate(true)
{
	mOwner->GetGame()->GetPhysWorld()->AddBox(this);
}

BoxCollisionComponent::~BoxCollisionComponent()
{
	mOwner->GetGame()->GetPhysWorld()->RemoveBox(this);
}

void BoxCollisionComponent::OnUpdateWorldTransform()
{
	// Reset to object space box
	mWorldBox = mObjectBox;
	// Scale
	mWorldBox.mMin *= mOwner->GetScale();
	mWorldBox.mMax *= mOwner->GetScale();
	// Rotate (if we want to)
	if (mShouldRotate)
	{
		mWorldBox.Rotate(mOwner->GetRotation());
	}
	// Translate
	mWorldBox.mMin += mOwner->GetPosition();
	mWorldBox.mMax += mOwner->GetPosition();
}