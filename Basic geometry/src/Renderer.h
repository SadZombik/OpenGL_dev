#ifndef RENDERER_H
#define RENDERER_H

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Utils
#include <vector>
#include <iostream>

#include "util/InputHandler.h"
#include "geometry/Cube.h"
#include "geometry/Surface.h"

class Renderer
{
public:
    Renderer(const Renderer&) = delete;
    static Renderer& Get() {
        try {
            static Renderer renderer;
            return renderer;
        } 
        catch (std::exception& ex) {
            std::cerr << "Renderer exception: " 
                << ex.what() << std::endl;
            throw;
        }
        catch (...) {
            std::cerr << "Renderer exception: "
                << "Unexpected exception" << std::endl;
            throw;
        }
    }

    static void Loop() {
        Get().m_Loop();
    }

private:
    Renderer() {
        if (!Init()) 
            throw std::exception("Failed to Init Renderer");
    }

    ~Renderer() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    Camera camera;
    GLFWwindow* window;

    bool Init() {
        InputHandler::SetCamera(&camera);
        window = CreateWindow(InputHandler::Get());
        return (window ? 1 : 0);
    }

    void m_Loop() {
        Cube cubes[10];
        Surface surf(10, 10);
        surf.SetPos(0, -cubes[0].GetEdgeLength() /*/2*/, 0);

        std::vector<GeometryObject*> Objects;
        Objects.push_back(&surf);

        for (int i = 0; i < 10; i++) {
            cubes[i].SetPos(i, 0, 0);
            Objects.push_back(&cubes[i]);
        }



        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window)) {
            InputHandler::Update();
            InputHandler::keyboard_processing(window);

            // Render here
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очищаем буфер цвета и буфер глубины

            for (auto object : Objects)
                object->Update(camera);

            cubes[7].SetPos(sin(glfwGetTime()), 0, 1);

            glBindVertexArray(0);

            // Swap front and back bufmfers
            glfwSwapBuffers(window);

            // Poll for and process events
            glfwPollEvents();
        }
    }

    GLFWwindow* CreateWindow(InputHandler& ih) {
        static GLFWwindow* window;

        // Initialize the library
        if (!glfwInit())
            return nullptr;

        // Create a windowed mode window and its OpenGL context
        window = glfwCreateWindow(S_WIDTH, S_HEIGHT, "OpenGL", NULL, NULL);
        if (!window) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return nullptr;
        }

        // Make the window's context current
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        glfwSetFramebufferSizeCallback(window, ih.framebuffer_size_callback);
        glfwSetCursorPosCallback(window, ih.mouse_callback);
        glfwSetScrollCallback(window, ih.scroll_callback);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (glewInit() != GLEW_OK)
            std::cout << "glewInit failed!" << std::endl;
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        //glFrontFace(GL_CCW);
        return window;
    }
};

#endif // !RENDERER_H