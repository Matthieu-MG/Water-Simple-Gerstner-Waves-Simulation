#include "Camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position) : m_Pitch(0.0f), m_Yaw(-90.0f), m_Position(position), m_Zoom(MAX_ZOOM)
{
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	UpdateDirection();
}

Camera::Camera(float x, float y, float z) : m_Pitch(0.0f), m_Yaw(-90.0f), m_Zoom(MAX_ZOOM)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
	WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	UpdateDirection();
}

Camera::Camera(glm::vec3 position, float pitch, float yaw):
	m_Position(position), m_Pitch(pitch), m_Yaw(yaw), m_Zoom(MAX_ZOOM)
{
	UpdateDirection();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(m_Position, m_Position + m_FrontCam, WorldUp);
}

glm::vec3 Camera::GetCameraPosition() const
{
	return m_Position;
}

glm::vec3 Camera::GetCameraFront() const
{
	return m_FrontCam;
}

float Camera::GetZoom() const
{
	return m_Zoom;
}

void Camera::ProcessKeyboardInput(CameraMovement movement, float deltaTime)
{
	float velocity = MOVEMENT_SPEED * deltaTime;
	switch (movement)
	{
	case LEFT:
		m_Position -= m_RightCam * velocity;
		break;
	case RIGHT:
		m_Position += m_RightCam * velocity;
		break;
	case FORWARD:
		m_Position += m_FrontCam * velocity;
		break;
	case BACKWARD:
		m_Position -= m_FrontCam * velocity;
		break;
	default:
		break;
	}
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= SENSITIVITY;
	yoffset *= SENSITIVITY;

	m_Yaw = glm::mod(m_Yaw+xoffset, 360.0f);
	m_Yaw += xoffset;
	m_Pitch += yoffset;

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	else if (m_Pitch < -89.0)
		m_Pitch = -89.0f;

	UpdateDirection();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	m_Zoom -= yoffset;
	if (m_Zoom > 45.0f)
		m_Zoom = 45.0f;
	else if (m_Zoom < 1.0f)
		m_Zoom = 1.0f;
}

void Camera::UpdateDirection()
{
	glm::vec3 direction(0.0f, 0.0f, 0.0f);
	direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	direction.y = sin(glm::radians(m_Pitch));
	direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	m_FrontCam = glm::normalize(direction);

	m_RightCam = glm::normalize(glm::cross(m_FrontCam ,WorldUp));
	m_UpCam	   = glm::normalize(glm::cross(m_RightCam, m_FrontCam));
}