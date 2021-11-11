#include "Map.h"

Map::Map(int height, int width)
{
	this->width = width;
	this->height = height;
	std::vector<std::vector<Junction>> map(width, std::vector<Junction>(height));
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

void Map::addJunction(Junction* junction, int posX, int posY)
{
	this->map[posY][posX] = *junction;
}

void Map::removeJunction(int posX, int posY)
{
	this->map[posY][posX] = *new Junction();
}

Junction Map::getMiddle() {
	int middle = (this->width * this->height) / 2;
	std::cout <<"middle="<< middle << std::endl;
	return this->map[middle / this->height][middle % this->height];
}
