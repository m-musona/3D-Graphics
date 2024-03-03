#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "SDL/SDL.h"
#include "GameProgCpp/Math.h"

struct DirectionalLight
{
	// Direction of light
	Vector3 mDirection;
	// Diffuse color
	Vector3 mDiffuseColor;
	// Specular color
	Vector3 mSpecColor;
};

class Renderer
{
public:
	Renderer(class Game* game);
	~Renderer();

	// Initialize and shutdown renderer
	bool Initialize(float screenWidth, float screenHeight);
	void Shutdown();

	// Unload all textures/meshes
	void UnloadData();

	// Draw the frame
	void Draw();

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);

	void AddMeshComp(class MeshComponent* mesh);
	void RemoveMeshComp(class MeshComponent* mesh);

	void SetViewMatrix(const Matrix4& view) { mView = view; }

	void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
	DirectionalLight& GetDirectionalLight() { return mDirLight; }

	// Given a screen space point, unprojects it into world space,
	// based on the current 3D view/projection matrices
	// Expected ranges:
	// x = [-screenWidth/2, +screenWidth/2]
	// y = [-screenHeight/2, +screenHeight/2]
	// z = [0, 1) -- 0 is closer to camera, 1 is further
	Vector3 Unproject(const Vector3& screenPoint) const;
	void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;

	float GetScreenWidth() const { return mScreenWidth; }
	float GetScreenHeight() const { return mScreenHeight; }

private:
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader);

	// Screen Height/Width
	float mScreenHeight;
	float mScreenWidth;

	// Map of textures loaded
	std::unordered_map<std::string, class Texture*> mTextures;

	// Map of meshes loaded
	std::unordered_map<std::string, class Mesh*> mMeshes;

	// All the sprites drawn
	std::vector<class SpriteComponent*> mSprites;

	// All mesh components drawn
	std::vector<class MeshComponent*> mMeshComps;

	// Sprite shader
	class Shader* mSpriteShader;
	// Sprite vertex array
	class VertexArray* mSpriteVerts;

	// Mesh shader
	class Shader* mMeshShader;

	// View/projection for 3D shaders
	Matrix4 mView;
	Matrix4 mProjection;

	// Lighting data
	Vector3 mAmbientLight;
	DirectionalLight mDirLight;

	// Window Created by SDL
	SDL_Window* mWindow;
	// OpenGL Context
	SDL_GLContext mContext;

	// Game
	class Game* mGame;
};