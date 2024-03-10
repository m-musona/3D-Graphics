#pragma once

#include "MoveComponent.h"

class BallMove : public MoveComponent
{
public:
	BallMove(class Actor* owner);

	void SetPlayer(Actor* player) { mPlayer = player; }
	void Update(float deltaTime) override;

	TypeID GetType() const override { return TBallMove; }
protected:
	class Actor* mPlayer;
};