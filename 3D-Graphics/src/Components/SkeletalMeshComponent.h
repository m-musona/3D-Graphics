#pragma once

#include "MeshComponent.h"
#include "../MatrixPalette.h"

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* owner);

	// Draw this mesh component
	void Draw(class Shader* shader) override;

	void Update(float deltaTime) override;

	// Setters
	void SetSkeleton(const class Skeleton* sk) { mSkeleton = sk; }

	// Play an animation. Returns the length of the animation
	float PlayAnimation(const class Animation* anim, float playRate = 1.0f);

private:
	void ComputeMatrixPalette();

	const class Skeleton* mSkeleton;

	// Matrix palette
	MatrixPalette mPalette;
	// Animation currently playing
	const class Animation* mAnimation;
	// Play rate of animation (1.0 is normal speed)
	float mAnimPlayRate;
	// Current time in the animation
	float mAnimTime;

};