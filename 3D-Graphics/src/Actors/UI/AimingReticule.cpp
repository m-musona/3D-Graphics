#include "AimingReticule.h"

#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Components/SpriteComponent.h"

AimingReticule::AimingReticule(Game* game)
	:Actor(game)
{
	SetPosition(Vector3(0.0f,0.0f,0.0f));
	SpriteComponent* spriteComp = new SpriteComponent(this);
	spriteComp->SetTexture(GetGame()->GetRenderer()->GetTexture("Assets/Crosshair.png"));
}
