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

protected:
    static GLuint texRed;
    static GLuint texRedAmb;
    static GLuint texGreen;
    static GLuint texAmb;
public:
    //static void initTrafficTex();

    TrafficLight(int green_duration);
    TrafficLight(TrafficLight* old);
    TrafficLight();
    ~TrafficLight();
    bool isRed();
    //void Render(Shader& shader, glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix);

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

    inline int getTimeLeftInState() {
        //std::cout << this->other_duration - this->seconds_other << std::endl;
        return this->other_duration-this->seconds_other;
    }
};


#endif //GROUP9_TRAFFICLIGHT_H
