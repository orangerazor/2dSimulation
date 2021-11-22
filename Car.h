#pragma once
#include "glm\glm.hpp"
#include "Junction.h"
#include "Sprite.h"

class Car : public Sprite
{
private:
	std::string currentJunction;
	int identifier;
	int entryTurning;
	int exitTurning = -1;
	int exit;
	float angle;
	Junction* junction;

public:
	Car();
	Car(glm::mat4 rotation);
	glm::mat4 rotate(float speed, int direction, int entryPoint, float fps, std::vector<Car> collideCheck);

	int entryPoint();
	int decideDirection(int entryPoint);
	void respawn(Junction *junction, int presetEntry = -1);
	void newSpawn(int entry);
	int setSpawn();

	void setCurrentJunction(std::string name);
	std::string getCurrentJunction();

	glm::vec3 rayStart;
	glm::vec3 rayDirection;
	glm::vec3 forVec = glm::vec3(-1.0f, 0.0f, 0.0f);

	inline bool operator==(const Car* b) {
		if (identifier != b->identifier) {
			return false;
		}
		return true;
	}
	inline bool operator!=(const Car* b) {
		if (identifier == b->identifier) {
			return true;
		}
		return false;
	}

	inline void setIdentifier(int num) {
		this->identifier = num;
	}
	inline void setAngle(float angle) {
		this->angle = angle;
	}
	inline int getIdentifier() {
		return identifier;
	}
	inline int getEntryTurning() {
		return entryTurning;
	}
	inline float getAngle() {
		return angle;
	}

	inline void setJunction(Junction *junction) {
		this->junction = junction;
	}

	inline Junction* getJunction() {
		return this->junction;
	}
};