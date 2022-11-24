#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Camera.h"

class InputHandler
{
public:
	InputHandler(const InputHandler&) = delete;
	static InputHandler& Get() {
		static InputHandler IH(m_camera);
		return IH;
	}

	static void SetCamera(Camera* newCam);
	static void Update();

	static void keyboard_processing(GLFWwindow* window);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double x, double y);
	static void scroll_callback(GLFWwindow* window, double x, double y);

private:
	InputHandler(Camera* camera);

	// Тайминги
	static float deltaTime;	// время между текущим кадром и последним кадром
	static float lastFrame;

	static Camera* m_camera;
};

#endif // !INPUTHANDLER_H