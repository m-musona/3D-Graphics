#pragma once
#include "../Actor.h"

class PlaneActor : public Actor
{
public:
	PlaneActor(class Game* game);
	~PlaneActor();
	class BoxCollisionComponent* GetBox() { return mBox; }

	TypeID GetType() const override { return TPlaneActor; }

private:
	class BoxCollisionComponent* mBox;
};