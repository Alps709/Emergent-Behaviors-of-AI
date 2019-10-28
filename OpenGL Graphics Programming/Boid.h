#pragma once
#include "Object.h"
#include "clock.h"

enum GameplayState;

class Boid final : public Object
{
public:
	Boid(Mesh* _mesh, Shader* _shader, glm::vec2 _position);
	~Boid() = default;
	
	void Render(Camera& _myCamera) override;

	void Process(GameplayState& _gameState, int _mouseX, int _mouseY, double _deltaTime);

	void SetShaderUniforms(glm::mat4 _pvm) const;

	double GetRadius() const override { return m_snekColliderRadius; }

	void ApplyForce(glm::vec2 _force);

	void Seek(glm::vec2 _target);
	void Arrive(glm::vec2 _target);

private:
	CClock m_snekClock;

	glm::vec2 m_velocity;
	glm::vec2 m_acceleration;
	glm::vec2 m_force;

	const float m_maxSpeed = 0.5f;
	const float m_maxAcceleration = 0.075f;
	const float m_maxForce = 0.01f;

	double m_snekColliderRadius = 20.0f;
};

