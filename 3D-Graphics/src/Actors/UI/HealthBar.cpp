#include "HealthBar.h"

#include "../Game.h"
#include "../Renderer/Renderer.h"
#include "../Components/SpriteComponent.h"

#include "../WindowSize.h"

HealthBar::HealthBar(Game* game)
	:Actor(game)
{
	SetPosition(Vector3(
		-screenWidth / 2 + 170.0f,
		-screenHeight / 2 + 40.0f,
		0.0f)
	);
	SpriteComponent* spriteComp = new SpriteComponent(this);
	spriteComp->SetTexture(GetGame()->GetRenderer()->GetTexture("Assets/HealthBar.png"));
}
