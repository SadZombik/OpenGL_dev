#ifndef SKYBOX_H
#define SKYBOX_H

#include "../util/Shader.h"
#include "../util/OpenGLObjects.h"
#include "GeometryObject.h"
#include <vector>

class Cubemap : public GeometryObject
{
public:
	Cubemap(std::vector<std::string>& paths);
	~Cubemap();

	void Update(const Camera& camera) override;

private:
	GLuint load_texture();
	
	GLuint ID;
	Shader m_Shader;
	std::vector<std::string> m_Faces;
	VertexArrayObject m_VAO;
	VertexBufferObject m_VBO;
	static const float m_Vertices[108];

	// matrices
	glm::mat4 view;
	glm::mat4 projection;
};

#endif // !SKYBOX_H
