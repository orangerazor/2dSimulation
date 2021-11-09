#include "Map.h"

Map::Map(int width, int length)
{
	this->width = width;
	this->length = length;
	std::vector<std::vector<Junction>> map(width, std::vector<Junction>(length));
	this->map = map;

}

Map::Map(std::vector<std::vector<Junction>> map)
{
	this->map = map;
	this->width = map[0].size();
	this->length = map.size();
}

Map::Map()
{
	this->width = 2;
	this->length = 2;
}

Map::~Map()
{
}

void Map::addJunction(Junction* junction, int posX, int posY)
{
	this->map[posX][posY] = *junction;
}

void Map::removeJunction(int posX, int posY)
{
	this->map[posX][posY] = *new Junction();
}

Junction Map::getMiddle() {
	int middle = (this->width * this->length) / 2;
	std::cout <<"middle="<< middle << std::endl;
	return map[middle / length][middle % length];
}
