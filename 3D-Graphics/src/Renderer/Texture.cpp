#include "Texture.h"

#include "STB_Image/stb_image.h"
#include "glad/glad.h"
#include "SDL/SDL.h"

#include <iostream>

Texture::Texture()
	:mTextureID(0),
	mWidth(0),
	mHeight(0)
{
}

Texture::~Texture()
{
}

bool Texture::Load(const std::string& fileName)
{
	mFileName = fileName;
	int channels = 0;

	stbi_set_flip_vertically_on_load(0);

	unsigned char* image = stbi_load(
		fileName.c_str(),
		&mWidth,
		&mHeight,
		&channels,
		4
	);

	//unsigned char* image = SOIL_load_image(
	//	fileName.c_str(),	// Name of file
	//	&mWidth,			// Stores Width
	//	&mHeight,			// Stores Height
	//	&channels,			// Stores number of channels
	//	SOIL_LOAD_AUTO		// Type of image file, or auto for any
	//);

	if (image == nullptr)
	{
		SDL_Log("STBI failed to load image %s: %s", fileName.c_str(), stbi_failure_reason());
		return false;
	}

	// If Image is RGB or RGBA
	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	// Create OpenGL texture object
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// Copy raw image texture data into texture object
	glTexImage2D(
		GL_TEXTURE_2D,		// Texture target
		0,					// Level of detail (for now, assume 0)
		format,				// Color format OpenGL should use
		mWidth,				// Width of texture
		mHeight,			// Height of texture
		0,					// Border - "this value must be 0"
		format,				// Color format of input data
		GL_UNSIGNED_BYTE,	// Bit depth of input data / Unsigned byte specifies 8-bit channels
		image				// Pointer to image data
	);

	// Free image from memory
	stbi_image_free(image);

	// Generate mipmaps for texture
	glGenerateMipmap(GL_TEXTURE_2D);

	// Enable linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Enable anisotropic filtering, if supported
	if (GLAD_GL_EXT_texture_filter_anisotropic)
	{
		// Get the maximum anisotropy value
		GLfloat largest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
		// Enable it
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
	}

	return true;
}

void Texture::CreateFromSurface(SDL_Surface* surface)
{
	mWidth = surface->w;
	mHeight = surface->h;

	// Generate a GL texture
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_RGBA, 
		mWidth, 
		mHeight, 
		0, 
		GL_BGRA,
		GL_UNSIGNED_BYTE, 
		surface->pixels
	);

	// Use linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::CreateForRendering(int width, int height, unsigned int format)
{
	mWidth = width;
	mHeight = height;

	// Create the texture id
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// Set the image width/height with null initial data
	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, GL_RGB,
		GL_FLOAT, nullptr);

	// For a texture we'll render to, just use nearest neighbor
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive(int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

