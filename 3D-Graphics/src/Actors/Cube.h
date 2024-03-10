#pragma once

#include "../Actor.h"

class Cube : public Actor
{
public:
	Cube(class Game* game);

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

	TypeID GetType() const override { return TCube; }
};