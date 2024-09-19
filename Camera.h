#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include "glm/gtc/matrix_transform.hpp"

enum CameraMovement
{
	LEFT,RIGHT,FORWARD,BACKWARD
};

class Camera
{
public:
	Camera(glm::vec3 position);
	Camera(float x, float y, float z);
	Camera(glm::vec3 position, float pitch, float yaw);

	glm::mat4 GetViewMatrix() const;
	glm::vec3 GetCameraPosition() const;
	glm::vec3 GetCameraFront() const;
	float GetZoom() const;
	void ProcessKeyboardInput(CameraMovement movement, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessMouseScroll(float yoffset);

private:
	const float MOVEMENT_SPEED = 95.0f;
	const float SENSITIVITY = 0.1f;
	const float MAX_ZOOM = 45.0f;
	glm::vec3 WorldUp;
	glm::vec3 m_Position;
	glm::vec3 m_FrontCam;
	glm::vec3 m_UpCam;
	glm::vec3 m_RightCam;
	float m_Zoom;
	float m_Pitch;
	float m_Yaw;

	void UpdateDirection();
};

#endif
