#pragma once

#include<glad/glad.h>

#include <vector>

class EBO
{
public:
	GLuint ID;
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};