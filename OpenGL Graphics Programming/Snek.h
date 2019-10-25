#pragma once

#include "SnekPart.h"
#include "Pill.h"
#include "TextLabel.h"

#include <vector>

enum GameState;

namespace FMOD {
	class System;
	class Sound;
}

class Snek
{
public:
	Snek(std::vector<Pill>* _pillObjects, TextLabel* _scoreText, FMOD::System* _audioSystem, FMOD::Sound* _yeatSound, FMOD::Sound* _shoopSound, unsigned _snekStartSize = 50);
	~Snek();

	void Render(Camera& _camera);
	void Process(GameState& _gameState, unsigned& _score, int _mouseX, int _mouseY, double _deltaTime);

private:
	CClock m_snekClock;

	std::vector<SnekPart> m_snekObjects;
	std::vector<Pill>* m_pillObjects;

	TextLabel* m_scoreText;
	FMOD::System* m_audioSystem;
	FMOD::Sound* m_yeatSound;
	FMOD::Sound* m_shoopSound;

	Mesh* m_snekMesh = nullptr;
	Shader* m_snekShader = nullptr;
	Texture* m_snekTexture = nullptr;

	//The spacing (in pixels) the snake can move at a time
	int m_snekSpeed = 5;

	double m_movementX = 0.0;
	double m_movementY = 0.0;
};

