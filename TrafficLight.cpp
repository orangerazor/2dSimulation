//
// Created by Luke Marden on 11/10/2021.
//

#include "TrafficLight.h"

TrafficLight::TrafficLight(int green_duration){
    this->lights = {1,0,0};
    this->current_phase = 0;
    this->num_of_phases = 4;
    this->green_duration = green_duration;
    this->seconds_green = 0;
    this->other_duration = 2;
    this->seconds_other = 0;
}

TrafficLight::TrafficLight(TrafficLight* old) {
    this->lights = old->lights;
    this->current_phase = old->current_phase;
    this->num_of_phases = old->num_of_phases;
    this->green_duration = old->green_duration;
    this->seconds_green = old->seconds_green;
    this->other_duration = old->other_duration;
    this->seconds_other = old->seconds_other;
}

TrafficLight::TrafficLight() {
    this->lights = { 1,0,0 };
    this->current_phase = 0;
    this->num_of_phases = 4;
    this->green_duration = 2;
    this->seconds_green = 0;
    this->other_duration = 2;
    this->seconds_other = 0;
}

TrafficLight::~TrafficLight()
{

}

bool TrafficLight::isRed() {
    if (this->lights[0] && !this->lights[1]) {
        return true;
    }
    return false;
}

std::array<bool, 3> TrafficLight::getLights() {
    switch (this->current_phase) {
        case 0:
            this->lights = {1,0,0};
            break;
        case 1:
            this->lights = {1,1,0};
            break;
        case 2:
            this->lights = {0,0,1};
            break;
        case 3:
            this->lights = {0,1,0};
            break;
    }
    return this->lights;
}

void TrafficLight::nextLight() {
    if(this->current_phase == this->num_of_phases-2&&this->seconds_green<this->green_duration){
        this->seconds_green+=1;
    }
    else if(this->current_phase == this->num_of_phases-2&&this->seconds_green==this->green_duration){
        this->current_phase+=1;
        this->seconds_other=1;
    }
    else if(this->seconds_other<this->other_duration){
        this->seconds_other += 1;
    }
    else if(this->current_phase<this->num_of_phases-1){
        this->current_phase += 1;
        this->seconds_other = 1;
        if(this->current_phase == this->num_of_phases-2){
            this->seconds_green = 1;
        }
    }
    else{
        this->current_phase = 0;
        this->seconds_other = 1;
    }


}

//int test(){
//    TrafficLight* trafficLight = new TrafficLight(3);
////    std::cout << trafficLight->getLights()[0];
////    std::cout << trafficLight->getLights()[1];
////    std::cout << trafficLight->getLights()[2] << std::endl;
//
////    for(int i = 0;i<100;i++){
////        trafficLight->nextLight();
////        std::cout << trafficLight->getLights()[0];
////        std::cout << trafficLight->getLights()[1];
////        std::cout << trafficLight->getLights()[2] << std::endl;
////        std::this_thread::sleep_for(std::chrono::seconds(1));
////    }
//    while(true){
//        trafficLight->nextLight();
//        std::cout << trafficLight->getLights()[0];
//        std::cout << trafficLight->getLights()[1];
//        std::cout << trafficLight->getLights()[2] << std::endl;
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }
//}
