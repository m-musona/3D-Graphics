#pragma once

#include "../Actor.h"

class Sphere : public Actor
{
public:
	Sphere(class Game* game);
	void UpdateActor(float deltaTime) override;

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

	TypeID GetType() const override { return TSphere; }

private:
	class MoveComponent* mMoveComp;
	float mForwardSpeed;
};