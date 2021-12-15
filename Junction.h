/**
 * File - Junction.h
 * Author - Group 9
 * Description - An object to handle a Junction
 */
#ifndef GROUP9_JUNCTION_H
#define GROUP9_JUNCTION_H
#include <functional>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <sstream>
#include <utility>
#include "glm\glm.hpp"
#include "Sprite.h"
#include "TrafficLight.h"

/**
 * Enum to determine what a junctions type is
 */
enum RoadType {
    T,
    X,
    S,
    N,
};

/**
 * Class to handle junctions
 *
 */
class Junction : public Sprite {
private:
    //name of a jucntion
    std::string name;
    //what turnings a car can go down
    std::array<bool, 4> turnings; //left, right, forward, backward
    //speed limit of a junction
    int speedLimit;
    //orientation of a junction
    int orientation;
    //traffic light objects for the turnings
    TrafficLight* trafficLights = new TrafficLight[4];
    glm::vec3 roadStart;
    glm::vec3 roadDirection;
    glm::vec3 joinStart;
    glm::vec3 joinDirection;
    //the type of the junction
    RoadType type;
    //which sets of traffic lights to cycle through next
    bool state;
    //values for lines inside of the junctions middle box
    float yBotSquare, yTopSquare, xLeftSquare, xRightSquare, ySideCentre, xVerticalCentre;
    //whether the junction can be spawned on, and the ints that it can spawned on from
    std::pair<bool, std::vector<int>> spawnable;
    //position in a map
    int xPosition, yPosition;
    // Respawn boxes
    // Left
    float leftInner;
    // Right
    float rightInner;
    // Top
    float topInner;
    //Bottom
    float botInner;
    //unique identifier
    std::string identifier;

public:
    /**
     * A constructor used to recycle textures of the junction and traffic lights
     * @param oldj - the old junction
     * @param oldTL - the old traffic light
     */
    Junction(Junction* oldJ, TrafficLight* oldTL);
    /**
     * A constructor to make an empty junction of non-type
     */
    Junction();
    /**
     * A constructor used to make a new junction
     * @param name - the initial identifier
     * @param orientation - the orienation of the junction
     * @param speedLimit - the speed limit on the junction
     * @param rotation - the textures rotation
     * @param type - the type of junction that it is
     */
    Junction(std::string name, int orientation, int speedLimit, glm::mat4 rotation, RoadType type);
    /**
    * a destructor for the junction
    */
    ~Junction();
    /**
    * a method to get the name of the junction
    * @returns - the name
    */
    std::string getName();
    /**
    * A method to control the traffic lights on a junction based on the orientation and type of the junction 
    */
    void trafficLightFlow();
    /**
    * A method to get the number of turnings possible from a junction
    * @returns - the number of turnings possible
    */
    int getNumberTurnings();
    /**
    * A method to calculate all the lines within the middle of a junction
    */
    void calculateLines();
    /**
    * A method to check whether a junction is of a non-type
    * @returns - whether the junction has a type == 'N'
    */
    bool nullJunction();
    /**
    * Sets a new orientation of a junction
    * @param orientation - the new orientation
    * @param old - the old traffic light textures
    */
    void setOrientation(int orientation, TrafficLight *old);
    


    inline std::array<bool, 4> getTurnings() {
        return this->turnings;
    }

    inline int getSpeedLimit() {
        return this->speedLimit;
    }

    inline void setTurning(bool value, int index) {
        this->turnings[index] = value;
    }

    inline void setSpeedLimit(int speedLimit) {
        this->speedLimit = speedLimit;
    }

    inline bool getTurning(int index) {
        return this->turnings[index];
    }

    inline glm::vec3 getRoadDirection() {
        return this->roadDirection;
    }
    inline glm::vec3 getRoadStart() {
        return this->roadStart;
    }
    inline float getYBotSquare() {
        return this->yBotSquare;
    }
    inline float getXLeftSquare() {
        return this->xLeftSquare;
    }
    inline float getYTopSquare() {
        return this->yTopSquare;
    }
    inline float getXRightSquare() {
        return this->xRightSquare;
    }
    inline float getLeftInner() {
        return this->leftInner;
    }
    inline float getRightInner() {
        return this->rightInner;
    }
    inline float getTopInner() {
        return this->topInner;
    }
    inline float getBotInner() {
        return this->botInner;
    }

    inline TrafficLight* getTrafficLights() {
        return this->trafficLights;
    }

    inline int getOrientation() {
        return this->orientation;
    }

    inline RoadType getType() {
        return this->type;
    }

    inline std::string getIdentifier(){
        return this->identifier;
    }

    inline void setIdentifier(std::string identifier) {
        this->identifier = identifier;
    }

    inline void setSpawnable(bool spawnable, std::vector<int> entrance) {
        this->spawnable.first = spawnable;
        this->spawnable.second = entrance;
    }

    inline std::pair<bool, std::vector<int>> getSpawnable() {
        return this->spawnable;
    }

    inline void setXPosition(int x) {
        this->xPosition = x;
    }

    inline void setYPosition(int y) {
        this->yPosition = y;
    }

    inline int getXPosition() {
        return this->xPosition;
    }

    inline int getYPosition() {
        return this->yPosition;
    }

    inline void setPosition(int x, int y) {
        this->xPosition = x;
        this->yPosition = y;
    }

    inline std::pair<int, int> getPosition() {
        return std::make_pair(this->xPosition, this->yPosition);
    }
    bool operator==(const Junction& j) const
    {
        return identifier == j.identifier;
    }
   
    
    

};
bool operator==(Junction& j1, Junction& j2);
#endif //GROUP9_JUNCTION_H
