#pragma once
#include<glad/glad.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <vector>

//struct Vertex
//{
//	glm::vec3 position;
//	glm::vec3 normal;
//	glm::vec3 color;
//	glm::vec2 texUV;
//};


class VBO
{
public:
	GLuint ID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};