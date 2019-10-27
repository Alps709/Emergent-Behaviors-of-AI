#include "Math.h"

#include "Object.h"
#include "Camera.h"

Object::Object(Mesh* _mesh, Shader* _shader, glm::vec2 _position)
{
	m_mesh = _mesh;
	m_shader = _shader;
	m_position = _position;
	
	//Update the stored model matrix
	UpdateModelMat();
}

Object::~Object()
{
}

void Object::SetPosition(glm::vec2 _pos)
{
	m_position = _pos;

	//Update the stored model matrix
	UpdateModelMat();
}

void Object::SetTexture0(Texture* _tex)
{
	m_tex0 = _tex;
}

//void Object::SetTexture1(Texture* _tex)
//{
//	m_tex1 = _tex;
//}

void Object::ChangePRS(float _translateX, float _translateY, float _rotationAngle, float _scaleX, float _scaleY)
{
	m_position += glm::vec2(_translateX, _translateY);

	//Add new rotation and cap it to 360
	m_rotationZ += _rotationAngle;
	m_rotationZ = fmod(m_rotationZ, 360.0f);

	m_scale += glm::vec2(_scaleX, _scaleY);

	//Update the stored model matrix
	UpdateModelMat();
}

void Object::SetPRS(float _translateX, float _translateY, float _rotationAngle, float _scaleX, float _scaleY)
{
	m_position = glm::vec2(_translateX, _translateY);

	//Add new rotation and cap it to 360
	m_rotationZ = _rotationAngle;
	m_rotationZ = fmod(m_rotationZ, 360.0f);

	m_scale = glm::vec2(_scaleX, _scaleY);

	//Update the stored model matrix
	UpdateModelMat();
}

void Object::UpdateModelMat()
{
	//Update the stored model matrix
	m_modelMat = Math::Create2DModelMatrix(m_position.x, m_position.y, m_rotationZ, m_scale.x, m_scale.y);
}

void Object::Render(Camera& _myCamera)
{
	//Prepare renderer (eg. clear buffer, create PVM matrix etc.)
	glm::mat4 projViewMat = _myCamera.GetProjView();

	//Bind the mesh that all the models will use
	m_mesh->Bind();
	m_shader->Bind();

	glm::mat4 pvmMat = projViewMat * m_modelMat;

	//Prepare the object for drawing
	BindTexture(0);

	//Set object specific uniforms
	m_shader->SetUniformMat4f("u_PVM", pvmMat);

	//Draw the object
	GLCall(glDrawElements(GL_TRIANGLES, m_mesh->GetindicesCount(), GL_UNSIGNED_INT, static_cast<void *>(0)));

	Mesh::Unbind();
	Shader::Unbind();
}

void Object::BindTexture(unsigned int _texNum) const
{
	if (_texNum == 0)
	{
		m_tex0->Bind();
	}
	//else if (_texNum == 1)
	//{
	//	m_tex1->Bind();
	//}
}
