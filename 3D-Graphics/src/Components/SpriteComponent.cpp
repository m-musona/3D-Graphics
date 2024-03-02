#include "SpriteComponent.h"

#include "../Actor.h"
#include "../Game.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Renderer.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	: Component(owner), 
	mTexture(nullptr), 
	mDrawOrder(drawOrder), 
	mTextureWidth(0), 
	mTextureHeight(0),
	mVisible(true)
{
	mOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{
	if (mTexture)
	{
		// Scale the quad by the width/height of texture
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTextureWidth),
			static_cast<float>(mTextureHeight),
			1.0f);

		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		// Since all sprites use the same shader/vertices,
		// the game first sets them active before any sprite draws

		// Set world transform
		shader->SetMatrixUniform("uWorldTransform", world);

		// Set current texture
		mTexture->SetActive();

		// Draw quad
		glDrawElements(
			GL_TRIANGLES,		// Type of polygon/primitive to draw
			6,					// Number of indices in index buffer
			GL_UNSIGNED_INT,	// Type of each index
			nullptr				// Usually nullptr
		);
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	
	// Set width/height of texture
	mTextureWidth = texture->GetWidth();
	mTextureHeight = texture->GetHeight();
}
