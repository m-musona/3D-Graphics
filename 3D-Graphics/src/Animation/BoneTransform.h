#pragma once

#include "GameProgCpp/Math.h"

class BoneTransform
{
public:
	// For now, just make this data public
	Quaternion mRotation;
	Vector3 mTranslation;

	// Convert to matrix
	Matrix4 ToMatrix() const;

	static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};