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

	glm::vec2 Seek(glm::vec2 _target);
	glm::vec2 Arrive(glm::vec2 _target);
	glm::vec2 Flocking(glm::vec2 _target, std::vector<Boid>& _boids);
	glm::vec2 Alignment(std::vector<Boid>& _boids);
	glm::vec2 Separation(std::vector<Boid>& _boids);
	glm::vec2 Cohesion(std::vector<Boid>& _boids);

private:
	int m_boidID;
	CClock m_boidClock;

	glm::vec2 m_velocity;
	glm::vec2 m_acceleration;
	glm::vec2 m_force;

	const float m_maxSpeed = 1.25f;
	const float m_maxAcceleration = 0.15f;
	const float m_maxForce = 0.02f;
	const float m_maxSeperationForce = 0.05f;

	double m_boidViewRadius = 60.0f;
	double m_boidArriveViewRadius = 600.0f;
	double m_boidCohesionViewRadius = 200.0f;
};

