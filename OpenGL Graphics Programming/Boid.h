#pragma once
#include "Object.h"
#include "clock.h"
#include "Utils.h"

enum GameplayState;

class Path
{
public:
	Path()
	{
		//Add horizontal line across the screen
		AddPathPoint(glm::vec2(-Utils::HSCREEN_WIDTH, 0));
		AddPathPoint(glm::vec2(-Utils::HSCREEN_WIDTH / 2, Utils::HSCREEN_HEIGHT / 2));
		AddPathPoint(glm::vec2(Utils::HSCREEN_WIDTH / 2, -Utils::HSCREEN_HEIGHT / 2));
		AddPathPoint(glm::vec2(Utils::HSCREEN_WIDTH, 0));
	};

	void AddPathPoint(glm::vec2 _point)
	{
		m_pathPoints.push_back(_point);
	}

	std::vector<glm::vec2> m_pathPoints;

	float m_pathRadius = 1;
};

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
	glm::vec2 FollowPath(Path _path);
	glm::vec2 Flocking(glm::vec2 _target, std::vector<Boid>& _boids);
	glm::vec2 Alignment(std::vector<Boid>& _boids);
	glm::vec2 Separation(std::vector<Boid>& _boids);
	glm::vec2 Cohesion(std::vector<Boid>& _boids);

private:
	int m_boidID;
	CClock m_boidClock;
	Path m_path;

	glm::vec2 m_velocity;
	glm::vec2 m_acceleration;
	glm::vec2 m_force;

	const float m_maxSpeed = 0.75f;
	const float m_maxPathSpeed = 0.25f;

	const float m_maxAcceleration = 0.0075f;
	const float m_maxPathAcceleration = 0.05f;

	const float m_maxForce = 0.02f;
	const float m_maxPathForce = 0.5f;

	const float m_maxSeperationForce = 0.05f;

	double m_boidViewRadius = 60.0f;
	double m_boidArriveViewRadius = 600.0f;
	double m_boidCohesionViewRadius = 200.0f;
};

