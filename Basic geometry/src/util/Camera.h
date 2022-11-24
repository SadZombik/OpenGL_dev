#ifndef CAMERA_H
#define CAMERA_H

#define S_WIDTH 1280
#define S_HEIGHT 720

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera
{
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 FrontVector = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 UpVector = glm::vec3(0.0f, 1.0f, 0.0f);

	float yaw = -90.0f;
	float pitch = 0.0f;
	float lastX = (float)S_WIDTH  / 2.0;
	float lastY = (float)S_HEIGHT / 2.0;
	float fov = 45.0f;
	bool  firstMouse = true;

	glm::mat4 GetPerspectiveMatrix() const
	{
		return glm::perspective(
			glm::radians(fov),
			(float)S_WIDTH / (float)S_HEIGHT, 
			0.1f, 
			100.0f
		);
	}

	glm::mat4 GetViewMatrix() const
	{
		return glm::lookAt(
			Position,
			Position + FrontVector,
			UpVector
		);
	}
};

#endif // !CAMERA_H
