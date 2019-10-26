#pragma once
#include "Object.h"
#include "clock.h"

enum GameState;

class Boid final : public Object
{
public:
	Boid(Mesh* _mesh, Shader* _shader, glm::vec2 _position);
	~Boid() = default;
	
	void Render(Camera& _myCamera) override;

	void Process(GameState& _gameState, int _mouseX, int _mouseY, double _deltaTime);

	void SetShaderUniforms(glm::mat4 _pvm) const;

	double GetRadius() const override { return m_snekColliderRadius; }

	CClock m_snekClock;

	glm::vec2 m_velocity;
	glm::vec2 m_acceleration;

	double m_snekColliderRadius = 20.0f;
};

