//
// Created by Luke Marden on 06/10/2021.
//

#include "Junction.h"



Junction::Junction(std::string name, bool left, bool right, bool forward, bool backward, int speedLimit, glm::mat4 rotation, RoadType type) {
    this->name = name;
    this->turnings[0] = left;
    this->turnings[1] = right;
    this->turnings[2] = forward;
    this->turnings[3] = backward;
    this->speedLimit = speedLimit;
    this->type = type;
    this->state = true;
    for (int i = 0; i < this->turnings.size(); i++) {
        if (this->turnings[i]) {
            this->trafficLights[i] = TrafficLight::TrafficLight(2);
        }
    }


    //model.calcCentrePoint();
    //// Find coordinate inbetween centre and edge of junction
    //float coordinateX = model.theBBox.centrePoint.x + (model.theBBox.centrePoint.x + (model.theBBox.boxWidthX / 2));
    //float coordinateY = model.theBBox.centrePoint.y;// +(model.theBBox.centrePoint.y + (model.theBBox.boxWidthY / 2));
    //float coordinateZ = model.theBBox.centrePoint.z;// +(model.theBBox.centrePoint.z + (model.theBBox.boxWidthZ / 2));

    //// direction vector before rotation
    //float directionX = coordinateX;
    //float directionY = coordinateY;
    //float directionZ = coordinateZ + 1;

    //// values rotated for rotation of model
    //float xRotate = (coordinateX * rotation[0][0]) + (coordinateY * rotation[0][1]) + (coordinateZ * rotation[0][2]);
    //float yRotate = (coordinateX * rotation[1][0]) + (coordinateY * rotation[1][1]) + (coordinateZ * rotation[1][2]);
    //float zRotate = (coordinateX * rotation[2][0]) + (coordinateY * rotation[2][1]) + (coordinateZ * rotation[2][2]);

    //// values rotated for direction vector of model
    //float directionXRot = (directionX * rotation[0][0]) + (directionY * rotation[0][1]) + (directionZ * rotation[0][2]);;
    //float directionYRot = (directionX * rotation[1][0]) + (directionY * rotation[1][1]) + (directionZ * rotation[1][2]);;
    //float directionZRot = (directionX * rotation[2][0]) + (directionY * rotation[2][1]) + (directionZ * rotation[2][2]);

    //// Final direction vector
    //float directX = directionXRot - xRotate;
    //float directY = directionYRot - yRotate;
    //float directZ = directionZRot - zRotate;

    //roadStart = glm::vec3(xRotate, yRotate, zRotate);
    //roadDirection = glm::vec3(directX, directY, directZ);

    //// Calculate joining road centre line
    //float xCoord = model.theBBox.centrePoint.x + 1;
    //float yCoord = model.theBBox.centrePoint.y;
    //float zCoord = model.theBBox.centrePoint.z;

    //// Rotate start point
    //float startRotX = (model.theBBox.centrePoint.x * rotation[0][0]) + (model.theBBox.centrePoint.y * rotation[0][1]) + (model.theBBox.centrePoint.z * rotation[0][2]);
    //float startRotY = (model.theBBox.centrePoint.x * rotation[1][0]) + (model.theBBox.centrePoint.y * rotation[1][1]) + (model.theBBox.centrePoint.z * rotation[1][2]);
    //float startRotZ = (model.theBBox.centrePoint.x * rotation[2][0]) + (model.theBBox.centrePoint.y * rotation[2][1]) + (model.theBBox.centrePoint.z * rotation[2][2]);

    ////Rotate direction point
    //float joinRotX = (xCoord * rotation[0][0]) + (yCoord * rotation[0][1]) + (zCoord * rotation[0][2]);
    //float joinRotY = (xCoord * rotation[1][0]) + (yCoord * rotation[1][1]) + (zCoord * rotation[1][2]);
    //float joinRotZ = (xCoord * rotation[2][0]) + (yCoord * rotation[2][1]) + (zCoord * rotation[2][2]);

    //joinStart = glm::vec3(startRotX,startRotY,startRotZ);
    //joinDirection = glm::vec3(joinRotX - startRotX, joinRotY - startRotY, joinRotZ - startRotZ);

    /*cout << joinStart.x << "," << joinStart.y << "," << joinStart.z << endl;*/
    /*cout << joinDirection.x << "," << joinDirection.y << "," << joinDirection.z << endl;*/
    /*cout << roadDirection.x << "," << roadDirection.y << "," << roadDirection.z << endl;*/
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
}

//int main(){
//    Junction* testJunction = new Junction("test", true, true, false, true, 60);
//    std::cout << testJunction->getNumberTurnings();
//
//
//}
