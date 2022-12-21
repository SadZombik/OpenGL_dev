#include "Cube.h"
#include "../util/Log.h"

Cube::Cube(const float EdgeLength)
    : ID(++idGen), m_EdgeLength(EdgeLength)
{
    Init();

    m_Shader = Shader(
        "res/shaders/cube/vertex.glsl",
        "res/shaders/cube/fragment.glsl"
    );
    m_Shader.Use();
    m_Shader.SetInt("material.diffuse", 0);
    m_Shader.SetInt("material.specular", 1);
}

Cube::Cube(const Cube& other) :
    ID(++idGen), 
    m_EdgeLength(other.m_EdgeLength),
    m_DiffuseMap(other.m_DiffuseMap),
    m_SpecularMap(other.m_SpecularMap)
{
    m_Shader = other.m_Shader;
    m_Pos = other.m_Pos;

    LOG("COPYING CUBE " << other.ID << " TO CUBE " << ID);
    Init();
}

void Cube::Init()
{
    LOG("ID = " << ID);
    m_DiffuseMap = Texture("res/textures/wooden_container_2.png");
    m_SpecularMap = Texture("res/textures/container_2_specular.png");

    m_Vbo.Bind();
    m_Vbo.SetData(sizeof(s_Vertices), s_Vertices);
    m_CubeVao.Bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    m_LightVao.Bind();
    m_Vbo.Bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

Cube::~Cube()
{

}

void Cube::Update(const Camera& camera)
{
    m_Shader.Use();
    m_Shader.SetVec3("light.position", camera.Position);
    m_Shader.SetVec3("viewPos", camera.Position);
    
    m_Shader.SetVec3("light.direction", camera.FrontVector);
    m_Shader.SetFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    m_Shader.SetFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
    
    // Lighting options
    m_Shader.SetVec3("light.ambient", 0.5f, 0.5f, 0.5f);
    m_Shader.SetVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    m_Shader.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
    
    m_Shader.SetFloat("light.constant", 1.0f);
    m_Shader.SetFloat("light.linear", 0.09f);
    m_Shader.SetFloat("light.quadratic", 0.032f);

    // Material options
    m_Shader.SetFloat("material.shininess", 32.0f);

    projection = camera.GetPerspectiveMatrix();
    view = camera.GetViewMatrix();
    model = glm::mat4(1.0f);
    model = glm::translate(model, m_Pos);
    model = glm::scale(model, glm::vec3(m_EdgeLength, m_EdgeLength, m_EdgeLength));

    m_Shader.SetMat4("projection", projection);
    m_Shader.SetMat4("view", view);
    m_Shader.SetMat4("model", model);

    m_DiffuseMap.Bind(0);
    m_SpecularMap.Bind(1);

    m_CubeVao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::SetEdgeLength(const float value) 
{
    m_EdgeLength = value; 
}

// statics
unsigned int Cube::idGen = 0;
const float Cube::s_Vertices[] = {
    // координаты        // нормали           // текстурные координаты
   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};