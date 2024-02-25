#include "Game.h"

#include "Actor.h"
#include "Actors/PlaneActor.h"
#include "Actors/CameraActor.h"
#include "Actors/Sphere.h"
#include "Actors/Cube.h"
#include "Actors/UI/HealthBar.h"
#include "Actors/UI/Radar.h"

#include "Components/MeshComponent.h"
#include "Components/SpriteComponent.h"'

#include "Renderer/Texture.h"
#include "Renderer/Renderer.h"

#include "Systems/AudioSystem.h"

#include <algorithm>
#include <iostream>
#include <string>

#include "SDL/SDL_image.h"
#include "glad/glad.h"


#include "WindowSize.h"

Game::Game()
	:mRenderer(nullptr),
	mAudioSystem(nullptr),
	mIsRunning(true), 
	mUpdatingActors(false)
{
}

bool Game::Initialize()
{
	// Iniotialize SDL library
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	// If sdl initialization fails return false
	if (sdlResult != 0)
	{
		// SDL Log is a way to output things to the console in SDL
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create the renderer
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(screenWidth, screenHeight))
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	// Create the audio system
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}
	
	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	// Because ~Actor calls RemoveActor, use a different style loop
	UnloadData();
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
	if (mAudioSystem)
	{
		mAudioSystem->Shutdown();
	}
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;

	// While there still events in the queue
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// Handle different event types here

		case SDL_QUIT:
			mIsRunning = false;
			break;

		// This fires when a key's initially pressed
		case SDL_KEYDOWN:
			if (!event.key.repeat)
			{
				HandleKeyPress(event.key.keysym.sym);
			}
			break;

		default:
			break;

		}
	}

	// Get State of Keyboard
	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	// If escape is pressed, also end loop
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}

void Game::HandleKeyPress(int key)
{
	switch (key)
	{
	case '-':
	{
		// Reduce master volume
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	case '=':
	{
		// Increase master volume
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Min(1.0f, volume + 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	case 'e':
		// Play explosion
		mAudioSystem->PlayEvent("event:/Explosion2D");
		break;
	case 'p':
		// Toggle music pause state
		mMusicEvent.SetPaused(!mMusicEvent.GetPaused());
		break;
	case 'r':
		// Stop or start reverb snapshot
		if (!mReverbSnap.IsValid())
		{
			mReverbSnap = mAudioSystem->PlayEvent("snapshot:/WithReverb");
		}
		else
		{
			mReverbSnap.Stop();
		}
		break;
	case '1':
		// Set default footstep surface
		mCameraActor->SetFootstepSurface(0.0f);
		break;
	case '2':
		// Set grass footstep surface
		mCameraActor->SetFootstepSurface(0.5f);
		break;
	default:
		break;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// Delta time is the difference in ticks from the last frame (converted to seconds)
	float deltatime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// Clamp maximum delta time value
	if (deltatime > 0.05f)
	{
		deltatime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update all Actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltatime);
	}
	mUpdatingActors = false;

	// move any pending actors to mActors
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();

		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Add any dead actors to a temp vector
	std::vector<Actor*> deadActors;

	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// Delete dead actors (which removes them from mActors)
	for (auto actor : deadActors)
	{
		delete actor;
	}

	// Update audio system
	mAudioSystem->Update(deltatime);

}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	// Create actors
	Actor* a = new Actor(this);

	// Setup floor
	const float start = -1250.0f;
	const float size = 250.0f;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -200.0f));
		}
	}

	// Left/right walls
	Quaternion q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	// Forward/back walls
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q);
	}

	// Setup lights
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// Camera actor
	mCameraActor = new CameraActor(this);
	// Sphere actor
	mSphere = new Sphere(this);
	// Cube actor
	mCube = new Cube(this);

	// UI ELEMENTS
	// Health Bar
	mHealthBar = new HealthBar(this);
	// Rader
	mRadar = new Radar(this);

	// Start music
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music");
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	if (mRenderer)
	{
		mRenderer->UnloadData();
	}
}

void Game::AddActor(Actor* actor)
{
	// if updating actors need to add to pending Actors
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Is it in pending actors?
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}