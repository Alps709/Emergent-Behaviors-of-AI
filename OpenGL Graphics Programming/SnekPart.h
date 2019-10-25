#pragma once
#include "Object.h"

class SnekPart final : public Object
{
public:
	SnekPart(Mesh* _mesh, Shader* _shader, glm::vec3 _position);
	~SnekPart() = default;
	
	void Render(Camera& _myCamera) override;
	void SetShaderUniforms(glm::mat4 _pvm) const;
	double GetRadius() const override { return m_snekColliderRadius; }

	double m_snekColliderRadius = 20.0f;
};

