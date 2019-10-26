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
	Snek();
	~Snek();

	void Render(Camera& _camera);
	void Process(GameState& _gameState, unsigned& _score, int _mouseX, int _mouseY, double _deltaTime);

private:
	CClock m_snekClock;

	SnekPart m_boidObject;

	//The spacing (in pixels) the snake can move at a time
	int m_snekSpeed = 5;

	double m_movementX = 0.0;
	double m_movementY = 0.0;
};

