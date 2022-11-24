#ifndef OPENGLOBJECTS_H
#define OPENGLOBJECTS_H

#include <stb_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Log.h"

// Abstract class for all objects in OpenGL
class OpenGLObject
{
public:
	OpenGLObject() : ID(0) {};
	virtual ~OpenGLObject() {};
	virtual GLuint getID() const { return ID; }
	virtual void Bind() = 0;
	virtual void Unbind() = 0;

protected:
	GLuint ID;
};

// Vertex Array Object 
class VAO : public OpenGLObject
{
public:
	VAO() { glGenVertexArrays(1, &ID); }
	~VAO() { 
        glDeleteVertexArrays(1, &ID);
        Unbind(); 
    }

	void Bind()		override { glBindVertexArray(ID); }
	void Unbind()	override { glBindVertexArray(0); }
};

// Vertex Buffer Object 
class VBO : public OpenGLObject
{
public:
	VBO() { glGenBuffers(1, &ID); }
	~VBO() { 
        glDeleteBuffers(1, &ID);
        Unbind(); 
    }

	void Bind()		override { glBindBuffer(GL_ARRAY_BUFFER, ID); }
	void Unbind()	override { glBindBuffer(GL_ARRAY_BUFFER, 0); }
	void SetData(size_t size, const void* data) {
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
};

// Elements Buffer Object 
class EBO : public OpenGLObject
{
public:
	EBO() { glGenBuffers(1, &ID); }
	~EBO() { Unbind(); }

	void Bind() override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
	void Unbind() override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
	void SetData(size_t size, const void* data) {
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}
};

class Texture : public OpenGLObject
{
public:
    Texture() { LOG(" CONSTRUCTION"); }
    Texture(const std::string& path)
        : m_Path(path)
    {
        LOG("INITIALIZATION");
        Init();
    }

    Texture(const Texture& other)
        : Texture(other.m_Path)
    {
        LOG("COPYING");
    }

    Texture& operator = (const Texture& other) {
        m_Path = other.m_Path;
        Init();
        return *this;
    }

    void Init() {
        // Загрузка и создание текстуры
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        // установка параметров наложения текстуры
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // установка метода наложения текстуры GL_REPEAT (стандартный метод наложения)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Установка параметров фильтрации текстуры
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(m_Path.c_str(), &m_Width, &m_Height, &m_Channels, 0);
        if (data) {
            GLenum format = 0;
            switch (m_Channels)
            {
            case 1:
                format = GL_RED;
                break;
            case 3:
                format = GL_RGB;
                break;
            case 4:
                format = GL_RGBA;
                break;
            default:
                break;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture: " << m_Path << std::endl;
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    ~Texture() {
        glDeleteTextures(1, &ID);
    }

    void Bind() override {
        Bind(0);
    }

    void Bind(GLuint slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    void Unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    int m_Width = 0, m_Height = 0, m_Channels = 0;
    std::string m_Path;
};

#endif // !OPENGLOBJECTS_H
