#pragma once

#include "Sprite.h"


class Box : public Sprite
{
private:
	glm::vec3 rayStart;
	glm::vec3 rayDirection;
public:
	Box(glm::mat4 rotation);
	glm::mat4 turn(float speed, int direction);
	glm::mat4 rotate(float speed, int direction);
	glm::mat4 move(glm::mat4& rotation, glm::vec3& position, float& speed);

};