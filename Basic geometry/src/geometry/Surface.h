#ifndef SURFACE_H
#define SURFACE_H

#include "GeometryObject.h"

class Surface : public GeometryObject
{
public:
	Surface(const float width=1, const float height=1);
	Surface(const Surface& other);
	~Surface();

	void Init();
	void Update(const Camera& camera) override;
	void SetSize(const float width, const float height);
	const float GetWidth() const { return m_Width; }
	const float GetHeight() const { return m_Height; }

private:
	float m_Width, m_Height;

	VertexArrayObject m_SurfaceVAO, m_LightVao;
	VertexBufferObject m_VBO;
	ElementsBufferObject m_EBO;

	Texture m_SurfaceTexture;

	const unsigned int ID;
	static const float s_Vertices[32];
	static const unsigned int s_Indices[6];
	static unsigned int idGen;

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
};

#endif // !SURFACE_H
