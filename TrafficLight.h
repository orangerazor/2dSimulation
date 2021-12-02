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
    int num_of_phases, current_phase, phase_duration, time;

protected:
	Sprite lightsSprite[4]; // red, redAndAmber, green, amber
    static GLuint texRed;
    static GLuint texRedAmb;
    static GLuint texGreen;
    static GLuint texAmb;
public:
    //static void initTrafficTex();

    TrafficLight(int green_duration);
    TrafficLight(TrafficLight* old);
    TrafficLight();
    bool isRed();
	bool isGreen();
    int getTimeLeftInState();
	void InitLights(Shader& shader, float colour[3], std::string phaseOne, std::string phaseTwo, std::string phaseThree, std::string phaseFour);
    void Render(Shader& shader, glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix, glm::mat4& ModelMatrix);

    void nextLight();
    std::array<bool, 3> getLights();

    inline void setLights(std::array<bool, 3> lights){
        this->lights = lights;
    }

    inline int getTime() {
        return this->time;
    }

    inline int getNumPhases() {
        return this->num_of_phases;
    }

    inline int getCurrentPhase() {
        return this->current_phase;
    }

    inline int getPhaseDuration() {
        return this->phase_duration;
    }


};


#endif //GROUP9_TRAFFICLIGHT_H
