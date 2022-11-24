#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GLFW/glfw3.h>

class Shader
{
public:
	Shader();
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	Shader(const Shader& other);
	~Shader();
	Shader& operator = (const Shader& other);

	GLuint GetID() const;
	void Use() const;

	void SetInt		(const std::string& name, const int value) const;
	void SetFloat	(const std::string& name, const float value) const;
	void SetVec3	(const std::string& name, const glm::vec3& vec) const;
	void SetVec3	(const std::string& name, const float x, const float y, const float z) const;
	void SetMat4	(const std::string& name, const glm::mat4& mat) const;	

private:
	GLuint ID = 0;

	std::string ParseShader(const std::string& filepath) const;
	GLuint CompileShader(const std::string& source, const GLuint type) const;
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const;
};

#endif // !SHADER_H
