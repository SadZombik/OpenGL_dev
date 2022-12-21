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
#include "geometry/Cubemap.h"

// Imgui
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"

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

    ~Renderer() {}

    Camera camera;
    GLFWwindow* window;

    bool Init() {
        InputHandler::SetCamera(&camera);
        window = CreateWindow(InputHandler::Get());
        
        // Setup ImGui binding
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplGlfwGL3_Init(window, true);

        // Setup style
        ImGui::StyleColorsDark();
        return (window ? 1 : 0);
    }

    void m_Loop() {
        Cube cube;
        Surface surf(10, 10);
        surf.SetPos(0, -cube.GetEdgeLength() /*/2*/, 0);

        std::vector<std::string> faces {
            "res/textures/skybox/right.jpg",
            "res/textures/skybox/left.jpg",
            "res/textures/skybox/top.jpg",
            "res/textures/skybox/bottom.jpg",
            "res/textures/skybox/front.jpg",
            "res/textures/skybox/back.jpg"
        };
        Cubemap skybox(faces);

        std::vector<GeometryObject*> Objects;
        Objects.push_back(&cube);
        Objects.push_back(&surf);
        Objects.push_back(&skybox);

        float cube_size = cube.GetEdgeLength();
        float surf_size = surf.GetWidth();

        // Loop until the user closes the window
        while (!glfwWindowShouldClose(window)) {
            InputHandler::Update();
            InputHandler::keyboard_processing(window);

            // Render here
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            cube.SetEdgeLength(cube_size);
            surf.SetSize(surf_size, surf_size);

            for (auto object : Objects)
                object->Update(camera);

            if (InputHandler::GetMouseMode())
            {
                ImGui_ImplGlfwGL3_NewFrame();
                ImGui::Begin("Settings");  
                ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::SliderFloat("fov", &camera.fov, 45.0f, 90.0f);
                ImGui::SliderFloat("cube size", &cube_size, 0.1f, 10.0f);
                ImGui::SliderFloat("plane size", &surf_size, 0.1f, 10.0f);
                ImGui::End();
                ImGui::Render();
                ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
            }

            // Swap front and back bufmfers
            glfwSwapBuffers(window);

            // Poll for and process events
            glfwPollEvents();
        }
        ImGui_ImplGlfwGL3_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window);
        glfwTerminate();
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