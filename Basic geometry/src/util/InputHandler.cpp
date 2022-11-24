#include "InputHandler.h"

#define LOG(x) \
{\
    std::cout << __FUNCTION__\
    << " " << __LINE__ << " "\
    << x << std::endl;\
}

InputHandler::InputHandler(Camera* camera)
{
	m_camera = camera;
}

void InputHandler::SetCamera(Camera* newCam)
{
	m_camera = newCam;
}

void InputHandler::Update()
{
	// Логическая часть работы со временем для каждого кадра
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void InputHandler::keyboard_processing(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	const float cameraSpeed = 2.5f * deltaTime; // настройте по вашему усмотрению
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_camera->Position += cameraSpeed * m_camera->FrontVector;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_camera->Position -= cameraSpeed * m_camera->FrontVector;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_camera->Position -= glm::normalize(glm::cross(m_camera->FrontVector, m_camera->UpVector)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_camera->Position += glm::normalize(glm::cross(m_camera->FrontVector, m_camera->UpVector)) * cameraSpeed;
}

void InputHandler::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// Убеждаемся, что окно просмотра соответствует новым размерам окна
	glViewport(0, 0, width, height);
}

void InputHandler::mouse_callback(GLFWwindow* window, double x, double y) {
	float xpos = (float)x;
	float ypos = (float)y;

	if (m_camera->firstMouse) {
		m_camera->lastX = xpos;
		m_camera->lastY = ypos;
		m_camera->firstMouse = false;
	}

	float xoffset = xpos - m_camera->lastX;
	float yoffset = m_camera->lastY - ypos; // перевернуто, так как y-координаты идут снизу вверх
	m_camera->lastX = xpos;
	m_camera->lastY = ypos;

	float sensitivity = 0.1f; // измените это значение по своему усмотрению
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_camera->yaw += xoffset;
	m_camera->pitch += yoffset;

	// Убеждаемся, что когда значение тангаж выходит за пределы, экран не переворачивается
	m_camera->pitch = m_camera->pitch > 89.0f ? 89.0f : m_camera->pitch;
	m_camera->pitch = m_camera->pitch < -89.0f ? -89.0f : m_camera->pitch;

	glm::vec3 front = glm::vec3(
		cos(glm::radians(m_camera->yaw)) * cos(glm::radians(m_camera->pitch)),
		sin(glm::radians(m_camera->pitch)),
		sin(glm::radians(m_camera->yaw)) * cos(glm::radians(m_camera->pitch))
	);
	m_camera->FrontVector = glm::normalize(front);
}

void InputHandler::scroll_callback(GLFWwindow* window, double x, double y) {
	float xoffset = (float)x;
	float yoffset = (float)y;

	if (m_camera->fov >= 1.0f && m_camera->fov <= 45.0f) m_camera->fov -= yoffset;
	if (m_camera->fov <= 1.0f) m_camera->fov = 1.0f;
	if (m_camera->fov >= 45.0f) m_camera->fov = 45.0f;
}

Camera* InputHandler::m_camera = nullptr;
float InputHandler::deltaTime = 0.0f;
float InputHandler::lastFrame = 0.0f;