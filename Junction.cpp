/**
 * File - Junction.cpp
 * Author - Group 9
 * Description - An object to handle a Junction
 */
#include "Junction.h"

Junction::Junction(Junction* oldJ, TrafficLight* oldTL)
{
    this->m_vaoID = oldJ->m_vaoID;
    this->m_vboID[3] = oldJ->m_vboID[3];
    this->m_NumberOfVerts = oldJ->m_NumberOfVerts;

    this->m_TexName = oldJ->m_TexName;

    this->obb = oldJ->obb;
    this->collide = oldJ->collide;
    this->spawnOBB = oldJ->spawnOBB;

    this->m_Width = oldJ->m_Width;
    this->m_Height = oldJ->m_Height;

    this->m_xpos = oldJ->m_xpos;
    this->m_ypos = oldJ->m_ypos;

    this->objectRotation = oldJ->objectRotation;

    this->name = oldJ->name;
    this->type = oldJ->type;
    this->turnings = oldJ->turnings;
    for (int i = 0; i < this->turnings.size(); i++) {
        if (this->type == RoadType::S) {
            break;
        }
        if (this->turnings[i]) {
            this->trafficLights[i] = TrafficLight::TrafficLight(oldTL);
        }
    }
    this->state = true;
    this->identifier = (&this->name)->c_str();
    this->spawnable.first = false;
    this->xPosition = -1;
    this->yPosition = -1;

}

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
    this->identifier = (&this->name)->c_str();
    this->xPosition = -1;
    this->yPosition = -1;
}

Junction::Junction(std::string name, int orientation, int speedLimit, glm::mat4 rotation, RoadType type) {
    this->name = name;
    this->orientation = orientation;
    switch (type) {
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
            this->turnings[0] = false;
            this->turnings[1] = true;
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
            this->turnings[0] = true;
            this->turnings[1] = false;
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
            this->trafficLights[i] = TrafficLight::TrafficLight();
        }
    }
    this->identifier = (&this->name)->c_str();
    this->spawnable.first = false;
    this->xPosition = -1;
    this->yPosition = -1;
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

void Junction::trafficLightFlow() {

    switch (this->type) {
    case(RoadType::T):
        if (state) {
            switch (orientation)
            {

            case(1):
            case(3):
                
                trafficLights[2].nextLight();
                trafficLights[3].nextLight();
                if (trafficLights[2].getLights()[0] && !trafficLights[2].getLights()[1] && trafficLights[3].getLights()[0] && 
                    !trafficLights[3].getLights()[1] && trafficLights[2].getTimeLeftInState() == 0 && trafficLights[3].getTimeLeftInState() == 0) {
                    this->state = false;
                }
                break;
            case(0):
            case(2):
            default:
                trafficLights[0].nextLight();
                trafficLights[1].nextLight();
                if (trafficLights[0].getLights()[0] && !trafficLights[0].getLights()[1] && trafficLights[0].getLights()[0] && !trafficLights[0].getLights()[1] && trafficLights[0].getTimeLeftInState() == 0 && trafficLights[1].getTimeLeftInState() == 0) {
                    this->state = false;
                }
                break;
            }
        }
        else {
            switch (orientation) {
            case(1):
                trafficLights[1].nextLight();
                if (trafficLights[1].getLights()[0] && !trafficLights[1].getLights()[1] && trafficLights[1].getTimeLeftInState() == 0) {
                    this->state = true;
                }
                break;
            case(2):
                trafficLights[2].nextLight();
                if (trafficLights[2].getLights()[0] && !trafficLights[2].getLights()[1] && trafficLights[2].getTimeLeftInState() == 0) {
                    this->state = true;
                }
                break;
            case(3):
                trafficLights[0].nextLight();
                if (trafficLights[0].getLights()[0] && !trafficLights[0].getLights()[1] && trafficLights[0].getTimeLeftInState() == 0) {
                    this->state = true;
                }
                break;
            case(0):
            default:
                trafficLights[3].nextLight();
                if (trafficLights[3].getLights()[0] && !trafficLights[3].getLights()[1] && trafficLights[3].getTimeLeftInState() == 0) {
                    this->state = true;
                }
                break;
            }
            
        }
        break;
    case(RoadType::X):
        if (this->state) {
            trafficLights[0].nextLight();
            trafficLights[1].nextLight();
            if (trafficLights[0].getLights()[0] && !trafficLights[0].getLights()[1] && trafficLights[1].getLights()[0] && !trafficLights[1].getLights()[1] && trafficLights[0].getTimeLeftInState() == 0 && trafficLights[1].getTimeLeftInState() == 0) {
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

    leftInner = m_xpos + (m_Width * 1 / 3);
    rightInner = m_xpos - (m_Width * 1 / 3);
    topInner = m_ypos - (m_Height * 1 / 3);
    botInner = m_ypos + (m_Height * 1 / 3);
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

void Junction::setOrientation(int orientation, TrafficLight* old) {
    this->orientation = orientation;
    switch (type) {
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
            this->turnings[0] = false;
            this->turnings[1] = true;
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
            this->turnings[0] = true;
            this->turnings[1] = false;
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
    for (int i = 0; i < this->turnings.size(); i++) {
        if (this->type == RoadType::S) {
            break;
        }
        if (this->turnings[i]) {
            this->trafficLights[i] = TrafficLight::TrafficLight(old);
        }
    }
}
