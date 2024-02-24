#pragma once
#include "../Component.h"
#include "SDL/SDL.h"

class SpriteComponent : public Component
{
public:
	// (Lower draw order corresponds with further back)
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(class Shader* shader);

	virtual void SetTexture(class Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTextureHeight() const { return mTextureHeight; }
	int GetTextureWidth() const { return mTextureWidth; }
protected:
	// Texture to draw
	class Texture* mTexture;

	// Draw order used for painter's algorithm
	int mDrawOrder;
	
	// WIdth/Height of texture
	int mTextureHeight;
	int mTextureWidth;
};
