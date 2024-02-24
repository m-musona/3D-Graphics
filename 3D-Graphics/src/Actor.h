#pragma once

#include <vector>
#include "GameProgCpp/Math.h"
#include <cstdint>

class Actor
{
public:
	// Used to track state of actor
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	// Constructor/ Destructor
	Actor(class Game* game);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltatime);

	// Updates all components attached to the actor (not overridable)
	void UpdateComponents(float deltatime);

	// Any actor-specific update code (overridable)
	virtual void UpdateActor(float deltatime);

	// Process Input function called from Game (not overridable)
	void ProcessInput(const uint8_t* keyState);

	// Any Actor Specific Input Code (overridable)
	virtual void ActorInput(const uint8_t* keyState);

	// Getters/Setters
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	Quaternion GetRotation() const { return mRotation; }
	void SetRotation(Quaternion rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }

	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }

	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }


	// Add/Remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// Actor's State
	State mState;

	// Transform
	Matrix4 mWorldTransform;
	Vector3 mPosition; // Center position of actor
	Quaternion mRotation; // Rotation angle in radians
	float mScale; // uniforms Scale of actor (1.0f for 100%)
	bool mRecomputeWorldTransform;

	// Components held by this Actor
	std::vector<class Component*> mComponents;

	class Game* mGame;
};