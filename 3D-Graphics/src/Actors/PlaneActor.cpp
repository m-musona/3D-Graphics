#include "PlaneActor.h"

#include "../Components/MeshComponent.h"
#include "../Game.h"
#include "../Renderer/Renderer.h"

PlaneActor::PlaneActor(Game* game)
	:Actor(game)
{
	SetScale(10.0f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh"));
}