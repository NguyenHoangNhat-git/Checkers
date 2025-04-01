#include "HandlePosition.h"

Position operator-(Position a, Position b)
{
	return { a.x - b.x, a.y - b.y };
}
Position operator+(Position a, int num)
{
	return { a.x + num, a.y + num };
}
Position operator*(Position a, Position b)
{
	return { a.x * b.x, a.y * b.y };
}
Position XYToBlock(float precise_x, float precise_y)
{
	int block_x = (int)((precise_x / 100) + 1);
	int block_y = (int)((precise_y / 100) + 1);
	return { block_x, block_y };
}

void GetCursorPosition(GLFWwindow* window, Position& newPos) //sometimes returns null
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (firstClick)
		{
			firstClick = false;
			double x = 0.0f;
			double y = 0.0f;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwGetCursorPos(window, &x, &y);
			try
			{
				if (x == NULL || y == NULL)
				{
					throw std::invalid_argument("cant get CursorPos");
				}
			}
			catch (const std::invalid_argument& error)
			{
				std::cout << error.what() << std::endl;
				std::terminate;
			}
			x = x - 50;
			y = WINDOW_HEIGHT - y - 50;
			newPos = XYToBlock(x, y);
			std::cout << "Got Position : x : " << newPos.x << " | y : " << newPos.y << std::endl;
			ClickCount += 1;
			/*return { position };*/
		}
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}
