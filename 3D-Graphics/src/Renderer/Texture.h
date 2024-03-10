#pragma once

#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();

	void SetActive(int index = 0);

	void CreateFromSurface(struct SDL_Surface* surface);
	void CreateForRendering(int width, int height, unsigned int format);

	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
	unsigned int GetTextureID() const { return mTextureID; }

	const std::string& GetFileName() const { return mFileName; }

private:
	std::string mFileName;
	// OpenGL ID of this texture
	unsigned int mTextureID;

	// Width/height of the texture
	int mWidth;
	int mHeight;
};