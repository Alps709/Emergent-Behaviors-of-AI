#pragma once
#include "gtc/matrix_transform.hpp"

class Camera
{
public:
	Camera(bool _isOrthoView = true);
	~Camera();

	glm::mat4 GetView() const { return m_viewMat; }
	glm::mat4 GetProjection() const { return ((m_isOrthographicView) ? m_orthoProjectionMat : m_perspectiveProjectionMat); }
	glm::mat4 GetProjView() const { return m_projViewMat; }
	
	glm::vec3 GetPosition() const { return m_camPosition; }
	glm::vec3 GetLookDirection() const { return m_camLookDir; }
	glm::vec3 GetCamRight() const { return m_camRight; }
	float GetCamSpeed() const { return m_camSpeed; }

	void SetPosition(glm::vec3 _newPos);
	void SetLookDirection(glm::vec3 _newLookDirection);
	void SetOrthographicProjection(bool _isOrtho);
	void ResetView();

	void UpdateView();
	void UpdateVectors();
	void ProcessMouseInput(float _xOffset, float _yOffset, bool _limitPitch);


private:
	//Camera vectors
	bool m_isOrthographicView;
	glm::vec3 m_camPosition;
	glm::vec3 m_camLookDir;
	glm::vec3 m_camUp;
	glm::vec3 m_camRight;
	glm::vec3 m_worldUp;

	//Camera controls (rotation values in degrees)
	float m_camYaw;
	float m_camPitch;
	float m_camRoll;
	float m_camSpeed;
	float m_mouseSens;

	//Camera matrices
	glm::mat4 m_viewMat{};
	glm::mat4 m_orthoProjectionMat{};
	glm::mat4 m_perspectiveProjectionMat{};
	glm::mat4 m_projViewMat{};
};

