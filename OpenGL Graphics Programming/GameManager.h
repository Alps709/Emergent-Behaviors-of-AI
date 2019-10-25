#pragma once
#include "TextLabel.h"
#include "Texture.h"
#include "Object.h"
#include "clock.h"
#include "Pill.h"
#include "Snek.h"

#include <fmod.hpp>


enum GameState
{
	GAME_MENU = 0,
	GAME_PLAY = 1,
	GAME_OVER = 2,
};

class GameManager
{
public:
	GameManager();
	~GameManager();

	void AudioInitialise();

	void ProcessPills();

	void Update(int _mousePosX, int _mousePosY);
	void Render();

	inline static GameState m_gameState = GAME_MENU;

private:
	//Game variables
	unsigned int m_gameScore = 0;
	

	//Clock
	CClock m_clock;
	double m_pillSpawnTimer = 0.0;

	//Camera
	Camera* m_camera = nullptr;

	///Objects
	//Snek
	Snek* m_snek = nullptr;

	//Pills that the snek will collect for points
	std::vector<Pill> m_pillObjects;
	Mesh* m_pillMesh = nullptr;
	Texture* m_pillTexture = nullptr;

	//Game Background
	Object m_backgroundObject;
	Mesh* m_backgroundMesh = nullptr;
	Texture* m_backgroundTexture = nullptr;

	//Default shader
	Shader* m_defaultShader = nullptr;

	//Text
	TextLabel* m_menuTitleText = nullptr;
	TextLabel* m_menuInstructText = nullptr;
	TextLabel* m_overText = nullptr;
	TextLabel* m_overScoreText = nullptr;
	TextLabel* m_scoreText = nullptr;
	TextLabel* m_timeText = nullptr;

	//Audio
	inline static FMOD::System* m_audioSystem = nullptr;
	inline static FMOD::Sound* m_yeatSound = nullptr;
	inline static FMOD::Sound* m_shoopSound = nullptr;
	inline static FMOD::Sound* m_trackBackground = nullptr;

	//Uniforms
	GLfloat u_currentTime = 0;
	int u_frameNum = 0;
};

