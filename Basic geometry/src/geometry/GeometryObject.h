#ifndef GEOMETRYOBJECT_H
#define GEOMETRYOBJECT_H

#include "../util/OpenGLObjects.h"
#include "../util/Camera.h"
#include "../util/Shader.h"

class GeometryObject
{
public:
	GeometryObject(): m_Pos(0) {}
	virtual ~GeometryObject() { glDeleteShader(m_Shader.GetID()); }

	void SetPos(const float x, const float y, const float z) {
		m_Pos = { x, y, z };
	}

	void SetPos(const glm::vec3& Position) {
		m_Pos = {
			Position.x,
			Position.y,
			Position.z
		};
	}
	glm::vec3 GetPos() const { return m_Pos; }
	virtual void Update(const Camera& camera) {}

protected:
	Shader m_Shader;
	glm::vec3 m_Pos;
};

#endif // !GEOMETRYOBJECT_H
