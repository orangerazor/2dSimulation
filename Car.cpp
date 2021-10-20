#include "Car.h"
#include "glm/ext/matrix_transform.hpp"

Car::Car(glm::mat4 rotation) {
	rayStart = glm::vec3(0, 0, 0);
	rayDirection = glm::vec3(0, 0, 0);


	//// Calculate starting point for the car
	//float x = model.theBBox.centrePoint.x;
	//float y = model.theBBox.centrePoint.y;
	//float z = model.theBBox.centrePoint.z;

	////Calculate direction vector pointing out the front of the car
	//float zFront = z + 1;

	//// Rotate the points by the rotation matrix
	//float xRot = (x * rotation[0][0]) + (y * rotation[0][1]) + (z * rotation[0][2]);
	//float yRot = (x * rotation[1][0]) + (y * rotation[1][1]) + (z * rotation[1][2]);
	//float zRot = (x * rotation[2][0]) + (y * rotation[2][1]) + (z * rotation[2][2]);

	//float directRotX = (x * rotation[0][0]) + (y * rotation[0][1]) + (zFront * rotation[0][2]);
	//float directRotY = (x * rotation[1][0]) + (y * rotation[1][1]) + (zFront * rotation[1][2]);
	//float directRotZ = (x * rotation[2][0]) + (y * rotation[2][1]) + (zFront * rotation[2][2]);

	//rayStart = glm::vec3(xRot, yRot, zRot);
	//rayDirection = glm::vec3(directRotX, directRotY, directRotZ);
}

glm::mat4 Car::move(glm::mat4& objectRotation, glm::vec3& pos, float& speed) {

	pos.x += objectRotation[2][0] * speed * 20;
	pos.z += objectRotation[2][2] * speed * 20;
	return glm::translate(glm::mat4(1.0f), pos);


}

glm::mat4 Car::moveJunction(Junction junction, int desiredTurning, glm::mat4 objectRotation, glm::vec3 pos, float speed) {
	// Check if centre of the car has reached the mark to turn
	//float tVal = (model.theBBox.centrePoint.x - junction.getRoadStart().x) / junction.getRoadDirection().x;
	//if ((junction.getRoadStart().y + (tVal * junction.getRoadDirection().y) == model.theBBox.centrePoint.y) && (junction.getRoadStart().z + (tVal * junction.getRoadDirection().z) == model.theBBox.centrePoint.z)) {
	//	// Turn the car until it matches the target vector equation given by the junction class
	//	while ((rayStart != junction.getRoadStart() && rayDirection != junction.getRoadDirection())) {
	//		// Perform check to see which say it needs to rotate
	//		if (desiredTurning == 0) {
	//			//Left
	//			objectRotation = glm::rotate(objectRotation, -0.03f, glm::vec3(0, 1, 0));
	//		}
	//		if (desiredTurning == 1) {
	//			// Right
	//			objectRotation = glm::rotate(objectRotation, 0.03f, glm::vec3(0, 1, 0));
	//		}
	//	}
	//}
	//return objectRotation;
	return glm::mat4();
}
