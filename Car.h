#pragma once
#include "glm\glm.hpp"
#include "Junction.h"
#include "Sprite.h"

class Car : public Sprite
{
private:
	glm::vec3 rayStart;
	glm::vec3 rayDirection;
public:
	Car(glm::mat4 rotation);
	glm::mat4 move(glm::mat4& rotation, glm::vec3& position, float& speed);
	glm::mat4 moveJunction(Junction junction, int desiredTurning, glm::mat4 objectRotation, glm::vec3 pos, float speed);
};