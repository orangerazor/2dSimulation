//
// Created by Luke Marden on 06/10/2021.
//

#ifndef GROUP9_JUNCTION_H
#define GROUP9_JUNCTION_H
#include <functional>
#include <iostream>
#include <string>
#include <array>
#include "glm\glm.hpp"
#include "Sprite.h"


class Junction : public Sprite{
private:
    std::string name;
    std::array<bool, 4> turnings;
    int speedLimit;

    glm::vec3 roadStart;
    glm::vec3 roadDirection;
    glm::vec3 joinStart;
    glm::vec3 joinDirection;
public:


    Junction(std::string name, bool left, bool right, bool forward, bool backward, int speedLimit, glm::mat4 rotation);

    inline std::array<bool, 4> getTurnings(){
        return this->turnings;
    }

    inline int getSpeedLimit(){
        return this->speedLimit;
    }

    inline void setTurning(bool value, int index){
        this->turnings[index] = value;
    }

    inline void setSpeedLimit(int speedLimit){
        this->speedLimit = speedLimit;
    }

    inline bool getTurning(int index){
        return this->turnings[index];
    }

    inline glm::vec3 getRoadDirection() {
        return this->roadDirection;
    }
    inline glm::vec3 getRoadStart() {
        return this->roadStart;
    }

    int getNumberTurnings();

};
#endif //GROUP9_JUNCTION_H
