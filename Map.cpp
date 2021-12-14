/**
 * File - Map.cpp
 * Author - Group 9
 * Description - An object to handle a vector of junctions
 */
#include "Map.h"

Map::Map(int height, int width)
{
	Junction emptyJunction = Junction::Junction();
	this->width = width;
	this->height = height;
	std::vector<std::vector<Junction>> map(height, std::vector<Junction>(width));
	this->map = map;
}

Map::Map(std::vector<std::vector<Junction>> map)
{
	this->map = map;
	this->width = map[0].size();
	this->height = map.size();
}

Map::Map()
{
	this->width = 2;
	this->height = 2;
	std::vector<std::vector<Junction>> map(this->width, std::vector<Junction>(this->height));
	this->map = map;
}

Map::~Map()
{
}

Junction* Map::getMapJunction(int y, int x) {
	return &this->map[y][x];
}
void Map::addJunction(Junction junction, int posY, int posX)
{
	this->map[posY][posX] = junction;
}

void Map::removeJunction(int posX, int posY)
{
	this->map[posY][posX] = *new Junction();
}

Junction Map::getMiddle() {
	int middle = (this->width * this->height) / 2;
	if (middle == 0) {
		return this->map[0][0];
	}
	//std::cout << "middle=" << middle << std::endl;
	//std::cout <<"y="<< middle / this->height << std::endl;
	//std::cout <<"x="<< middle % this->height << std::endl;

	return this->map[middle / this->height][middle % this->height];
}

void Map::initialiseSpawns() {
	for (int i = 0; i < this->height; i++){
		for (int j = 0; j < this->width; j++) {
			if (this->map[i][j].getSpawnable().first == true) {
				for (int k = 0; k < this->map[i][j].getSpawnable().second.size(); k++) {
					std::pair<int, int> coords;
					coords.first = i;
					coords.second = j;
					std::pair<std::pair<int, int>, int> temp;
					temp.first = coords;
					temp.second = this->map[i][j].getSpawnable().second[k];
					this->spawns.push_back(temp);
				}
			}
		}
	}
}

std::pair<std::vector<Junction>, std::vector<int>> Map::pathfinder(Junction start, int entryPoint, Junction goal, int exitPoint) {
	std::pair<std::vector<Junction>, std::vector<int>> solution;
	int bound = this->fValue({ start }, goal);
	//std::cout << "bound = " << bound << std::endl;
	//std::cout << "start = " << start.getIdentifier() << std::endl;
	//std::cout << "goal = " << goal.getIdentifier() << std::endl;
	while (true)
	{
		
		solution = this->aStar({ start }, {}, entryPoint, goal, exitPoint, bound);
		if (solution.first.size()!=0) {
			return solution;
		}
		else {
			//std::cout << "bound = " << bound << std::endl;
			bound = *std::min_element(biggerBounds.begin(), biggerBounds.end());
			biggerBounds = {};
		}
	}
	return solution;
}

std::pair<std::vector<Junction>, std::vector<int>> Map::aStar(std::vector<Junction> path, std::vector<int> exits, int entryPoint, Junction goal, int exitPoint, int bound)
{
	int fValue;
	std::pair<std::vector<Junction>, std::vector<int>> solution;
	if (path[path.size() - 1].getIdentifier() == goal.getIdentifier()) {
		//std::cout << "found" << std::endl;
		exits.push_back(exitPoint);
		std::reverse(exits.begin(), exits.end());
		return std::make_pair(path, exits);
	}
	std::pair<std::vector<Junction>, std::vector<int>> nextMoves = possibleMoves(path[path.size() - 1], entryPoint);
	for (int i = 0; i < nextMoves.first.size(); i++) {
	https://coduber.com/how-to-check-if-vector-contains-a-given-element-in-cpp/
		if (std::find(path.begin(), path.end(), nextMoves.first[i]) == path.end()) {
			std::vector<int> newExits = exits;
			std::vector<Junction> newPath = path;
			newExits.push_back(nextMoves.second[i]);
			newPath.push_back(nextMoves.first[i]);
			fValue = this->fValue(path, goal);
			//std::cout << "path-1 = " << path[path.size() - 1].getIdentifier() << std::endl;
			
			if (fValue <= bound) {
				switch (newExits[newExits.size() - 1])
				{
				case(0):
					solution = aStar(newPath, newExits, 1, goal, exitPoint, bound);
					break;
				case(1):
					solution = aStar(newPath, newExits, 0, goal, exitPoint, bound);
					break;
				case(2):
					solution = aStar(newPath, newExits, 3, goal, exitPoint, bound);
					break;
				case(3):
					solution = aStar(newPath, newExits, 2, goal, exitPoint, bound);
					break;
				}
				if (!solution.first.empty()) {
					return solution;
				}
			}
			else {
				//std::cout << "fvalue = " << fValue << std::endl;

				biggerBounds.push_back(fValue);
			}
			


		}
	}
	return std::pair<std::vector<Junction>, std::vector<int>>();
}



std::pair<std::vector<Junction>, std::vector<int>> Map::possibleMoves(Junction origin, int entryPoint) {
	std::vector<Junction> potentialNewJunctions;
	std::vector<int> potentialExits;
	std::vector<int> possibleTurnings;
	std::pair<int, int> originPosition = origin.getPosition();
	for (int i = 0; i < origin.getTurnings().size(); i++) {
		if (i == entryPoint) {
			continue;
		}
		if (origin.getTurning(i) == true) {
			possibleTurnings.push_back(i);
		}
	}

	for (int i = 0; i < possibleTurnings.size(); i++) {
		switch (possibleTurnings[i])
		{
		case 0:
			if (origin.getXPosition() - 1 <= this->width - 1 && origin.getXPosition() - 1 >= 0 && this->map[origin.getYPosition()][origin.getXPosition()-1].getType() != RoadType::N) {
				potentialNewJunctions.push_back(this->map[origin.getYPosition()][origin.getXPosition() - 1]);
				potentialExits.push_back(0);

			}			
			break;
		case 1:
			if (origin.getXPosition() + 1 <= this->width - 1 && origin.getXPosition() + 1 >= 0 && this->map[origin.getYPosition()][origin.getXPosition()+1].getType() != RoadType::N) {
				potentialNewJunctions.push_back(this->map[origin.getYPosition()][origin.getXPosition() + 1]);
				potentialExits.push_back(1);
			}
			break;
		case 2:
			if (origin.getYPosition() - 1 <= this->height - 1 && origin.getYPosition() - 1 >= 0 && this->map[origin.getYPosition()-1][origin.getXPosition()].getType() != RoadType::N) {
				potentialNewJunctions.push_back(this->map[origin.getYPosition()-1][origin.getXPosition()]);
				potentialExits.push_back(2);
			}
			break;
		case 3:
			if (origin.getYPosition() + 1 <= this->height-1 && origin.getYPosition() + 1 >= 0 && this->map[origin.getYPosition()+1][origin.getXPosition()].getType() != RoadType::N) {
				potentialNewJunctions.push_back(this->map[origin.getYPosition() + 1][origin.getXPosition()]);
				potentialExits.push_back(3);
			}
			break;
		default:
			break;
		}
		
	}
	return std::make_pair(potentialNewJunctions, potentialExits);
}

int Map::fValue(std::vector<Junction> path, Junction goal)
{
	int h;
	h = std::abs(path[path.size() - 1].getXPosition() - goal.getXPosition()) + std::abs(path[path.size() - 1].getYPosition() - goal.getYPosition());
	return path.size()-1+h;
}
