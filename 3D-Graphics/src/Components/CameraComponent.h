#pragma once

#include "../Component.h"
#include "GameProgCpp/Math.h"

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder = 200);
protected:
	void SetViewMatrix(const Matrix4& view);
};
