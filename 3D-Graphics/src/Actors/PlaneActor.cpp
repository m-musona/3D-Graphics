#include "PlaneActor.h"

#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/Mesh.h"

#include "../Components/MeshComponent.h"
#include "../Components/BoxCollisionComponent.h"


PlaneActor::PlaneActor(Game* game)
	:Actor(game)
{
	SetScale(10.0f);
	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh");
	mc->SetMesh(mesh);
	// Add collision box
	mBox = new BoxCollisionComponent(this);
	mBox->SetObjectBox(mesh->GetBox());

	game->AddPlane(this);
}

PlaneActor::~PlaneActor()
{
	GetGame()->RemovePlane(this);
}
