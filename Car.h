#pragma once
#include "glm\glm.hpp"
#include "Junction.h"
#include "Sprite.h"

class Car : public Sprite
{
private:

public:
	Car(glm::mat4 rotation);
	glm::mat4 turn(float speed, int direction);
	glm::mat4 rotate(float speed, int direction);
	glm::mat4 move(glm::mat4& rotation, glm::vec3& position, float& speed);
	glm::mat4 moveJunction(Junction junction, int desiredTurning, glm::mat4 objectRotation, glm::vec3 pos, float speed);
	glm::mat4 drive(float speed, int direction, float angle);

	glm::vec3 rayStart;
	glm::vec3 rayDirection;
	float angle;
};