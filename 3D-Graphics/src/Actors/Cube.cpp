#include "Cube.h"

#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Components/MeshComponent.h"

Cube::Cube(Game* game)
	:Actor(game)
{
	Quaternion q(Vector3::UnitY, -Math::PiOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	SetRotation(q);
	SetPosition(Vector3(-200.0f, 475.0f, 0.0f));
	SetScale(100.0f);

	MeshComponent* meshComp = new MeshComponent(this);
	meshComp->SetMesh(GetGame()->GetRenderer()->GetMesh("Assets/Cube.gpmesh"));
}
