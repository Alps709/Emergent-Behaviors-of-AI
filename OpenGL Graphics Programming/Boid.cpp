#include "Boid.h"
#include "Utils.h"


Boid::Boid(Mesh* _mesh, Shader* _shader, glm::vec2 _position) : Object(_mesh, _shader,  _position)
{
	m_position = glm::vec2(0.0f, 0.0f);
	m_velocity = glm::vec2(0.0, 0.0f);
	m_acceleration = glm::vec2(0.0001f, 0.0001f);
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

void Boid::Process(GameplayState& _gameState, int _mouseX, int _mouseY, double _deltaTime)
{
	Arrive(glm::vec2(_mouseX, _mouseY));

	m_velocity += m_acceleration;

	////Limit velocity
	//if(glm::length(m_velocity) > m_maxSpeed)
	//{
	//	m_velocity = glm::normalize(m_velocity) * m_maxSpeed;
	//}

	//Apply velocity to movement (affected by delta time)
	m_position += m_velocity * static_cast<float>(_deltaTime);

	//Reset acceleration every update
	m_acceleration *= 0.0f;

	//Update model matrix so the boid faces in the direction it is moving
	SetPRS(m_position.x, m_position.y, glm::degrees(std::atan2(m_velocity.y, m_velocity.x)) - 90, 1.0f, 1.0f);
}

void Boid::SetShaderUniforms(glm::mat4 _pvm) const
{
	m_shader->SetUniform1i("tex1", 0);
	m_shader->SetUniformMat4f("u_PVM", _pvm);
}

void Boid::Seek(glm::vec2 _target)
{
	//Calculate the desired vector
	glm::vec2 desiredVec = _target - m_position;

	//Limit the magnitude to max speed
	desiredVec = glm::normalize(desiredVec);
	desiredVec *= m_maxSpeed;

	glm::vec2 steeringVec = desiredVec - m_velocity;
	steeringVec = glm::normalize(steeringVec);
	steeringVec *= m_maxForce;
	m_acceleration += steeringVec;
}

void Boid::Arrive(glm::vec2 _target)
{
	//Calculate the desired vector
	glm::vec2 desiredVec = _target - m_position;
	float desiredVecLength = glm::length(desiredVec);

	desiredVec = glm::normalize(desiredVec);
	if (desiredVecLength <= 250)
	{
		const float arriveSpeed = Utils::remap(desiredVecLength, 0.0f, 250.0f, 0.0f, m_maxSpeed);
		desiredVec *= arriveSpeed;
	}
	else
	{
		//Limit the magnitude to max speed
		if (glm::length(desiredVec) > m_maxSpeed)
		{
			desiredVec *= m_maxSpeed;
		}
	}

	//Calculate steering force
	glm::vec2 steeringForce = desiredVec - m_velocity;
	if(glm::length(steeringForce) > m_maxForce)
	{
		//Limit steering force
		steeringForce = glm::normalize(steeringForce) * m_maxForce;
	}
	m_acceleration += steeringForce;
}
