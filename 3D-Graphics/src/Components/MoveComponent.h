#pragma once
#include "../Component.h"

class MoveComponent : public Component
{
public:
	// Lower Update order to update first
	MoveComponent(class Actor* owner, int updateOrder = 10);

	void Update(float deltatime) override;

	// Getters/Setters
	float GetAngularSpeed() const { return mAngularSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }

private:
	// Controls Forward Movement (units/seconds)
	float mForwardSpeed;
	// Controls Rotation Movement (radians/seconds)
	float mAngularSpeed;
};
