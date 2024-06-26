#pragma once

#include "../Actor.h"
#include "../Sound/SoundEvent.h"

class FPSActor : public Actor
{
public:
	FPSActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;

	void Shoot();

	void SetFootstepSurface(float value);

	void SetVisible(bool visible);
	void FixCollisions();

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

	TypeID GetType() const override { return TFPSActor; }
private:
	class MoveComponent* mMoveComp;
	class MeshComponent* mMeshComp;
	class BoxCollisionComponent* mBoxCollComp;
	class Actor* mFPSModel;

	class FPSCamera* mCameraComp;
	class AudioComponent* mAudioComp;
	SoundEvent mFootstep;
	float mLastFootstep;
};