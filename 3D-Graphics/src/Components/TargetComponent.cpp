#include "TargetComponent.h"

#include "BoxCollisionComponent.h"
#include "../Actor.h"
#include "../Game.h"
#include "../PhysWorld.h"
#include "../Game.h"

#include "../UI/HUD.h"

#include "GameProgCpp/Math.h"

TargetComponent::TargetComponent(Actor * owner)
	:Component(owner)
{
	mOwner->GetGame()->GetHUD()->AddTargetComponent(this);
}

TargetComponent::~TargetComponent()
{
	mOwner->GetGame()->GetHUD()->RemoveTargetComponent(this);
}
