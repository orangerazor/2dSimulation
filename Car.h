#pragma once
#include "glm\glm.hpp"
#include "Map.h"
#include "glm/ext/matrix_transform.hpp"
#include <vector>
#include <random>
#include "Sprite.h"


class Car : public Sprite
{
private:
	std::string currentJunction;
	int identifier;
	int entryTurning;
	int exitTurning = -1;
	int direction;
	int exit;
	float angle;
	Junction* junction;
	std::string previousJunction;
	std::vector<int> path;

	bool currentlyRendered = false;

public:
	Car(Car *old);
	Car();
	Car(glm::mat4 rotation);
	glm::mat4 rotate(float speed, int direction, int entryPoint, float fps, std::vector<Car> collideCheck);

	int entryPoint();
	int decideDirection(int entryPoint);
	void respawn(Junction *junction, int presetEntry = -1);
	void newSpawn(int entry);
	int setSpawn(int entry = -1);

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
	inline int getDirection() {
		return direction;
	}
	inline float getAngle() {
		return angle;
	}

	inline void setJunction(Junction *junction) {
		this->junction = junction;
		this->currentJunction = "";
	}

	inline Junction* getJunction() {
		return this->junction;
	}

	inline void setPreviousJunction(std::string junctionIdentifier) {
		this->previousJunction = junctionIdentifier;
	}

	inline std::string getPreviousJunction() {
		return this->previousJunction;
	}

	inline bool getCurrentlyRendered() {
		return this->currentlyRendered;
	}
	inline void setCurrentlyRendered(bool yesOrNo) {
		currentlyRendered = yesOrNo;
	}

	inline void setPath(std::vector<int> path) {
		this->path = path;
	}

	inline std::vector<int> getPath() {
		return this->path;
	}
};