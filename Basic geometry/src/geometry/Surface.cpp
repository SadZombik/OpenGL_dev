#include "Surface.h"
#include "../util/Log.h"

Surface::Surface(const float width, const float height)
    : ID(++idGen), m_Width(width), m_Height(height)
{
    Init();
    m_Shader = Shader(
        "res/shaders/surface/vertex.glsl",
        "res/shaders/surface/fragment.glsl"
    );
    m_Shader.Use();
    m_Shader.SetInt("texture1", 0);
}

Surface::Surface(const Surface& other)
    : ID(++idGen)
{
    m_Shader = other.m_Shader;
    m_Pos = other.m_Pos;

    LOG("COPYING SURFACE" << other.ID << " TO SURFACE" << ID);
    Init();
}

Surface::~Surface()
{

}

void Surface::Init()
{
    LOG("ID = " << ID);
    m_SurfaceTexture = Texture("res/textures/marble.jpg");

    m_SurfaceVAO.Bind();
    m_VBO.Bind();
    m_VBO.SetData(sizeof(m_Vertices), m_Vertices);

    m_EBO.Bind();
    m_EBO.SetData(sizeof(m_Indices), m_Indices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void Surface::Update(const Camera& camera)
{
    m_Shader.Use();
    
    glm::mat4 projection = camera.GetPerspectiveMatrix();
    glm::mat4 view = camera.GetViewMatrix();    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_Pos);
    model = glm::scale(model, glm::vec3(m_Width, 1, m_Height));
    
    m_Shader.SetMat4("projection", projection);
    m_Shader.SetMat4("view", view);
    m_Shader.SetMat4("model", model);

    m_SurfaceTexture.Bind();
    m_SurfaceVAO.Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// statics 
unsigned int Surface::idGen = 0;
const float Surface::m_Vertices[] = {
    // координаты        // текстурные координаты
    0.5f, 0.0f,  0.5f, 1.0f, 1.0f, // верхн€€ права€ вершина
    0.5f, 0.0f, -0.5f, 1.0f, 0.0f, // нижн€€ права€ вершина
   -0.5f, 0.0f, -0.5f, 0.0f, 0.0f, // нижн€€ лева€ вершина
   -0.5f, 0.0f,  0.5f, 0.0f, 1.0f  // верхн€€ лева€ вершина
};
const unsigned int Surface::m_Indices[] = {
    0, 1, 3, // первый треугольник
    1, 2, 3  // второй треугольник
};
