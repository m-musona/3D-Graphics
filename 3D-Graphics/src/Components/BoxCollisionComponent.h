#pragma once

#include "../Component.h"
#include "../Collision.h"

class BoxCollisionComponent : public Component
{
public:
	BoxCollisionComponent(class Actor* owner, int updateOrder = 100);
	~BoxCollisionComponent();

	void OnUpdateWorldTransform() override;

	void SetObjectBox(const AxisAlignedBoundingBox& model) { mObjectBox = model; }
	const AxisAlignedBoundingBox& GetWorldBox() const { return mWorldBox; }

	void SetShouldRotate(bool value) { mShouldRotate = value; }
private:
	AxisAlignedBoundingBox mObjectBox;
	AxisAlignedBoundingBox mWorldBox;
	bool mShouldRotate;
};