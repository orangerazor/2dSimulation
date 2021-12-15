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
	// Identifier for the junction the car is presently on
	std::string currentJunction;
	// Identifier for the car
	int identifier;
	// The index of the entry point of the car on the current junction
	int entryTurning;
	// The exit index for the car to leave the current junction, is not set by default
	int exitTurning = -1;
	// The direction for the car on this current junction (-1 for left, 0 for straight, 1 for right)
	int direction;
	// The index of the exit for this car on the junction
	int exit;
	// The angle applied to rotate the car and determine the forward vector.
	float angle;
	// A pointer to the current Junction for the car
	Junction* junction;
	// A string for the identifier of the previous junction
	std::string previousJunction;
	// A stack of the exit indexes taken by the car on a route
	std::vector<int> path;
	// Another car identifier
	int uniqueIdentifier;
	// Boolean for if a car is currently rendered
	bool currentlyRendered = false;

public:
	Car(Car *old);
	Car();
	Car(glm::mat4 rotation);
	/**
	* Car movement function
	* @param speed        - The value for multiplying by the forward vector of the car i.e how far to move per frame
	* @param direction    - The direction the car wants to move on the junction, -1 for left, 0 for straight and 1 for right
	* @param entryPoint   - The entrance of the car onto the junction, corresponding to the boolean array turnings.
	* @param fps          - The frames per second of the program, alowing ditance moved to change with frames per second
	* @param collideCheck - A list of cars to be checked for collisions.
	* @returns            - A matrix corresponding to the rotation and movement applied to the car
	*/
	glm::mat4 rotate(float speed, int direction, int entryPoint, float fps, std::vector<Car> collideCheck);
	/**
	* Calculating the entry point for a car
	* @returns - The index of the closest entrance
	*/
	int entryPoint();
	/**
	* Deciding a direction to go based on entrance for the Car
	* @param entryPoint - The index of the closest entrance to the car
	* @returns          - An integer deciding the direction of the car (-1 for left, 0 for straight, 1 for right)
	*/
	int decideDirection(int entryPoint);
	/**
	* Respawning a car using a junction argument for which junction to spawn on
	* @param junction    - The junction for the car to spawn on
	* @param presetEntry - an integer that sets the spawn point defaulting to -1 (no specific spawn)
	*/
	void respawn(Junction *junction, int presetEntry = -1);
	void newSpawn(int entry);
	/**
	* Setting the new spawn for a car
	* @param entry    - The junction for the car to spawn on defaults to -1 where no set entry is provided
	*/
	int setSpawn(int entry = -1);

	/**
	* Setting the currentJunction for a car
	* @param name    - The name of the junction
	*/
	void setCurrentJunction(std::string name);
	std::string getCurrentJunction();

	glm::vec3 rayStart;
	glm::vec3 rayDirection;
	// The forward vector for the car upon initialisation
	glm::vec3 forVec = glm::vec3(-1.0f, 0.0f, 0.0f);

	//  Test the identifier of a car to see if they are equal
	inline bool operator==(const Car* b) {
		if (identifier != b->identifier) {
			return false;
		}
		return true;
	}
	// Opposite of equality operator
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

	inline int getUniqueIdentifier() {
		return this->uniqueIdentifier;
	}

	inline void setUniqueIdentifier(int uniqueIdentifier) {
		this->uniqueIdentifier = uniqueIdentifier;
	}


};