#include "Snek.h"

#include "Obj.h"
#include "GameManager.h"

#include <fmod.h>
#include <fmod.hpp>
#include <corecrt_math.h>


Snek::Snek()
{
	m_snekClock.Initialise();
	
	//Set Snake body circle's mesh and texture

	m_boidObject = SnekPart(new Mesh(Objects::verticesSnekPart, Objects::indicesSnekPart), new Shader(), glm::vec3(0, 0, 0));
	m_boidObject.SetTexture0(new Texture("Resources/Images/Circle.png", 0));
}


Snek::~Snek()
{
	delete m_snekMesh;
	delete m_snekShader;
	delete m_snekTexture;
}

void Snek::Render(Camera& _camera)
{
	//Iterate over it backwards so the first element (the head of the snek) is rendered on top of the others
	for (auto rIt = m_snekObjects.rbegin(); rIt != m_snekObjects.rend(); ++rIt)
	{
		rIt->Render(_camera);
	}
}

void Snek::Process(GameState& _gameState, unsigned int& _score, int _mouseX, int _mouseY, double _deltaTime)
{
	//
	m_snekClock.Process();
	if(m_snekClock.GetTimeElapsedS() > 1.0/3.0)
	{
		//2 seconds has past, so delete the last snek part
		m_snekClock.ResetElaspedTime();
		if(!m_snekObjects.empty())
		{
			m_snekObjects.pop_back();
		}
		else
		{
			_gameState = GAME_OVER;
		}
	}

	int i = 0;
	glm::vec3 tempPos{ 0 };

	for (auto& snekPart : m_snekObjects)
	{
		//If its the first iteration of the loop, then this is the head of the snek
		if (i == 0)
		{
			double mouseXDif = _mouseX - snekPart.GetPosition().x;
			double mouseYDif = _mouseY - snekPart.GetPosition().y;

			//Stop the snek if it moves within 5 pixels of the mouse
			if (mouseXDif < 5 && mouseXDif > -5 && mouseYDif < 5 && mouseYDif > -5)
			{
				return;
			}

			//Find distance
			double distance = (double)sqrtf((float)(mouseXDif * mouseXDif + mouseYDif * mouseYDif));

			//Normalise
			mouseXDif /= distance;
			mouseYDif /= distance;

			//Accumulate movement
			m_movementX += mouseXDif * _deltaTime;
			m_movementY += mouseYDif * _deltaTime;

			//Actual movement distance
			double movementDist = sqrt((m_movementX * m_movementX) + (m_movementY * m_movementY));

			//If the movement spacing threshold is not met, then the snake can not move yet
			if (!(movementDist > m_snekSpeed)) return;

			//Move the head of the snek
			snekPart.ChangePRS((float)m_movementX, (float)m_movementY, 0.0f, 0.0f, 0.0f);
			tempPos = snekPart.GetPosition();

			//Set these back to 0 after the accumulated movement was used
			m_movementX = 0;
			m_movementY = 0;

			//Do head collision checks
			for (auto pillIt = m_pillObjects->begin(); pillIt != m_pillObjects->end();)
			{
				//Find distance to current pill
				const double distanceX = pillIt->GetPosition().x - snekPart.GetPosition().x;
				const double distanceY = pillIt->GetPosition().y - snekPart.GetPosition().y;
				const double pillDist = sqrt(distanceX * distanceX + distanceY * distanceY);

				
				if (pillDist < pillIt->GetRadius() + snekPart.GetRadius())
				{
					//Head of snek and current pill are colliding
					//EAT PILL
					//Erase pill
					pillIt = m_pillObjects->erase(pillIt);

					//Add a snek part
					SnekPart myTempObject = SnekPart(m_snekMesh, m_snekShader, glm::vec3(-10000, -10000, 0));
					myTempObject.SetTexture0(m_snekTexture);
					m_snekObjects.push_back(myTempObject);

					//Update score text
					std::string tempScore = "Score: ";
					tempScore = tempScore + std::to_string(++_score);
					m_scoreText->SetText(tempScore);

					//Play sound
					const FMOD_RESULT play = m_audioSystem->playSound(m_yeatSound, 0, false, 0);
					if (play != FMOD_OK)
					{
						std::cout << "Failed to play sound: Thump.wav" << std::endl;
					}
				}
				else
				{
					//Only move the iterator forward if nothing was erased
					//Because it is automatically moved forward after erase() is called
					++pillIt;
				}
			}
			//Increment to say it's not the first loop anymore
			i++;
		}
		else
		{	//This is not the first iteration of the loop, so it's a body section of the snek
			const glm::vec3 tempPos2 = snekPart.GetPosition();

			//Move the rest of the body
			snekPart.SetPosition(tempPos);
			tempPos = tempPos2;
		}
		i++;
	}
}