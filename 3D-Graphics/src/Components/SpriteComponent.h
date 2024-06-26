#pragma once
#include "../Component.h"
#include "SDL/SDL.h"
// #include "../Renderer/Texture.h"

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

	void SetVisible(bool visible) { mVisible = visible; }
	bool GetVisible() const { return mVisible; }

	TypeID GetType() const override { return TSpriteComponent; }

	void LoadProperties(const rapidjson::Value& inObj) override;
	void SaveProperties(rapidjson::Document::AllocatorType& alloc,
		rapidjson::Value& inObj) const override;

protected:
	// Texture to draw
	class Texture* mTexture;

	// Draw order used for painter's algorithm
	int mDrawOrder;

	// WIdth/Height of texture
	int mTextureHeight;
	int mTextureWidth;
	bool mVisible;
};
