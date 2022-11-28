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

private:
	float m_Width, m_Height;

	VertexArrayObject m_SurfaceVAO, m_LightVao;
	VertexBufferObject m_VBO;
	ElementsBufferObject m_EBO;

	Texture m_SurfaceTexture;

	const unsigned int ID;
	static const float m_Vertices[20];
	static const unsigned int m_Indices[6];
	static unsigned int idGen;
};

#endif // !SURFACE_H
