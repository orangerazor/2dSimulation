#pragma once
#include "Sprite.h"
#include "Junction.h"
#include <utility>

class Map : public Sprite {
private:
    int width;
    int height;
    std::vector<std::vector<Junction>> map;
    std::vector<std::pair<int, int>> spawns;

public:
    Map(int height, int width);
    Map(std::vector<std::vector<Junction>> map);
    Map();
    ~Map();
    void initialiseSpawns();
    void addJunction(Junction junction, int posX, int posY);
    void removeJunction(int posX, int posY);
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
    inline Junction* getMapJunction(int y, int x) {
        return &this->map[y][x];
    }
    inline void setMap(std::vector<std::vector<Junction>> map) {
        this->map = map;
    }
    inline std::vector<std::pair<int, int>> getSpawns() {
        return this->spawns;
    }
    Junction getMiddle();
};