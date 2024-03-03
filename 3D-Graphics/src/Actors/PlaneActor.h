#pragma once
#include "../Actor.h"

class PlaneActor : public Actor
{
public:
	PlaneActor(class Game* game);
	~PlaneActor();
	class BoxCollisionComponent* GetBox() { return mBox; }
private:
	class BoxCollisionComponent* mBox;
};