#pragma once
#include "TextLabel.h"
#include "Texture.h"
#include "Object.h"
#include "clock.h"
#include "Pill.h"
#include "Boid.h"

#include <fmod.hpp>


enum GameState
{
	GAME_MENU = 0,
	GAME_PLAY = 1,
	GAME_OVER = 2,
};

enum GameplayState
{
	PLAY_SEEK = 0,
	PLAY_ARRIVE = 1,
	PLAY_FOLLOWPATH = 2,
	PLAY_FLOCK = 3
};

class GameManager
{
public:
	GameManager();
	~GameManager();

	//void AudioInitialise();

	void Update(int _mousePosX, int _mousePosY);
	void Render();

	bool& GetContainment() { return m_containment; }
	std::vector<Boid>& GetBoids();
	Boid MakeBoid();

	TextLabel* m_boidStateText = nullptr;
	TextLabel* m_containmentStateText = nullptr;

	inline static GameState m_gameState = GAME_MENU;
	inline static GameplayState m_gameplayState = PLAY_SEEK;

private:
	//Game variables
	unsigned int m_gameScore = 0;
	
	//Clock
	CClock m_clock;
	double m_pillSpawnTimer = 0.0;

	//Camera
	Camera* m_camera = nullptr;

	///Objects
	//Boids
	bool m_containment = false;
	int m_boidIDCounter = 0;
	std::vector<Boid> m_boids;
	Mesh* m_boidMesh = nullptr;

	//Game Background
	Object m_backgroundObject;
	Mesh* m_backgroundMesh = nullptr;
	Texture* m_backgroundTexture = nullptr;

	//Default shader
	Shader* m_defaultShader = nullptr;

	//Text
	TextLabel* m_menuTitleText = nullptr;
	TextLabel* m_menuInstructText = nullptr;
	

	//Audio
	inline static FMOD::System* m_audioSystem = nullptr;
	inline static FMOD::Sound* m_yeatSound = nullptr;
	inline static FMOD::Sound* m_shoopSound = nullptr;
	inline static FMOD::Sound* m_trackBackground = nullptr;

	//Uniforms
	GLfloat u_currentTime = 0;
	int u_frameNum = 0;
};

