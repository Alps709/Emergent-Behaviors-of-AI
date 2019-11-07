#include "Boid.h"
#include "Math.h"
#include "GameManager.h"

namespace glm
{
	//Returns the angle between two vectors
	float AngleBetween(glm::vec2 _a, glm::vec2 _b)
	{
		return acos(glm::dot(_a, _b) / (glm::length(_a) * glm::length(_b)));
	}

	glm::vec2 LineNormalPoint(glm::vec2 _p, glm::vec2 _a, glm::vec2 _b)
	{
		glm::vec2 ap = _p - _a;
		glm::vec2 ab = _b - _a;

		ab = glm::normalize(ab);
		ab *= glm::dot(ab, ap);

		return _a + ab;
	}
}

Boid::Boid(int _boidID, Mesh* _mesh, Shader* _shader, glm::vec2 _position) : Object(_mesh, _shader,  _position)
{
	m_boidID = _boidID;
	m_position = glm::vec2(0.0f, 0.0f);
	m_velocity = glm::vec2(0.0, 0.0f);
	m_acceleration = glm::vec2(0.0f, 0.0f); 
	m_path = Path();
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

void Boid::Process(GameplayState& _gameState, std::vector<Boid> _boids, bool _containment, int _mouseX, int _mouseY, double _deltaTime)
{
	if (GameManager::m_gameplayState == PLAY_SEEK)
	{
		m_acceleration += Seek(glm::vec2(_mouseX, _mouseY));
	}
	else if(GameManager::m_gameplayState == PLAY_ARRIVE)
	{
		m_acceleration += Arrive(glm::vec2(_mouseX, _mouseY));
	}
	else if (GameManager::m_gameplayState == PLAY_FOLLOWPATH)
	{
		m_acceleration += PathFollowing(m_path) + Separation(_boids);
	}
	else if (GameManager::m_gameplayState == PLAY_FLOCK)
	{
		m_acceleration += Flocking(glm::vec2(_mouseX, _mouseY), _boids);
	}

	//Apply acceleration
	//Limit acceleration based on game play state
	if (GameManager::m_gameplayState == PLAY_FOLLOWPATH)
	{
		Math::LimitVector2D(m_acceleration, m_maxPathAcceleration);
	}
	else
	{
		Math::LimitVector2D(m_acceleration, m_maxAcceleration);
	}
	m_velocity += m_acceleration;

	//Limit velocity based on game play state
	if(GameManager::m_gameplayState == PLAY_FOLLOWPATH)
	{
		Math::LimitVector2D(m_velocity, m_maxPathSpeed);
	}
	else
	{
		Math::LimitVector2D(m_velocity, m_maxSpeed);
	}
	
	//Apply velocity to movement (affected by delta time)
	m_position += m_velocity * static_cast<float>(_deltaTime);

	//Reset acceleration every update
	m_acceleration *= 0.0f;

	//Have the boid loop back to the other side if it goes off screen
	if(_containment)
	{
		Containment();
	}
	else
	{
		WrapPos();
	}
	
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
	else if (m_position.x < -Utils::HSCREEN_WIDTH)
	{
		m_position.x = Utils::HSCREEN_WIDTH;
	}

	if (m_position.y > Utils::HSCREEN_HEIGHT)
	{
		m_position.y = -Utils::HSCREEN_HEIGHT;
	}
	else if (m_position.y < -Utils::HSCREEN_HEIGHT)
	{
		m_position.y = Utils::HSCREEN_HEIGHT;
	}
}

void Boid::Containment()
{
	//Have the boids reverse their velocity when they go off screen
	if (m_position.x > Utils::HSCREEN_WIDTH)
	{
		m_position.x = Utils::HSCREEN_WIDTH;
		m_velocity.x *= -1;
	}
	else if (m_position.x < -Utils::HSCREEN_WIDTH)
	{
		m_position.x = -Utils::HSCREEN_WIDTH;
		m_velocity.x *= -1;
	}

	if (m_position.y > Utils::HSCREEN_HEIGHT)
	{			   					  
		m_position.y = Utils::HSCREEN_HEIGHT;
		m_velocity.y *= -1;
	}			   
	else if (m_position.y < -Utils::HSCREEN_HEIGHT)
	{			   					   
		m_position.y = -Utils::HSCREEN_HEIGHT;
		m_velocity.y *= -1;
	}
}

glm::vec2 Boid::Seek(glm::vec2 _target)
{
	//Calculate the desired vector
	glm::vec2 desiredVec = _target - m_position;

	//Limit the magnitude to max speed
	Math::LimitVector2D(desiredVec, m_maxSpeed);

	glm::vec2 steeringForce = desiredVec - m_velocity;

	if(GameManager::m_gameplayState == PLAY_FOLLOWPATH)
	{
		Math::LimitVector2D(steeringForce, m_maxPathForce);
		
	}
	else
	{
		Math::LimitVector2D(steeringForce, m_maxForce);
	}
	
	return steeringForce;
}

glm::vec2 Boid::Arrive(glm::vec2 _target)
{
	//Calculate the desired vector
	glm::vec2 desiredVec = _target - m_position;
	float desiredVecLength = glm::length(desiredVec);

	//Distance in pixels
	if (desiredVecLength <= m_boidArriveViewRadius)
	{
		const float arriveSpeed = (float)Utils::remap(desiredVecLength, 0.0f, m_boidArriveViewRadius, 0.0f, m_maxSpeed);
		desiredVec = glm::normalize(desiredVec);
		desiredVec *= arriveSpeed;
	}
	else
	{
		//Limit the magnitude to max speed
		desiredVec = glm::normalize(desiredVec);
		Math::LimitVector2D(desiredVec, m_maxSpeed);
	}

	//Calculate steering force
	glm::vec2 steeringForce = desiredVec - m_velocity;
	Math::LimitVector2D(steeringForce, m_maxForce);

	return steeringForce;
}

glm::vec2 Boid::PathFollowing(Path _path)
{
	//Keep the distance of the closest normal (Set it high to begin with)
	float lowestDistance = 100000;
	
	//Point to move towards
	glm::vec2 targetPoint;

	const glm::vec2 predictedPosition = m_position + (glm::normalize(m_velocity) * 25.0f);

	//Check distance for the normal of each path segment
	for(int i = 0; i < m_path.m_pathPoints.size() - 1; ++i)
	{
		glm::vec2 a = m_path.m_pathPoints[i];
		glm::vec2 b = m_path.m_pathPoints[i + 1];

		glm::vec2 normalPoint = glm::LineNormalPoint(predictedPosition, a, b);

		//Is normal on the path (only works for paths that go to the right)
		if(normalPoint.x > b.x || normalPoint.x < a.x)
		{
			//Set normal for this segment to be the end of the segment
			//as the actual normal is not on the path
			normalPoint = b;
		}

		float distance = glm::length(predictedPosition - normalPoint);

		if (distance < lowestDistance)
		{
			lowestDistance = distance;

			//Set the target position on the line to 50 pixels ahead of the normalPoint
			glm::vec2 direction = b - a;
			targetPoint = normalPoint + (glm::normalize(direction) * 50.0f);
		}
	}

	//Seek to the point on the line
	if (lowestDistance > m_path.m_pathRadius)
	{
		//Return the steering force towards the point
		return Seek(targetPoint);
	}
	return glm::vec2{ 0.0f, 0.0f };
}


glm::vec2 Boid::Flocking(glm::vec2 _target, std::vector<Boid>& _boids)
{
	return Alignment(_boids) +
		Separation(_boids) + 
		Cohesion(_boids)
		/*Seek(_target)*/;
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
			if (distance <= m_boidAlignmentViewRadius)
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
		//Math::LimitVector2D(sum, m_maxSpeed);
		steeringForce = sum - m_velocity;
	}
	
	//Math::LimitVector2D(steeringForce, m_maxForce);
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
				//glm::normalize(diff);
				//diff = diff / (glm::length(diff));
				sum += diff;
				count++;
			}
		}
	}

	glm::vec2 steeringForce;
	if (count > 0)
	{
		//Calculate average of all the vectors
		sum /= count;
		//Calculate the steering force
		steeringForce = sum - m_velocity;
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
			if (distance <= m_boidCohesionViewRadius)
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
