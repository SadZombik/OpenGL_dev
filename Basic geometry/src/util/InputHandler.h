#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

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
	static bool GetMouseMode() { return Get().m_MouseMode; }
	static void Update();

	static void keyboard_processing(GLFWwindow* window);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double x, double y);
	static void scroll_callback(GLFWwindow* window, double x, double y);

private:
	InputHandler(Camera* camera);

	static bool m_MouseMode;

	// Тайминги
	static float m_DeltaTime;	// время между текущим кадром и последним кадром
	static float m_LastFrame;

	static Camera* m_camera;
};

#endif // !INPUTHANDLER_H