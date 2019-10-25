#include "Camera.h"
#include "Utils.h"

Camera::Camera(bool _isOrthoView)
   :m_isOrthographicView(_isOrthoView),
	m_camPosition(glm::vec3{ 0.0f, 0.0f, 500.0f }),
	m_camLookDir(glm::vec3{0.0f, 0.0f, -1.0f}),
	m_worldUp(glm::vec3{ 0.0f, 1.0f, 0.0f }), 
	m_camYaw(-90.0f), m_camPitch(0), m_camRoll(0), m_camSpeed(5), m_mouseSens(1.0f),
	m_viewMat(glm::lookAt(m_camPosition, m_camPosition + m_camLookDir, m_worldUp)),
	m_orthoProjectionMat(glm::ortho(-Utils::HSCREEN_WIDTH, Utils::HSCREEN_WIDTH, -Utils::HSCREEN_HEIGHT, Utils::HSCREEN_HEIGHT, -10000.0f, 10000.0f)),
	m_perspectiveProjectionMat(glm::perspective(glm::radians(90.0f), 960.0f/540.0f, 0.1f, 10.0f))
{
	UpdateVectors();
	UpdateView();
}

Camera::~Camera()
= default;

void Camera::SetPosition(glm::vec3 _newPos)
{
	m_camPosition = _newPos;
	UpdateVectors();
	UpdateView();
}

void Camera::SetLookDirection(glm::vec3 _newLookDirection)
{
	m_camLookDir = _newLookDirection;
	UpdateVectors();
	UpdateView();
}

void Camera::SetOrthographicProjection(bool _isOrtho)
{
	m_isOrthographicView = _isOrtho;

	//Reset the camera view back to default for an orthographic view
	if (_isOrtho)
	{
		ResetView();
	}
}

void Camera::ResetView()
{
	//Change the view back to the original
	//So that it is changed back to a view that is perpendicular to the 2d plane
	m_camYaw = -90.0f;
	m_camPitch = 0;
	m_camRoll = 0;
	SetLookDirection(glm::vec3{ 0.0f, 0.0f, -1.0f });
}

void Camera::UpdateView()
{
	m_viewMat = glm::lookAt(m_camPosition, m_camPosition + m_camLookDir, m_worldUp);
	//Update the projection view matrix
	m_projViewMat = (m_isOrthographicView ? m_orthoProjectionMat * m_viewMat : m_perspectiveProjectionMat * m_viewMat);
}

void Camera::UpdateVectors()
{
	//Update front vector
	m_camLookDir = glm::normalize
                   (
                    glm::vec3(cos(glm::radians(m_camYaw)) * cos(glm::radians(m_camPitch)), //x
                   	sin(glm::radians(m_camPitch)),												  //y
                   	sin(glm::radians(m_camYaw)) * cos(glm::radians(m_camPitch)))             //z
                   );

	//Update up and right vectors
	m_camRight = glm::normalize(glm::cross(m_camLookDir, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	m_camUp = glm::normalize(glm::cross(m_camRight, m_camLookDir));
}

void Camera::ProcessMouseInput(float _xOffset, float _yOffset, bool _limitPitch = true)
{
	_xOffset *= m_mouseSens;
	_yOffset *= m_mouseSens;

	m_camYaw += _xOffset;
	
	//Limit the pitch between 90 and -90 degrees, so the camera can't get flipped
	if (_limitPitch)
	{
		m_camPitch += _yOffset;
		if (m_camPitch > 89)
		{
			m_camPitch = 89;
		}
		else if (m_camPitch < -89)
		{
			m_camPitch = -89;
		}
	}

	//Update the vectors from the mouse input
	UpdateVectors();
	UpdateView();
}
