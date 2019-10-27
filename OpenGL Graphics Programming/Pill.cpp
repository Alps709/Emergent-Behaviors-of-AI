#include "Pill.h"

//Calls the base class constructor
Pill::Pill(Mesh* _mesh, Shader* _shader, glm::vec2 _position) : Object(_mesh, _shader, _position)
{
	m_pillClock.Initialise();
}

void Pill::Render(Camera& _myCamera)
{
	//Prepare renderer (eg. clear buffer, create PVM matrix etc.)
	const glm::mat4 projViewMat = _myCamera.GetProjView();

	//Bind the mesh that all the models will use
	m_mesh->Bind();
	m_shader->Bind();

	const glm::mat4 pvmMat = projViewMat * m_modelMat;

	//Prepare the object for drawing
	BindTexture(0);

	//Set object specific uniforms
	SetShaderUniforms(pvmMat);

	//Draw the object
	GLCall(glDrawElements(GL_TRIANGLES, m_mesh->GetindicesCount(), GL_UNSIGNED_INT, static_cast<void *>(0)));

	Mesh::Unbind();
}

void Pill::SetShaderUniforms(glm::mat4 _pvm) const
{
	m_shader->SetUniform1i("tex1", 0);
	m_shader->SetUniformMat4f("u_PVM", _pvm);
}

double Pill::GetTimeAlive()
{
	m_pillClock.Process();
	return m_pillClock.GetTimeElapsedS();
}
