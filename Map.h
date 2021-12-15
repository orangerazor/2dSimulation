/**
 * File - Map.h
 * Author - Group 9
 * Description - An object to handle a vecotr of junctions
 */
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
/**
* The map class
*/
class Map : public Sprite {
private:
    int width;
    int height;
    std::vector<std::vector<Junction>> map;
    std::vector<std::pair<std::pair<int, int>, int>> spawns;
    std::vector<int> biggerBounds;

public:
    /**
    * map constructor 
    * @param height - how tall the map is
    * @param width - how wide the map is
    */
    Map(int height, int width);
    /**
    * map constructor
    * @param map - a 2d vector of junctions to be used as the map
    */
    Map(std::vector<std::vector<Junction>> map);
    /**
    * default constructor for map
    */
    Map();
    /**
    * destructor for the map class
    */
    ~Map();
    /**
    * works out what junctions and entrances can be spawned on and saves them in the spawns variable
    */
    void initialiseSpawns();
    /**
    * method to handle the iterative deepening of the a star method
    * @param start - the junction the to start the pathfinder from
    * @param entryPoint - the entry point it is entering from
    * @param goal - the end goal junction
    * @param exitPoint - the goal exit point
    * @returns - the junctions to take, and the exits to take in the form of a stack
    */
    std::pair<std::vector<Junction>, std::vector<int>> pathfinder(Junction start, int entryPoint, Junction goal, int exitPoint);
    /**
    * method to do a A star algorithm to a given bound
    * @param path - the path so far
    * @param exits - the exit point stack
    * @param entryPoint - the entry point it is entering from
    * @param goal - the end goal junction
    * @param exitPoint - the goal exit point
    * @param bound - the current deepest the a star algorithm can go to
    * @returns - the junctions to take, and the exits to take in the form of a stack
    */
    std::pair<std::vector<Junction>, std::vector<int>> aStar(std::vector<Junction> path, std::vector<int> exits, int entryPoint, Junction goal, int exitPoint, int bound);
    /**
    * method to add a junction at given indices
    * @param juncion - the junction to add
    * @param posX - the x position to add the junction
    * @param posY - the y position to add the junction
    */
    void addJunction(Junction junction, int posX, int posY);
    /**
    * method to remove a junction at given indices
    * @param posX - the x position to remove the junction
    * @param posY - the y position to remove the junction
    */
    void removeJunction(int posX, int posY);
    /**
    * method to get a junction at given indices
    * @param x - the x position of the junction
    * @param y - the y position of the junction
    */
    Junction* getMapJunction(int y, int x);
    /**
    * method to get the possible junctions and exits for those junctions from a single junction
    * @param origin - the origin junction
    * @param entryPoint - the entry point into the origin junction
    * @returns - the jucntions that it can go to, and the exits it can take
    */
    std::pair<std::vector<Junction>, std::vector<int>> possibleMoves(Junction origin, int entryPoint);
    /**
    * computes an fvalue for the pathfinder. based on the difference of indices
    * @param path - the current path
    * @param goal - the goal junction
    * @return - the f value
    */
    int fValue (std::vector<Junction> path, Junction goal);
    


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
};