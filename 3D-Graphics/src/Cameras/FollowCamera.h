#pragma once

#include "../Components/CameraComponent.h"

class FollowCamera : public CameraComponent
{
public:
	FollowCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }

	TypeID GetType() const override { return TFollowCamera; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

private:
	Vector3 ComputeCameraPos() const;

	// Actual position of camera
	Vector3 mActualPos;
	// Velocity of actual camera
	Vector3 mVelocity;
	// Horizontal follow distance
	float mHorzDist;
	// Vertical follow distance
	float mVertDist;
	// Target distance
	float mTargetDist;
	// Spring constant (higher is more stiff)
	float mSpringConstant;
};