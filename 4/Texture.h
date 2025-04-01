#pragma once
#include <glad/glad.h>
#include <stb/stb_image.h>

#include"shaderClass.h"
#include"GLErrorChecking.h"

class Texture
{
public:
	GLuint ID;
	GLenum type;
	GLuint unit;

	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);

	void Bind();
	void Unbind();
	void Delete();
};
