#pragma once
#include "glm\glm.hpp"
#include "Junction.h"
#include "Sprite.h"

class Car : public Sprite
{
private:
	std::string currentJunction;
	int entryTurning;
	int exitTurning = -1;
	int exit;
	float angle;
public:
	Car(glm::mat4 rotation);
	glm::mat4 turn(float speed, int direction);
	glm::mat4 rotate(float speed, int direction, int entryPoint, Junction junction, float fps);
	glm::mat4 move(glm::mat4& rotation, glm::vec3& position, float& speed);
	glm::mat4 moveJunction(Junction junction, int desiredTurning, glm::mat4 objectRotation, glm::vec3 pos, float speed);
	glm::mat4 drive(float speed, int direction, float angle);

	int entryPoint(Junction junction);
	glm::mat4 faceJunction(int entryPoint, glm::mat4);
	int decideDirection(Junction junction, int entryPoint);
	void respawn(Junction junction);
	int setSpawn(Junction junction);

	void setCurrentJunction(std::string name);
	std::string getCurrentJunction();

	glm::vec3 rayStart;
	glm::vec3 rayDirection;
	glm::vec3 forVec = glm::vec3(-1.0f, 0.0f, 0.0f);

	inline void setAngle(float angle) {
		this->angle = angle;
	}
	inline int getEntryTurning() {
		return entryTurning;
	}
};