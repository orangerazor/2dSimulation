#pragma once
#include "Sprite.h"
#include "Junction.h"
#include <utility>
#include <set>
#include <fstream>
#include <istream>
#include <iostream>
#include <string>
#include <algorithm>

class Map : public Sprite {
private:
    int width;
    int height;
    std::vector<std::vector<Junction>> map;
    std::vector<std::pair<std::pair<int, int>, int>> spawns;

public:
    Map(int height, int width);
    Map(std::vector<std::vector<Junction>> map);
    Map();
    ~Map();
    void initialiseSpawns();
    std::pair<std::vector<Junction>, std::vector<int>> pathfinder(std::vector<Junction> path, std::vector<int> exits, int entryPoint, Junction goal, int exitPoint);
    void addJunction(Junction junction, int posX, int posY);
    void removeJunction(int posX, int posY);
    Junction* getMapJunction(int y, int x);
    std::pair<std::vector<Junction>, std::vector<int>> possibleMoves(Junction origin, int entryPoint);

    inline int getWidth() {
        return this->width;
    }
    inline void setWidth(int width) {
        this->width = width;
    }
    inline int getHeight() {
        return this->height;
    }
    inline void setHeight(int height) {
        this->height = height;
    }
    inline std::vector<std::vector<Junction>> getMap() {
        return this->map;
    }
    
    inline void setMap(std::vector<std::vector<Junction>> map) {
        this->map = map;
    }
    inline std::vector<std::pair<std::pair<int, int>, int>> getSpawns() {
        return this->spawns;
    }
    Junction getMiddle();
};