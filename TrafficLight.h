/**
 * File - TrafficLight.h
 * Author - Group 9
 * Description - An object to handle a traffic light
 */

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
 /**
  * Class to handle traffic lights
  *
  */
class TrafficLight : public Sprite {
private:
    //what lights to show
    std::array<bool, 3> lights; //[red, amber, green]
    //number of phases of the traffic lights, the current phase of the lights, how long a phase lasts, and the current time in the traffic lights
    int num_of_phases, current_phase, phase_duration, time;

protected:
    //where all the textures are stored
	Sprite lightsSprite[4]; // red, redAndAmber, green, amber
    //phase 0 texture
    static GLuint texRed;
    //phase 1 texture
    static GLuint texRedAmb;
    //phase 2 texture
    static GLuint texGreen;
    //phase 3 texture
    static GLuint texAmb;
public:

    /**
     * A constructor used to make a new traffic light and recycle textures
     * @param old - the old traffic light
     */
    TrafficLight(TrafficLight *old);
    /**
     * A constructor used to make a new traffic light and set the green duration
     * @param green_duration - how long a traffic light stays green
     */
    TrafficLight(int green_duration);
    /**
     * the default constructor used to make a new traffic light
     */
    TrafficLight();
    /**
     * used to tell if the traffic light is in phase 0
     * @returns if the light is only red
     */
    bool isRed();
    /**
     * used to tell if the traffic light is in phase 2 and the cars can go
     * @returns if the light is green
     */
	bool isGreen();
    /**
     * used to tell if the amount of time a traffic light has left in that phase
     * @returns time left
     */
    int getTimeLeftInState();
    /**
    * initialises all the different textures for a traffic light
    * @param shader - used to add colour to the texture
    * @param colour - a base colour
    * @param phaseOne - the texture location for the first phase
    * @param pahseTwo - the texture location for the second phase
    * @param phaseThree - the texture location for the third phase
    * @param phaseFour - the texture location for the fourth phase
    */
	void InitLights(Shader& shader, float colour[3], std::string phaseOne, std::string phaseTwo, std::string phaseThree, std::string phaseFour);
    /**
    * renders the traffic light
    * @param shader - used to add colour to the texture
    * @param ModelViewMatrix - the model view matrix
    * @param ProjectionMatrix - the projection matrix
    * @param ModelMatrix - the model matrix
    * @param hour - current hour of the day and night cycle
    */
    void Render(Shader& shader, glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix, glm::mat4& ModelMatrix, int hour);

    /**
    * works out the next light to show
    */
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
