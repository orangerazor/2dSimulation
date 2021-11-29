//
// Created by Luke Marden on 06/10/2021.
//

#include "Junction.h"

Junction::Junction() {
    this->name = "N";
    this->orientation = 0;
    for (int i = 0; i < turnings.size(); i++) {
        this->turnings[i] = false;
    }
    this->speedLimit = 0;
    this->type = RoadType::N;
    this->state = true;
    this->spawnable.first = false;
    this->spawnable.second = NULL;
    ////https://www.codegrepper.com/code-examples/cpp/convert+pointer+address+to+string+c%2B%2B
    //std::stringstream ss;
    //ss << &this->name;
    this->identifier = (&this->name)->c_str();
}

Junction::Junction(std::string name, int orientation, int speedLimit, glm::mat4 rotation, RoadType type) {
    this->name = name;
    this->orientation = orientation;
    switch(type) {
    case(RoadType::S):
        switch (orientation) {
        case(0):
        case(2):
            this->turnings[0] = false;
            this->turnings[1] = false;
            this->turnings[2] = true;
            this->turnings[3] = true;
            break;
        case(1):
        case(3):
        default:
            this->turnings[0] = true;
            this->turnings[1] = true;
            this->turnings[2] = false;
            this->turnings[3] = false;
            break;
        }
        break;
    case(RoadType::T):
        switch (orientation) {
        case(1):
            this->turnings[0] = true;
            this->turnings[1] = false;
            this->turnings[2] = true;
            this->turnings[3] = true;

            break;
        case(2):
            this->turnings[0] = true;
            this->turnings[1] = true;
            this->turnings[2] = true;
            this->turnings[3] = false;
            break;
        case(3):
            this->turnings[0] = false;
            this->turnings[1] = true;
            this->turnings[2] = true;
            this->turnings[3] = true;
            break;
        case(0):
        default:
            this->turnings[0] = true;
            this->turnings[1] = true;
            this->turnings[2] = false;
            this->turnings[3] = true;
            break;
        }
        break;
    case(RoadType::X):
        this->turnings[0] = true;
        this->turnings[1] = true;
        this->turnings[2] = true;
        this->turnings[3] = true;
        break;
    
    }
    this->speedLimit = speedLimit;
    this->type = type;
    this->state = true;
    for (int i = 0; i < this->turnings.size(); i++) {
        if (this->type == RoadType::S) {
            break;
        }
        if (this->turnings[i]) {
            this->trafficLights[i] = TrafficLight::TrafficLight(2);
        }
    }
    this->identifier = (&this->name)->c_str();
    this->spawnable.first = false;
    this->spawnable.second = NULL;
}

Junction::~Junction()
{
}

int Junction::getNumberTurnings(){
    int total=0;
    for(int i=0;i<turnings.size();i++){
        if(turnings[i]){
            total++;
        }
    }
    return total;
}

std::string Junction::getName() {
    return name;
}

void Junction::trafficLightFlow() {

    switch (type) {
    case(RoadType::T):
        if (state) {

            trafficLights[0].nextLight();
            trafficLights[1].nextLight();
            /*std::cout << trafficLights[0].getLights()[0] << trafficLights[0].getLights()[1] << std::endl;*/
            if (trafficLights[0].getLights()[0] && !trafficLights[0].getLights()[1] && trafficLights[0].getLights()[0] && !trafficLights[0].getLights()[1] && trafficLights[0].getTimeLeftInState()==0 && trafficLights[1].getTimeLeftInState() == 0) {

                //std::cout << "test"<<std::endl;
                //trafficLights[3].nextLight();
                this->state = false;
            }
        }
        else {
            trafficLights[3].nextLight();
            if (trafficLights[3].getLights()[0] && !trafficLights[3].getLights()[1] && trafficLights[3].getTimeLeftInState() == 0) {
                //trafficLights[0].nextLight();
                //trafficLights[1].nextLight();
                this->state = true;
            }
        }
        break;
    case(RoadType::X):
        if (state) {

            trafficLights[0].nextLight();
            trafficLights[1].nextLight();
            if (trafficLights[0].getLights()[0] && !trafficLights[0].getLights()[1] && trafficLights[0].getLights()[0] && !trafficLights[0].getLights()[1] && trafficLights[0].getTimeLeftInState() == 0 && trafficLights[1].getTimeLeftInState() == 0) {
                this->state = false;
            }
        }
        else {
            trafficLights[2].nextLight();
            trafficLights[3].nextLight();
            if (trafficLights[2].getLights()[0] && !trafficLights[2].getLights()[1] && trafficLights[3].getLights()[0] && !trafficLights[3].getLights()[1] && trafficLights[2].getTimeLeftInState() == 0 && trafficLights[3].getTimeLeftInState() == 0) {
                this->state = true;
            }
        }
        break;
    }
    

}

void Junction::calculateLines() {
    yBotSquare = m_ypos - (m_Height * 1 / 6);
    yTopSquare = m_ypos + (m_Height * 1 / 6);
    xLeftSquare = m_xpos - (m_Width * 1 / 6);
    xRightSquare = m_xpos + (m_Width * 1 / 6);

    leftInner = m_xpos + (m_Width * 1 / 6);
    rightInner = m_xpos - (m_Width * 1 / 6);
    topInner = m_ypos - (m_Height * 1 / 6);
    botInner = m_ypos + (m_Height * 1 / 6);
}

bool Junction::nullJunction()
{
    if (type == RoadType::N) {
        return true;
    }
    else {
        return false;
    }
}

//int main(){
//    Junction* testJunction = new Junction("test", true, true, false, true, 60);
//    std::cout << testJunction->getNumberTurnings();
//
//
//}
