//
// Created by Luke Marden on 11/10/2021.
//

#include "TrafficLight.h"
#include "ImageLoading.h"


TrafficLight::TrafficLight(TrafficLight* old) {
    this->m_vaoID = old->m_vaoID;
    this->m_vboID[3] = old->m_vboID[3];
    this->m_NumberOfVerts = old->m_NumberOfVerts;

    this->m_TexName = old->m_TexName;

    this->obb = old->obb;
    this->collide = old->collide;
    this->spawnOBB = old->spawnOBB;

    this->m_Width = old->m_Width;
    this->m_Height = old->m_Height;

    this->m_xpos = old->m_xpos;
    this->m_ypos = old->m_ypos;

    this->lightsSprite[0] = old->lightsSprite[0];
    this->lightsSprite[1] = old->lightsSprite[1];
    this->lightsSprite[2] = old->lightsSprite[2];
    this->lightsSprite[3] = old->lightsSprite[3];

    this->lights = old->lights;
    this->current_phase = old->current_phase;
    this->num_of_phases = old->num_of_phases;
    this->phase_duration = old->phase_duration;
    this->time = old->time;
}


TrafficLight::TrafficLight() {
    this->lights = { 1,0,0 };
    this->current_phase = 0;
    this->num_of_phases = 4;
    this->phase_duration = 2;
    this->time = 0;
}


bool TrafficLight::isRed() {
    if (this->lights[0] && !this->lights[1]) {
        return true;
    }
    return false;
}

bool TrafficLight::isGreen()
{
	return lights[2];
}

void TrafficLight::InitLights(Shader& shader, float colour[3], std::string phaseOne, std::string phaseTwo, std::string phaseThree, std::string phaseFour)
{
	for (int i = 0; i < 4; i++)
	{
		lightsSprite[i].SetWidth(this->getWidth());
		lightsSprite[i].SetHeight(this->getHeight());
	}
	lightsSprite[0].Init(shader, colour, phaseOne);
	lightsSprite[1].Init(shader, colour, phaseTwo);
	lightsSprite[2].Init(shader, colour, phaseThree);
    lightsSprite[3].Init(shader, colour, phaseFour);
}

void TrafficLight::Render(Shader & shader, glm::mat4 & ModelViewMatrix, glm::mat4 & ProjectionMatrix, glm::mat4& ModelMatrix, int hour)
{
    switch (this->current_phase) {
    case 0:
        lightsSprite[0].Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix, hour);
        break;
    case 1:
        lightsSprite[1].Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix, hour);
        break;
    case 2:
        lightsSprite[2].Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix, hour);
        break;
    case 3:
        lightsSprite[3].Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix, hour);
        break;
    default:
        Sprite::Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix, hour);
        break;
    }
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
    this->time++;
    if (this->time == this->num_of_phases * this->phase_duration) {
        this->time = 0;
    }
    this->current_phase = this->time / this->phase_duration;
}

int TrafficLight::getTimeLeftInState() {
    return this->time % this->phase_duration;
}