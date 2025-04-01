#include "Units.h"
constexpr auto Up_Move_Path = 0;
constexpr auto Up_Kill_Path = 2;

const CheckUnitPicked Team::UnitExist(const int &x,const int& y)
{
	for (int i = 0; i < UnitsPos.size(); i = i + 4)
	{
		if (x == UnitsPos[i] && y == UnitsPos[i + 1])
		{
			return { true, UnitsPos[i + 2], i };
		}
	}
	return { false, 0, 0 };
}
const CheckUnitPicked Team::UnitExist(const Position& unit)
{
	for (int i = 0; i < UnitsPos.size(); i = i + 4)
	{
		if (unit.x == UnitsPos[i] && unit.y == UnitsPos[i + 1])
		{
			return { true, UnitsPos[i + 2], i };
		}
	}
	return { false, 0, 0 };
}
const Position Team::GetTarget(const Position& current, const Position& desti)
{//get the pos of the unit which will be killed 
	if (current.x > desti.x && current.y > desti.y)
	{
		return { desti.x + 1, desti.y + 1 };//top right to bottom left
	}
	if (current.x < desti.x && current.y < desti.y)
	{
		return { desti.x - 1, desti.y - 1 };//bottom left to top right
	}
	if (current.x < desti.x && current.y > desti.y)
	{
		return { desti.x - 1, desti.y + 1 };//top left to bottom right
	}
	if (current.x > desti.x && current.y < desti.y)
	{
		return { desti.x + 1, desti.y - 1 };//bottom right to top left
	}
	else
	{
		return { 0, 0 };
	}
}
const void Team::Upgrade(const Position& desti, const int& UnitStridePos)
{
	if (desti.y == 4 + (4 * side))
	{
		UnitsPos[UnitStridePos] = 8;
		std::cout << "Upgraded, New UnitStride : " << UnitsPos[UnitStridePos] << std::endl;
	}
}
const void Team::ChangeDisplayPos(int index, const Position& stride)
{
	UnitsPos[index] += stride.x;
	UnitsPos[index + 1] += stride.y;
}
const void Team::ChangeVertices(int VerIndex, const Position& stride)
{
	for (int i = 0; i < 4; i++)
	{
		units_vertices[VerIndex * 8 * 4 + 8 * i] += stride.x * 100;
		units_vertices[VerIndex * 8 * 4 + 1 + 8 * i] += stride.y * 100;
	}
}
const void Team::DeleteDisplayPos(int index)
{
	for (int i = 0; i < 4; i++)
	{
		units_vertices[index * 8 * 4 + 8 * i] = 0;
		units_vertices[index * 8 * 4 + 1 + 8 * i] = 0;
	}
}
const CheckUnitPicked Team::Check_If_ValidMove(const Position& currentPos, const Position& destination,
	Team& enemy)
{
	CheckUnitPicked enemyCheck = enemy.UnitExist(destination.x, destination.y);
	if (enemyCheck.IfValid == true)
	{
		std::cout << "desti is already occupied by enemies!!\n";
		std::cerr << "Clickcount :" << ClickCount << std::endl;
		return { false, 0, 0 };
	}
	CheckUnitPicked destiCheck = UnitExist(destination.x, destination.y);
	if (destiCheck.IfValid == true)
	{
		std::cerr << "desti is already occupied by allies!!\n";
		std::cerr << "Clickcount :" << ClickCount << std::endl;
		return { false, 0, 0 };
	}
	if (destination.x < 1 || destination.y < 1 || destination.x > 8 || destination.y > 8)
	{
		std::cerr << "Desti is outside the board" << std::endl;
		return { false, 0, 0 };
	}
	CheckUnitPicked curCheck = UnitExist(currentPos.x, currentPos.y);
	if (curCheck.IfValid == true)
	{
		return { true, curCheck.whichUnit, curCheck.xIndex };
	}
	std::cerr << "no ally in current block!!\n";
	return { false, 0, 0 };
}
const bool Team::CheckUpgradedMove(Position currentPos, const Position& destination,
	Team& enemy, const int& pathType)
{//wrong somehow
	std::cout << "Checking...." << std::endl;
	Position block = currentPos;
	int xLoop = 0;
	int yLoop = 0;
	currentPos.x < destination.x ? xLoop = 1 : xLoop = -1;
	currentPos.y < destination.y ? yLoop = 1 : yLoop = -1;
	block.x += xLoop;
	block.y += yLoop;
	for (;(block.x * block.x) != (destination.x - pathType*xLoop)*(destination.x - pathType * xLoop) &&
		(block.y * block.y) != (destination.y - pathType*yLoop)*(destination.y - pathType * yLoop); block.x += xLoop, block.y += yLoop)
	{
		std::cout << "Position : " << block << std::endl;
		CheckUnitPicked BlockCheck = enemy.UnitExist(block);
		std::cout << BlockCheck.IfValid << std::endl;
		std::cout << destination.x - pathType * xLoop << "," << destination.y - pathType * yLoop << std::endl;
		std::cout << pathType * xLoop << "," << pathType * yLoop << std::endl;
		std::cout << "xloop: " << xLoop << " | yloop: " << yLoop << std::endl;
		if (BlockCheck.IfValid == true)
		{
			std::cout << "Path is blocked by enemy: " << block << std::endl;
			return false;
		}
		BlockCheck = UnitExist(block);
		if (BlockCheck.IfValid == true)
		{
			std::cout << "Path is blocked by ally : " << block << std::endl;
			return false;
		}
	}
	return true;
}
const void Team::Move(GLFWwindow* window, const Position& currentPos, const Position& destination,
	Team& enemy)
{
	Position stride = destination - currentPos;
	CheckUnitPicked result = Check_If_ValidMove(currentPos, destination, enemy);
	Position UnitStride = { UnitsPos[result.xIndex + 3], UnitsPos[result.xIndex + 3] };
	std::cout << "stride : " << stride << " | UnitStride: " << UnitStride << std::endl;
	if (result.IfValid == true && stride.x != 0)
	{
		if(UnitStride == NormalStride && stride.y * side > 0 && //normalMove 1,1 
				stride.y * side == UnitStride.y && (stride.x * stride.x) == (UnitStride.x * UnitStride.x))
		{//move//ok now
			ChangeDisplayPos(result.xIndex, stride);
			ChangeVertices(result.whichUnit, stride);
			Upgrade(destination, result.xIndex + 3);
			std::cout << "\n------NormalMove" << std::endl;
			ClickCount += 1;
			return;
		}
		if (UnitStride == NormalStride && stride.y * side == (UnitStride.y + 1) && //normal Move to kill 1,1 
				(stride.x * stride.x) == ((UnitStride.x + 1) * (UnitStride.x + 1)))
		{//move to kill
			CheckUnitPicked target = enemy.UnitExist(GetTarget(currentPos, destination));
			std::cout << target.IfValid << "; move to kill" << std::endl;
			if (target.IfValid > 0)
			{
				ChangeDisplayPos(result.xIndex, stride);
				enemy.UnitsPos.erase(std::next(enemy.UnitsPos.begin(), target.xIndex), std::next(enemy.UnitsPos.begin(), target.xIndex + 4));
				ChangeVertices(result.whichUnit, stride);
				enemy.DeleteDisplayPos(target.whichUnit);
				Upgrade(destination, result.xIndex + 3);
				std::cout << "\n------Kill" << std::endl;
				ClickCount -= 2;
				return;
			}
		}
		if (UnitStride == 8 && (stride.y * stride.y) <= (UnitStride.y * UnitStride.y) &&
			((stride.x * stride.x) <= (UnitStride.x * UnitStride.x))
			)
		{//upgraded Move to kill//need change
			if (CheckUpgradedMove(currentPos, destination, enemy, Up_Kill_Path) == true)
			{
				CheckUnitPicked target = enemy.UnitExist(GetTarget(currentPos, destination));
				std::cout << target.IfValid << "; Upgraded Move to kill" << std::endl;
				if (target.IfValid > 0)
				{
					ChangeDisplayPos(result.xIndex, stride);
					enemy.UnitsPos.erase(std::next(enemy.UnitsPos.begin(), target.xIndex), std::next(enemy.UnitsPos.begin(), target.xIndex + 4));
					ChangeVertices(result.whichUnit, stride);
					enemy.DeleteDisplayPos(target.whichUnit);
					Upgrade(destination, result.xIndex + 3);
					std::cout << "\n------Kill" << std::endl;
					ClickCount -= 2;
					return;
				}
			}
		}
		if (UnitStride == UpgradedStride && (stride.y * stride.y) <= (UnitStride.y * UnitStride.y) &&
			(stride.x * stride.x) <= (UnitStride.x * UnitStride.x)
			&& CheckUpgradedMove(currentPos, destination, enemy, Up_Move_Path) == true)//upgradedMove 8,8
		{
			ChangeDisplayPos(result.xIndex, stride);
			ChangeVertices(result.whichUnit, stride);
			Upgrade(destination, result.xIndex + 3);
			std::cout << "\n------UpgradedMove" << std::endl;
			ClickCount += 1;
			return;
		}
	}
	std::cout << "Unvalid Move!" << std::endl;
	ClickCount -= 2;
	return;
}

std::string CheckWin(const Team& firstT, const Team& secondT)
{
	//std::cout << "Checking !!!" << std::endl;
	int AliveCount = 0;
	for (int i = 0; i < firstT.UnitsPos.size(); i = i + 4)
	{
		//std::cout << "Checking Yellow" << std::endl;
		if (firstT.UnitsPos[i] != 0 && firstT.UnitsPos[i + 1] != 0)
		{
			AliveCount += 1;
			break;
		}
	}
	if (AliveCount == 0)
	{
		std::cout << firstT.name << std::endl;
		return secondT.name;
	}

	AliveCount = 0;
	for (int i = 0; i < secondT.UnitsPos.size(); i = i + 4)
	{
		//std::cout << "Checking Grey" << std::endl;
		if (secondT.UnitsPos[i] != 0 && secondT.UnitsPos[i + 1] != 0)
		{
			AliveCount += 1;
			break;
		}
	}
	if (AliveCount == 0)
	{
		std::cout << secondT.name << std::endl;
		return firstT.name;
	}

	return "";
}