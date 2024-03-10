#pragma once

#include "../Actor.h"

class OrbitActor : public Actor
{
public:
	OrbitActor(class Game* game);

	void ActorInput(const uint8_t* keys) override;

	void SetVisible(bool visible);

	TypeID GetType() const override { return TOrbitActor; }
private:
	class OrbitCamera* mCameraComp;
	class MeshComponent* mMeshComp;
};