#pragma once

#include "../Components/CameraComponent.h"

class FPSCamera : public CameraComponent
{
public:
	FPSCamera(class Actor* owner);

	void Update(float deltaTime) override;

	float GetPitch() const { return mPitch; }
	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetMaxPitch() const { return mMaxPitch; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetMaxPitch(float pitch) { mMaxPitch = pitch; }

	TypeID GetType() const override { return TFPSCamera; }

private:
	// Rotation/sec speed of pitch
	float mPitchSpeed;
	// Maximum pitch deviation from forward
	float mMaxPitch;
	// Current pitch
	float mPitch;
};