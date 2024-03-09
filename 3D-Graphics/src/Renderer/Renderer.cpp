#include "Renderer.h"

#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"

#include "../Components/SpriteComponent.h"
#include "../Components/MeshComponent.h"
#include "../Components/SkeletalMeshComponent.h"

#include "../UIScreen.h"
#include "../Game.h"

#include <algorithm>
#include <iostream>

#include "glad/glad.h"

Renderer::Renderer(Game* game)
	:mGame(game),
	mSpriteShader(nullptr),
	mMeshShader(nullptr),
	mSkinnedShader(nullptr)
{
}

Renderer::~Renderer()
{
}

bool Renderer::Initialize(float screenWidth, float screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	// Use Core Opengl profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Specify version 4.6
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	// Request a color buffer with 8-bits per RGBA channel
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Force OpenGL to use hardware acceleration(Run on GPU)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Create window
	mWindow = SDL_CreateWindow(
		"3D FPS Game", // Window Title
		0, // Top left x-coordinate of window
		0, // Top left y-coordinate of window
		static_cast<int>(screenWidth), // Width of window
		static_cast<int>(screenHeight), // Height of window
		SDL_WINDOW_OPENGL // Window for opengl usage
	);
	// Verify that window created succsefully
	if (!mWindow)
	{
		SDL_Log("Unable to create window: %s", SDL_GetError());
		return false;
	}

	// Create Opengle context
	mContext = SDL_GL_CreateContext(mWindow);
	// Check if context failed to be made
	if (mContext == NULL)
	{
		SDL_Log("Failed to Create Context: %s", SDL_GetError());
		return false;
	}

	// Initialize glad
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		SDL_Log("Failed to initialize Glad: %s", SDL_GetError());
		return false;
	}

	std::cout << std::left << "OpenGL Version: " << GLVersion.major << "." << GLVersion.minor << std::endl;
	std::cout << std::left << "OpenGL Shading Language Version: " << (char*)glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << std::left << "OpenGL Vendor: " << (char*)glGetString(GL_VENDOR) << std::endl;
	std::cout << std::left << "OpenGL Renderer: " << (char*)glGetString(GL_RENDERER) << std::endl;

	// Set OpenGL View Port
	// int ww, wh;
	// SDL_GetWindowSize(mWindow, &ww, &wh);
	// glViewport(0,0, ww, wh);

	// Make Sure we can create/compile shaders
	if (!LoadShaders())
	{
		SDL_Log("Failed to load shaders");
		return false;
	}

	// Create quad for drawing Sprites
	CreateSpriteVerts();

	return true;
}

void Renderer::Shutdown()
{
	delete mSpriteVerts;
	mSpriteShader->Unload();
	delete mSpriteShader;
	mMeshShader->Unload();
	delete mMeshShader;

	// Shutdown SDL Functions
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData()
{
	// Destroy textures
	for (auto i : mTextures)
	{
		i.second->Unload();
		delete i.second;
	}
	mTextures.clear();

	// Destroy meshes
	for (auto i : mMeshes)
	{
		i.second->Unload();
		delete i.second;
	}
	mMeshes.clear();
}

void Renderer::Draw()
{
	// Step 1: Clear the back buffer to a color

	// Set clear color to black
	glClearColor(0.0f /* R */, 0.0f /* G */, 0.0f /* B */, 1.0f /* A */);
	// Clear back buffer to current draw color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Step 2: Draw the entire game scene

	// Enable depth buffering/disable alpha blend
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	// Set the basic mesh shader active
	mMeshShader->SetActive();
	// Update view-projrction matrix
	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
	// Update lighting uniforms
	SetLightUniforms(mMeshShader);

	for (auto mc : mMeshComps)
	{
		if (mc->GetVisible())
		{
			mc->Draw(mMeshShader);
		}
	}

	// Draw any skinned meshes now
	mSkinnedShader->SetActive();
	// Update view-projection matrix
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);
	// Update lighting uniforms
	SetLightUniforms(mSkinnedShader);
	for (auto sk : mSkeletalMeshes)
	{
		if (sk->GetVisible())
		{
			sk->Draw(mSkinnedShader);
		}
	}

	// Disable depth buffering
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(
		GL_SRC_ALPHA,			// srcFactor is srcAlpha
		GL_ONE_MINUS_SRC_ALPHA,	// dstFactor is 1 - srcAlpha
		GL_ONE,
		GL_ZERO
	);

	// Set sprite shader and vertex array objects active
	mSpriteShader->SetActive();
	mSpriteVerts->SetActive();

	// Loop over the vector of sprite components and call Draw on each
	for (auto sprite : mSprites)
	{
		if (sprite->GetVisible())
		{
			sprite->Draw(mSpriteShader);
		}
	}

	// Draw any UI screens
	for (auto ui : mGame->GetUIStack())
	{
		ui->Draw(mSpriteShader);
	}

	//Step 3: Swap the front buffer and back buffer
	SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite)
{
	// Find the insertion point in the sorted vector
	// (The first element with a higher draw order than me)
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	// Inserts element before position of iterator
	mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite)
{
	// (We can't swap because it ruins ordering)
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

// Image loading Process
Texture* Renderer::GetTexture(const std::string& filename)
{
	Texture* texture = nullptr;

	// Check if the texture is already in the map?
	auto iter = mTextures.find(filename);
	if (iter != mTextures.end())
	{
		texture = iter->second;
	}
	else
	{
		texture = new Texture();

		// Loads an image from file
		if (texture->Load(filename))
		{
			mTextures.emplace(filename, texture);
		}
		else
		{
			delete texture;
			texture = nullptr;
		}
	}
	return texture;
}

Mesh* Renderer::GetMesh(const std::string& fileName)
{
	Mesh* m = nullptr;
	auto iter = mMeshes.find(fileName);
	if (iter != mMeshes.end())
	{
		m = iter->second;
	}
	else
	{
		m = new Mesh();
		if (m->Load(fileName, this))
		{
			mMeshes.emplace(fileName, m);
		}
		else
		{
			delete m;
			m = nullptr;
		}
	}
	return m;
}

void Renderer::AddMeshComp(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		mSkeletalMeshes.emplace_back(sk);
	}
	else
	{
		mMeshComps.emplace_back(mesh);
	}
}

void Renderer::RemoveMeshComp(MeshComponent* mesh)
{
	if (mesh->GetIsSkeletal())
	{
		SkeletalMeshComponent* sk = static_cast<SkeletalMeshComponent*>(mesh);
		auto iter = std::find(mSkeletalMeshes.begin(), mSkeletalMeshes.end(), sk);
		mSkeletalMeshes.erase(iter);
	}
	else
	{
		auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
		mMeshComps.erase(iter);
	}
}

void Renderer::SetLightUniforms(Shader* shader)
{
	// Camera position is from inverted view
	Matrix4 invView = mView;
	invView.Invert();
	shader->SetVectorUniform("uCameraPos", invView.GetTranslation());

	// Ambient Light
	shader->SetVectorUniform("uAmbientLight", mAmbientLight);

	// Directional light
	shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
	shader->SetVectorUniform("uDirLight.mDiffuseColor", mDirLight.mDiffuseColor);
	shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}

bool Renderer::LoadShaders()
{
	// Create Sprite shader
	mSpriteShader = new Shader();
	if (!mSpriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
	{
		return false;
	}

	mSpriteShader->SetActive();

	// Set the view-projection matrix
	Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
	mSpriteShader->SetMatrixUniform("uViewProj", viewProj);

	// Create basic mesh shader
	mMeshShader = new Shader();

	if (!mMeshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
	{
		return false;
	}

	mMeshShader->SetActive();

	// Set the view-projection matrix
	mView = Matrix4::CreateLookAt(
		Vector3::Zero, // Camera Position
		Vector3::UnitX, // target Position
		Vector3::UnitZ // Up
	);

	mProjection = Matrix4::CreatePerspectiveFOV(
		Math::ToRadians(70.0f), // Horizontal FOV
		mScreenWidth, // Width of view
		mScreenHeight, // Height of view
		25.0f, // Near plane distance
		10000.0f // Far plane distance
	);

	mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);

	// Create skinned shader
	mSkinnedShader = new Shader();
	if (!mSkinnedShader->Load("Shaders/Skinned.vert", "Shaders/Phong.frag"))
	{
		return false;
	}

	mSkinnedShader->SetActive();
	// Set the view-projection matrix
	mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	mProjection = Matrix4::CreatePerspectiveFOV(
		Math::ToRadians(70.0f),
		mScreenWidth, 
		mScreenHeight, 
		10.0f, 
		10000.0f
	);
	mSkinnedShader->SetMatrixUniform("uViewProj", mView * mProjection);

	return true;
}

void Renderer::CreateSpriteVerts()
{

	float vertices[] = {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
	};


	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	mSpriteVerts = new VertexArray(vertices, 4, VertexArray::PosNormTex, indices, 6);
}

Vector3 Renderer::Unproject(const Vector3& screenPoint) const
{
	// Convert screenPoint to device coordinates (between -1 and +1)
	Vector3 deviceCoord = screenPoint;
	deviceCoord.x /= (mScreenWidth) * 0.5f;
	deviceCoord.y /= (mScreenHeight) * 0.5f;

	// Transform vector by unprojection matrix
	Matrix4 unprojection = mView * mProjection;
	unprojection.Invert();
	return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

void Renderer::GetScreenDirection(Vector3& outStart, Vector3& outDir) const
{
	// Get start point (in center of screen on near plane)
	Vector3 screenPoint(0.0f, 0.0f, 0.0f);
	outStart = Unproject(screenPoint);
	// Get end point (in center of screen, between near and far)
	screenPoint.z = 0.9f;
	Vector3 end = Unproject(screenPoint);
	// Get direction vector
	outDir = end - outStart;
	outDir.Normalize();
}