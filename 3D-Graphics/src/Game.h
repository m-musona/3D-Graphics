#pragma once

#include "SDL/SDL.h"

#include <vector>
#include <unordered_map>
#include <string>
#include "GameProgCpp/Math.h"
#include "Sound/SoundEvent.h"

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

	Uint32 mTicksCount;

	// Are we updating actors
	bool mUpdatingActors;
	// Game should continue to run
	bool mIsRunning;

	// Game-specific code
	class CameraActor* mCameraActor;
	class Sphere* mSphere;
	class Cube* mCube;
	class HealthBar* mHealthBar;
	class Radar* mRadar;

	SoundEvent mMusicEvent;
	SoundEvent mReverbSnap;
};