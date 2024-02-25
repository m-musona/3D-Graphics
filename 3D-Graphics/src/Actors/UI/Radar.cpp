#include "Radar.h"

#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Components/SpriteComponent.h"

#include "../WindowSize.h"

Radar::Radar(Game* game)
	:Actor(game)
{
	SetPosition(Vector3(
		screenWidth / 2 - 175.0f,
		-screenHeight / 2 + 150.0f,
		0.0f)
	);
	SpriteComponent* spriteComp = new SpriteComponent(this);
	spriteComp->SetTexture(GetGame()->GetRenderer()->GetTexture("Assets/Radar.png"));
}
