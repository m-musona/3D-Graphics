#include "Texture.h"

// #include "SOIL/SOIL.h"
#include "STB_Image/stb_image.h"
#include "glad/glad.h"
#include "SDL/SDL.h"

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
	int channels = 0;

	stbi_set_flip_vertically_on_load(1);

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

	// Enable bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

