//
// Created by Luke Marden on 06/10/2021.
//

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

enum RoadType {
    T,
    X,
    S,
    N,
};


class Junction : public Sprite {
private:
    std::string name;
    std::array<bool, 4> turnings;
    int speedLimit;
    int orientation;
    //std::unique_ptr<TrafficLight> pointer;
    //std::array<pointer, 4> trafficLights;
    //std::array<TrafficLight, 4> trafficLights;
    TrafficLight* trafficLights = new TrafficLight[4];
    //std::vector<std::unique_ptr<TrafficLight>> trafficLights;
    glm::vec3 roadStart;
    glm::vec3 roadDirection;
    glm::vec3 joinStart;
    glm::vec3 joinDirection;
    RoadType type;
    bool state;
    float yBotSquare;
    float yTopSquare;
    float xLeftSquare;
    float xRightSquare;
    float ySideCentre;
    float xVerticalCentre;
    std::pair<bool, std::vector<int>> spawnable;

    // Respawn boxes
    // Left
    float leftInner;
    // Right
    float rightInner;
    // Top
    float topInner;
    //Bottom
    float botInner;

    std::string identifier;

public:

    Junction(Junction* oldJ, TrafficLight* oldTL);
    Junction();
    Junction(std::string name, int orientation, int speedLimit, glm::mat4 rotation, RoadType type);
    ~Junction();
    std::string getName();
    void trafficLightFlow();
    int getNumberTurnings();
    void calculateLines();
    bool nullJunction();
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
    //inline std::array<pointer, 4> getTrafficLights() {
    //    return this->trafficLights;
    //}
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

   

    

};
#endif //GROUP9_JUNCTION_H
