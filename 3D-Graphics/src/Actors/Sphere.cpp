#include "Sphere.h"

#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Components/MeshComponent.h"
#include "../Components/AudioComponent.h"
#include "../Components/MoveComponent.h"

float speed = 300.0f;

Sphere::Sphere(Game* game)
	:Actor(game)
{
	SetPosition(Vector3(600.0f, -675.0f, 0.0f));
	SetScale(3.0f);

	MeshComponent* meshComp = new MeshComponent(this);
	meshComp->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Sphere.gpmesh"));

	AudioComponent* audioComp = new AudioComponent(this);
	audioComp->PlayEvent("event:/FireLoop");

	mMoveComp = new MoveComponent(this);

	mForwardSpeed += speed;
}

void Sphere::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	
	if (GetPosition().x >= 1000)
	{
		mForwardSpeed -= speed;
	}
	else if (GetPosition().x <= -600)
	{
		mForwardSpeed += speed;
	}
	mMoveComp->SetForwardSpeed(mForwardSpeed);
}
