#pragma once
#include "Object.h"
#include "clock.h"

enum GameplayState;

class Boid final : public Object
{
public:
	Boid(int _boidID, Mesh* _mesh, Shader* _shader, glm::vec2 _position);
	~Boid() = default;
	
	void Render(Camera& _myCamera) override;

	void Process(GameplayState& _gameState, std::vector<Boid> _boids, int _mouseX, int _mouseY, double _deltaTime);

	void SetShaderUniforms(glm::mat4 _pvm) const;

	double GetRadius() const override { return m_boidViewRadius; }

	void ApplyForces(glm::vec2 _force);

	void WrapPos();

	void Seek(glm::vec2 _target);
	void Arrive(glm::vec2 _target);
	void Flocking(glm::vec2 _target, std::vector<Boid>& _boids);
	void Alignment(std::vector<Boid>& _boids);
	void Separation(std::vector<Boid>& _boids);
	void Cohesion(std::vector<Boid>& _boids);

private:
	int m_boidID;
	CClock m_boidClock;

	glm::vec2 m_velocity;
	glm::vec2 m_acceleration;
	glm::vec2 m_force;

	const float m_maxSpeed = 0.5f;
	const float m_maxAcceleration = 0.075f;
	const float m_maxForce = 0.01f;
	const float m_maxSeperationForce = 0.075f;
	const float m_aligmentWeighting = 0.1;

	double m_boidViewRadius = 100.0f;
};

