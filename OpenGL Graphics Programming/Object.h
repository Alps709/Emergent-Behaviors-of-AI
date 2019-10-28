#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include <detail/type_vec3.hpp>
#include <detail/type_mat4x4.hpp>

class  Object
{
public:
	Object() = default;
	Object(Mesh* _mesh, Shader* _shader, glm::vec2 _position);
	virtual ~Object();

	//Getters
	glm::vec2 GetPosition() const { return m_position; }

	//Setters
	void SetPosition(glm::vec2 _pos);
	void SetTexture0(Texture* _tex);
	/*void SetTexture1(Texture* _tex);*/

	//Getters
	Mesh* GetMesh() const { return m_mesh; }
	glm::mat4 GetModelMat() const { return m_modelMat; }
	virtual double GetRadius() const { return m_colliderRadius; };

	//Change m_position, rotation and m_scale
	void ChangePRS(float _translateX, float _translateY, float _rotationAngle, float _scaleX, float _scaleY);
	void SetPRS(float _translateX, float _translateY, float _rotationAngle, float _scaleX, float _scaleY);
	
	void UpdateModelMat();
	
	//Rendering stuff
	//Render is overidden by all derived classes, but can still be used for a base object
	virtual void Render(Camera& _myCamera);

	
	void BindTexture(unsigned int _texNum) const;

protected:
	glm::vec2 m_position{ 0.0f, 0.0f };
	float m_rotationZ = 0.0f;
	glm::vec2 m_scale{ 1.0f, 1.0f };
	double m_colliderRadius = 10.0f;

	glm::mat4 m_modelMat = glm::mat4(1.0f);

	Mesh* m_mesh;
	Shader* m_shader;
	Texture* m_tex0{};
	//Texture* m_tex1{};
};

