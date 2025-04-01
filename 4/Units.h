#pragma once
#include "HandlePosition.h"

constexpr auto NormalStride = 1;
constexpr auto UpgradedStride = 8;
struct CheckUnitPicked
{
	bool IfValid = false;
	int whichUnit = 0;
	int xIndex = 0;
};


class Team
{
private:
public:
	int side;
	std::string name = "";
	std::vector<GLfloat> units_vertices;
	std::vector<int> UnitsPos;

	const CheckUnitPicked UnitExist(const int& x, const int& y);
	const CheckUnitPicked UnitExist(const Position& target);

	const Position GetTarget(const Position& current, const Position& desti);
	const CheckUnitPicked Check_If_ValidMove(const Position& currentPos, const Position& destination,Team& enemy);
	const bool CheckUpgradedMove(Position currentPos, const Position& destination, Team& enemy, const int& pathType);

	const void ChangeDisplayPos(int index, const Position& stride);
	const void ChangeVertices(int VerIndex, const Position& stride);
	const void DeleteDisplayPos(int index);

	const void Upgrade(const Position& desti, const int& UnitStridePos);
	const void Move(GLFWwindow* window, const Position& currentPos, const Position& destination,
		Team& enemy);
};

class Board
{
public:
	std::vector<GLfloat> background;
};

std::string CheckWin(const Team& firstT, const Team& secondT);