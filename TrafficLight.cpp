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

TrafficLight::TrafficLight(int phase_duration){
    this->lights = {1,0,0};
    this->current_phase = 0;
    this->num_of_phases = 4;
    this->phase_duration = phase_duration;

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

void TrafficLight::Render(Shader & shader, glm::mat4 & ModelViewMatrix, glm::mat4 & ProjectionMatrix, glm::mat4& ModelMatrix)
{
    switch (this->current_phase) {
    case 0:
        lightsSprite[0].Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix);
        break;
    case 1:
        lightsSprite[1].Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix);
        break;
    case 2:
        lightsSprite[2].Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix);
        break;
    case 3:
        lightsSprite[3].Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix);
        break;
    default:
        Sprite::Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix);
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

//void TrafficLight::Render(Shader& shader, glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix)
//{
//    //std::cout << this->getHeight() << std::endl;
//    /****UPDATE THE CORNER VALUES BASED ON TRANSFORMATION***/
//    //std::cout << "noot noot" << std::endl;
//    obb.transformPoints(ModelViewMatrix);
//    //m_xpos = obb.xCentre(ModelViewMatrix);
//    //m_ypos = obb.yCentre(ModelViewMatrix);
//    //glm::vec4 vector = glm::vec4(m_xpos, m_ypos, 0.0f, 1.0f);
//    //vector = ModelViewMatrix * vector;
//    //std::cout << m_xpos << ", " << m_ypos << std::endl;
//    //m_xpos = vector.x;
//    //m_ypos = vector.y;
//    /*******************************************************/
//
//    glUseProgram(shader.handle());  // use the shader
//
//    //set the DiffuseMap in GLSL to the texture unit 0.
//    glUniform1i(glGetUniformLocation(shader.handle(), "DiffuseMap"), 0);
//
//    //if (lights[0] && !lights[1] && !lights[2]) {
//    //    glBindTexture(GL_TEXTURE_2D, texRed);
//    //}
//    //if (lights[0] && lights[1] && !lights[2]) {
//    //    glBindTexture(GL_TEXTURE_2D, texRedAmb);
//    //}
//    glBindTexture(GL_TEXTURE_2D, m_texName);
//
//    //set the uniform for the projectionmatrix
//    glUniformMatrix4fv(glGetUniformLocation(shader.handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
//
//    //pass the uniform for the ModelView matrix to the shader
//    glUniformMatrix4fv(glGetUniformLocation(shader.handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);
//
//    //Draw the object
//    glBindVertexArray(m_vaoID);		// select first VAO
//    glDrawArrays(GL_TRIANGLES, 0, m_NumberOfVerts);	// draw first object
//
//    glBindVertexArray(0); //unbind the vertex array object
//    glUseProgram(0); //turn off the current shader
//}

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
