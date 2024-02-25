#pragma once

#include "../Actor.h"

class Sphere : public Actor
{
public:
	Sphere(class Game* game);
	void UpdateActor(float deltaTime) override;
private:
	class MoveComponent* mMoveComp;
	float mForwardSpeed;
};