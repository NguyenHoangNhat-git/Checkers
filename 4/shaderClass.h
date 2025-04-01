#pragma once
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shader
{
public:
	GLuint ID;

	void CreateShader(const char* vertexFile, const char* fragmentFile);
	void Activate();
	void Delete();
private:
	void compileErrors(unsigned int shader, const char* type);
};
