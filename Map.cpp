#include "Map.h"

Map::Map(int width, int length)
{
	this->width = width;
	this->length = length;

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
