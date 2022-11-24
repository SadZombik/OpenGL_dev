#include "Shader.h"
#include "Log.h"

Shader::Shader()
{
	LOG("CONSTRUCTION");
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	LOG("INITIALIZATION");
	ID = CreateShader(
		ParseShader(vertexPath),
		ParseShader(fragmentPath)
	);
}

Shader::Shader(const Shader& other)
	: ID(other.ID)
{
	LOG("COPYING");
}

Shader::~Shader()
{
	glDeleteShader(ID);
}

Shader& Shader::operator = (const Shader& other)
{
	ID = other.ID;
	return *this;
}

GLuint Shader::GetID() const 
{
	return ID; 
}

void Shader::Use() const 
{ 
	glUseProgram(ID); 
}

void Shader::SetInt(const std::string& name, const int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, const float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& vec) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

void Shader::SetVec3(const std::string& name, const float x, const float y, const float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const 
{
	glUniformMatrix4fv(
		glGetUniformLocation(ID, name.c_str()),
		1,
		GL_FALSE,
		&mat[0][0]
	);
}

std::string Shader::ParseShader(const std::string& filepath) const
{
	std::ifstream fstream(filepath);

	if (!fstream.is_open()) {
		std::cout << "Can not parse shader: " << filepath << std::endl;
	}
	auto ss = std::ostringstream{};
	ss << fstream.rdbuf();
	return ss.str();
}

GLuint Shader::CompileShader(const std::string& source, GLuint type) const
{
	GLuint id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error logging
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int lenght;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
		char* message = (char*)_malloca(lenght * sizeof(char)); // allocating on the stack
		glGetShaderInfoLog(id, lenght, &lenght, message);
		std::cout
			<< "Failed to compile "
			<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< '\n' << message
			<< std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const
{
	GLuint program = glCreateProgram();
	GLuint vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
	GLuint fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}