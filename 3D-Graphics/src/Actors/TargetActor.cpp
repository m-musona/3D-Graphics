#include "TargetActor.h"

#include "../Game.h"

#include "../Renderer/Renderer.h"
#include "../Renderer/Mesh.h"

#include "../Components/MeshComponent.h"
#include "../Components/BoxCollisionComponent.h"
#include "../Components/TargetComponent.h"

TargetActor::TargetActor(Game* game)
	:Actor(game)
{
	//SetScale(10.0f);
	SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));

	MeshComponent* mc = new MeshComponent(this);
	Mesh* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Target.gpmesh");
	mc->SetMesh(mesh);

	// Add collision box
	BoxCollisionComponent* bc = new BoxCollisionComponent(this);
	bc->SetObjectBox(mesh->GetBox());

	new TargetComponent(this);

}
