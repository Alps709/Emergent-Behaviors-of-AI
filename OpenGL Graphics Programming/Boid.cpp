#include "Boid.h"
#include "Utils.h"
#include "Math.h"
#include "GameManager.h"


Boid::Boid(Mesh* _mesh, Shader* _shader, glm::vec2 _position) : Object(_mesh, _shader,  _position)
{
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
		Seek(glm::vec2(_mouseX, _mouseY));
	}
	else if(GameManager::m_gameplayState == PLAY_ARRIVE)
	{
		Arrive(glm::vec2(_mouseX, _mouseY));
	}
	//else if(GameManager::m_gameplayState == )

	//Apply acceleration
	m_velocity += m_acceleration;

	//Apply velocity to movement (affected by delta time)
	m_position += m_velocity * static_cast<float>(_deltaTime);

	//Reset acceleration every update
	m_acceleration *= 0.0f;

	WrapPos();

	//Update model matrix so the boid faces in the direction it is moving
	SetPRS(m_position.x, m_position.y, glm::degrees(std::atan2(m_velocity.y, m_velocity.x)) - 90, 1.0f, 1.0f);
}

void Boid::SetShaderUniforms(glm::mat4 _pvm) const
{
	m_shader->SetUniform1i("tex1", 0);
	m_shader->SetUniformMat4f("u_PVM", _pvm);
}

void Boid::ApplyForce(glm::vec2 _force)
{
	//Use this when your objects have mass
	//m_acceleration = _force / m_mass;
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

void Boid::Seek(glm::vec2 _target)
{
	//Calculate the desired vector
	glm::vec2 desiredVec = _target - m_position;

	//Limit the magnitude to max speed
	Math::LimitVector2D(desiredVec, m_maxSpeed);

	glm::vec2 steeringVec = desiredVec - m_velocity;
	Math::LimitVector2D(steeringVec, m_maxForce);

	m_acceleration += steeringVec;
	Math::LimitVector2D(m_acceleration, m_maxAcceleration);
}

void Boid::Arrive(glm::vec2 _target)
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

	m_acceleration += steeringForce;
	Math::LimitVector2D(m_acceleration, m_maxAcceleration);
}

void Boid::Allignment(std::vector<Boid>& _boids)
{
	int count = 0;
	glm::vec2 sum;
	for(auto& boid : _boids)
	{
		if(&boid == this)
		{
			glm::vec2 distance = boid.m_position - this->m_position;
			if(glm::length(distance) <= m_boidColliderRadius)
			{
				sum += boid.m_velocity;
				count++;
			}
		}
	}

	if(count > 0)
	{
		sum /= count;

		//Apply and limit steering force
		glm::vec2 steeringForce = sum - m_velocity;
		Math::LimitVector2D(steeringForce, m_maxForce);

		//Apply and limit acceleration
		m_acceleration += steeringForce;
		Math::LimitVector2D(m_acceleration, m_maxAcceleration);
	}
}
