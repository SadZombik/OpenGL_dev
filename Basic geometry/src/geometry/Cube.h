#ifndef CUBE_H
#define CUBE_H

#include "GeometryObject.h"

class Cube : public GeometryObject
{
public:
	Cube(const float EdgeLength=1);
	Cube(const Cube& other);
	~Cube();
	
	static unsigned int GetCount() { return idGen; }
	void SetEdgeLength(const float value);
	float GetEdgeLength() const { return m_EdgeLength; }

	void Update(const Camera& camera) override;

private:
	void Init();

	Texture m_DiffuseMap;
	Texture m_SpecularMap;

	VAO m_CubeVao, m_LightVao;
	VBO m_Vbo;

	float m_EdgeLength;
	static const float m_Vertices[288];
	const unsigned int ID;
	static unsigned int idGen;
};


#endif // !CUBE_H
