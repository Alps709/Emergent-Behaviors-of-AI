#include "Boid.h"
#include "Utils.h"
#include "Math.h"
#include "GameManager.h"


Boid::Boid(int _boidID, Mesh* _mesh, Shader* _shader, glm::vec2 _position) : Object(_mesh, _shader,  _position)
{
	m_boidID = _boidID;
	m_position = glm::vec2(0.0f, 0.0f);
	m_velocity = glm::vec2(0.0, 0.0f);
	m_acceleration = glm::vec2(0.0f, 0.0f);
}

void Boid::Render(Camera& _myCamera)
{
	//Prepare renderer (eg. clear buffer, create PVM matrix etc.)
	const glm::mat4 projViewMat = _myCamera.GetProjView();
	const glm::mat4 pvmMat = projViewMat * m_modelMat;

	//Bind the mesh that all the models will use
	m_mesh->Bind();
	m_shader->Bind();

	//Set object specific uniforms
	SetShaderUniforms(pvmMat);

	//Draw the object
	GLCall(glDrawElements(GL_TRIANGLES, m_mesh->GetindicesCount(), GL_UNSIGNED_INT, static_cast<void *>(0)));

	Mesh::Unbind();
}

void Boid::Process(GameplayState& _gameState, std::vector<Boid> _boids, int _mouseX, int _mouseY, double _deltaTime)
{
	if (GameManager::m_gameplayState == PLAY_SEEK)
	{
		m_acceleration += Seek(glm::vec2(_mouseX, _mouseY));
	}
	else if(GameManager::m_gameplayState == PLAY_ARRIVE)
	{
		m_acceleration += Arrive(glm::vec2(_mouseX, _mouseY));
	}
	else if (GameManager::m_gameplayState == PLAY_FLOCK)
	{
		m_acceleration += Flocking(glm::vec2(_mouseX, _mouseY), _boids);
	}

	//Apply acceleration
	Math::LimitVector2D(m_acceleration, m_maxAcceleration);
	m_velocity += m_acceleration;

	Math::LimitVector2D(m_velocity, m_maxSpeed);
	//Apply velocity to movement (affected by delta time)
	m_position += m_velocity * static_cast<float>(_deltaTime);

	//Reset acceleration every update
	m_acceleration *= 0.0f;

	//Have the boid loop back tot he other side if it oes off screen
	WrapPos();

	//Update model matrix so the boid faces in the direction it is moving
	SetPRS(m_position.x, m_position.y, glm::degrees(std::atan2(m_velocity.y, m_velocity.x)) - 90, 1.0f, 1.0f);
}

void Boid::SetShaderUniforms(glm::mat4 _pvm) const
{
	m_shader->SetUniform1i("tex1", 0);
	m_shader->SetUniformMat4f("u_PVM", _pvm);
}

void Boid::ApplyForces(glm::vec2 _force)
{
	m_acceleration += _force;
	Math::LimitVector2D(m_acceleration, m_maxAcceleration);
}

void Boid::WrapPos()
{
	//Have the boids wrap around when they go off screen
	if (m_position.x > Utils::HSCREEN_WIDTH)
	{
		m_position.x = -Utils::HSCREEN_WIDTH;
	}
	if (m_position.x < -Utils::HSCREEN_WIDTH)
	{
		m_position.x = Utils::HSCREEN_WIDTH;
	}

	if (m_position.y > Utils::HSCREEN_HEIGHT)
	{
		m_position.y = -Utils::HSCREEN_HEIGHT;
	}
	if (m_position.y < -Utils::HSCREEN_HEIGHT)
	{
		m_position.y = Utils::HSCREEN_HEIGHT;
	}
}

glm::vec2 Boid::Seek(glm::vec2 _target)
{
	//Calculate the desired vector
	glm::vec2 desiredVec = _target - m_position;

	//Limit the magnitude to max speed
	Math::LimitVector2D(desiredVec, m_maxSpeed);

	glm::vec2 steeringForce = desiredVec - m_velocity;
	Math::LimitVector2D(steeringForce, m_maxForce);

	return steeringForce;
}

glm::vec2 Boid::Arrive(glm::vec2 _target)
{
	//Calculate the desired vector
	glm::vec2 desiredVec = _target - m_position;
	float desiredVecLength = glm::length(desiredVec);

	desiredVec = glm::normalize(desiredVec);

	//Distance in pixels
	if (desiredVecLength <= 250)
	{
		const float arriveSpeed = Utils::remap(desiredVecLength, 0.0f, 250.0f, 0.0f, m_maxSpeed);
		desiredVec *= arriveSpeed;
	}
	else
	{
		//Limit the magnitude to max speed
		Math::LimitVector2D(desiredVec, m_maxSpeed);
	}

	//Calculate steering force
	glm::vec2 steeringForce = desiredVec - m_velocity;
	Math::LimitVector2D(steeringForce, m_maxForce);

	return steeringForce;
}


glm::vec2 Boid::Flocking(glm::vec2 _target, std::vector<Boid>& _boids)
{
	return Alignment(_boids) + Separation(_boids) + Cohesion(_boids); //+ Seek(_target);
}

glm::vec2 Boid::Alignment(std::vector<Boid>& _boids)
{
	int count = 0;
	glm::vec2 sum{ 0.0f, 0.0f };
	for(auto& boid : _boids)
	{
		if (boid.m_boidID != this->m_boidID)
		{
			float distance = glm::length(boid.m_position - this->m_position);
			if (distance <= m_boidViewRadius)
			{
				sum += boid.m_velocity;
				count++;
			}
		}
	}

	glm::vec2 steeringForce{0.0f, 0.0f};
	if(count > 0)
	{
		sum /= count;

		//Apply and limit steering force
		Math::LimitVector2D(sum, m_maxSpeed);
		steeringForce = sum - m_velocity;
	}
	
	Math::LimitVector2D(steeringForce, m_maxForce);
	return steeringForce;
}

glm::vec2 Boid::Separation(std::vector<Boid>& _boids)
{
	int count = 0;
	glm::vec2 sum;
	for (auto& boid : _boids)
	{
		if (boid.m_boidID != this->m_boidID)
		{
			glm::vec2 distance = this->m_position - boid.m_position;
			if (glm::length(distance) <= m_boidViewRadius)
			{
				glm::vec2 diff = this->m_position - boid.m_position;
				glm::normalize(diff);
				diff = diff / (glm::length(diff) * 0.75f);
				sum += diff;
				count++;
			}
		}
	}

	glm::vec2 steeringForce;
	if (count > 0)
	{
		sum /= count;

		//Apply and limit steering force
		Math::LimitVector2D(sum, m_maxSpeed);
		steeringForce = sum - m_velocity;
		Math::LimitVector2D(steeringForce, m_maxSeperationForce);
	}
	return steeringForce;
}

glm::vec2 Boid::Cohesion(std::vector<Boid>& _boids)
{
	int count = 0;
	glm::vec2 sum{ 0.0f, 0.0f };
	for (auto& boid : _boids)
	{
		if (boid.m_boidID != this->m_boidID)
		{
			float distance =  glm::length(boid.m_position - this->m_position);
			if (distance <= m_boidViewRadius)
			{
				sum += boid.m_position;
				count++;
			}
		}
	}

	//Check if there were neighbor boids
	if (count > 0)
	{
		sum /= count;

		return Seek(sum);
	}
	return glm::vec2{ 0.0f, 0.0f };
}
