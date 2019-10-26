#include "Boid.h"



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

	//Bind the mesh that all the models will use
	m_mesh->Bind();
	m_shader->Bind();

	const glm::mat4 pvmMat = projViewMat * m_objectMat;

	//Set object specific uniforms
	SetShaderUniforms(pvmMat);

	//Draw the object
	GLCall(glDrawElements(GL_TRIANGLES, m_mesh->GetindicesCount(), GL_UNSIGNED_INT, static_cast<void *>(0)));

	Mesh::Unbind();
}

void Boid::Process(GameState& _gameState, int _mouseX, int _mouseY, double _deltaTime)
{
	m_acceleration = glm::vec2(_mouseX - m_position.x, _mouseY - m_position.y);

	if (m_acceleration.length() > 1)
	{
		glm::normalize(m_acceleration);
		m_acceleration *= 1;
	}

	m_velocity += m_acceleration;

	if (m_velocity.length() > 0.01)
	{
		glm::normalize(m_velocity);
		m_velocity *= 0.01;
	}

	m_position += m_velocity;

	UpdateModelMat();
}

void Boid::SetShaderUniforms(glm::mat4 _pvm) const
{
	m_shader->SetUniform1i("tex1", 0);
	m_shader->SetUniformMat4f("u_PVM", _pvm);
}
