//
// Created by Luke Marden on 11/10/2021.
//

#ifndef GROUP9_TRAFFICLIGHT_H
#define GROUP9_TRAFFICLIGHT_H
#include <functional>
#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <thread>
#include <ctime>
#include <time.h>
#include "Sprite.h"


class TrafficLight : public Sprite {
private:
    std::array<bool, 3> lights; //[red, amber, green]
    int green_duration, num_of_phases, current_phase, seconds_green, seconds_other, other_duration;
public:
    TrafficLight(int green_duration);
    void nextLight();
    std::array<bool, 3> getLights();
    inline int getGreenDuration(){
        return this->green_duration;
    }

    inline void setGreenDuration(int green_duration){
        this->green_duration = green_duration;
    }

    inline void setLights(std::array<bool, 3> lights){
        this->lights = lights;
    }
};


#endif //GROUP9_TRAFFICLIGHT_H
