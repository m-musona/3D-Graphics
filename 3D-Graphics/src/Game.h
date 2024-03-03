#pragma once

#include "SDL/SDL.h"

#include <vector>
#include <unordered_map>
#include <string>
#include "GameProgCpp/Math.h"
#include "Sound/SoundEvent.h"
#include "Actors/PlaneActor.h"

class Game
{
public:
	Game();

	// Initialize the Game
	bool Initialize();
	// Runs the game loop until the game is over
	void RunLoop();
	// Shutdown the game
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class Renderer* GetRenderer() { return mRenderer; }
	class AudioSystem* GetAudioSystem() { return mAudioSystem; }
	class PhysWorld* GetPhysWorld() { return mPhysWorld; }

	// Game-specific
	void AddPlane(class PlaneActor* plane);
	void RemovePlane(class PlaneActor* plane);
	std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }

private:

	// Helper Functions for the game loop
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();



	// All actors in the game
	std::vector<class Actor*> mActors;
	// Pending actors
	std::vector<class Actor*> mPendingActors;

	class Renderer* mRenderer;
	class AudioSystem* mAudioSystem;
	class PhysWorld* mPhysWorld;

	Uint32 mTicksCount;

	// Are we updating actors
	bool mUpdatingActors;
	// Game should continue to run
	bool mIsRunning;

	// Game-specific code
	class FPSActor* mFPSActor;
	class FollowActor* mFollowActor;
	class OrbitActor* mOrbitActor;
	class SplineActor* mSplineActor;
	std::vector<class PlaneActor*> mPlanes;

	class Sphere* mSphere;
	class Cube* mCube;
	class AimingReticule* mAimingReticule;
	class SpriteComponent* mCrosshair;
	class HealthBar* mHealthBar;
	class Radar* mRadar;

	SoundEvent mMusicEvent;
	SoundEvent mReverbSnap;

	void ChangeCamera(int mode);
};