#pragma once
#include<iostream>
#include<vector>
#include<string>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<stb/stb_image.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shaderClass.h"

extern int ClickCount;
extern std::vector<GLuint> units_Indices;

constexpr auto WINDOW_WIDTH = 900;
constexpr auto WINDOW_HEIGHT = 900;
/////

struct Position
{
	int x = 0;
	int y = 0;

	Position() = default;
	Position(int x, int y) 
		: x(x), y(y) {};

	void operator=(const Position& value)
	{
		x = value.x;
		y = value.y;
	}
	friend std::ostream& operator<<(std::ostream& os, const Position& position)
	{
		os << position.x << "," << position.y;
		return os;
	}
	bool operator>(const Position& anotherPos)
	{
		return (x > anotherPos.x && y > anotherPos.y);
	}
	bool operator==(const Position& anotherPos)
	{
		return (x == anotherPos.x && y == anotherPos.y);
	}
	bool operator==(const int& num)
	{
		return (x == num && y == num);
	}
	bool operator!=(const int& a)
	{
		return (x != a && y != a);
	}
	Position operator*(const int& num)
	{
		return { x * num, y * num };
	}
};


Position operator-(Position a, Position b);
Position operator+(Position a, int num);
Position operator*(Position a, Position b);

Position XYToBlock(float precise_x, float precise_y);

extern bool firstClick;
void GetCursorPosition(GLFWwindow* window, Position& newPos);