#pragma once
#include <cstdint>
#include "../Actor.h"

class Component
{
public:
	// Constructor
	// (the lower the update order, the earlier the component updates)
	Component(class Actor* owner, int updateOrder = 100);

	// Destructor
	virtual ~Component();

	// Update this component by delta time
	virtual void Update(float deltatime);

	// Process input for this component
	virtual void ProcessInput(const uint8_t* keyState) {}

	// Called when world transform changes
	virtual void OnUpdateWorldTransform() {}

	class Actor* GetOwner() { return mOwner; }
	int GetUpdateOrder() const { return mUpdateOrder; }

protected:
	// Owning actor
	class Actor* mOwner;

	// Update order of Component
	int mUpdateOrder;
};