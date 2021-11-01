#include "Car.h"
const double PI = 3.141592653589793238463;
#include "glm/ext/matrix_transform.hpp"
#include <vector>
#include <random>

Car::Car(glm::mat4 rotation) {
	rayStart = glm::vec3(0, 0, 0);
	rayDirection = glm::vec3(0, 1, 0);
	float angle = 0;// 1.5708;
	objectRotation = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0,0,1));
	forVec = objectRotation * glm::vec4(forVec, 1.0f);



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

glm::mat4 Car::turn(float speed, int direction) {
	
	glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f,0.0f,1.0f));
	
	forVec = matrix * glm::vec4(forVec, 1.0f);

	m_xpos += forVec.x;	
	m_ypos += forVec.y;
	return glm::mat4(1.0f);


	//objectRotation = glm::rotate(objectRotation, -0.01f, glm::vec3(0, 0, 1));
	//std::cout << speed << std::endl;
	//return objectRotation;
	//return glm::translate(objectRotation, glm::vec3(0,speed,0));




	//called when arriving at a junction
	//std::cout << m_xpos << ", " << m_ypos << std::endl;
	//glm::mat4 matrix = glm::mat4(1.0f);
	//float x = m_xpos;
	//float y = m_ypos;
	//x += speed;
	//y += speed;
	//float x2 = x;
	//float y2 = y;
	//x = (cos(-0.01f) * x2) - (sin(-0.01) * y2);
	//y = (sin(-0.01f) * x2) + (cos(-0.01) * y2);
	//m_xpos = x;
	//m_ypos = y;
	//std::cout << m_xpos << ", " << m_ypos << "\n" << std::endl;

	//for (int i = 0; i < NUMVERTS; i++) {
	//	//std::cout << obb.vertOriginal[i].x << ", " << obb.vertOriginal[i].y << std::endl;
	//	float vertX = obb.vertOriginal[i].x;
	//	float vertY = obb.vertOriginal[i].y;
	//	vertX += speed;
	//	vertY += speed;
	//	float vertX2 = vertX;
	//	float vertY2 = vertY;
	//	vertX = (cos(-0.01f) * vertX2) - (sin(-0.01) * vertY2);
	//	vertY = (sin(-0.01f) * vertX2) + (cos(-0.01) * vertY2);
	//	obb.vertOriginal[i].x = vertX;
	//	obb.vertOriginal[i].y = vertY;
	//	//std::cout << obb.vertOriginal[i].x << ", " << obb.vertOriginal[i].y << "\n" << std::endl;
	//}
	//glm::vec3 vector = glm::vec3(m_xpos, m_ypos, 0);
	//matrix = glm::translate(matrix, vector);
	//return glm::rotate(matrix, -0.01f, glm::vec3(0,0,1));

	//objectRotation = glm::rotate(objectRotation, -0.01f, glm::vec3(0, 0, 1));
	//objectRotation = glm::rotate(objectRotation, 0.0f, glm::vec3(1, 0, 0));
	//objectRotation = glm::rotate(objectRotation, 0.0f, glm::vec3(0, 1, 0));
	//float x = m_xpos;
	//float y = m_ypos;
	//m_xpos += (objectRotation[0][0] * x * speed) + ((objectRotation[0][1] * y * speed));
	//m_ypos += (objectRotation[1][0] * x * speed) + ((objectRotation[1][1] * y * speed));
	//for (int i = 0; i < 4;i++) {
	//	for (int j = 0; j < 4; j++) {
	//		std::cout << objectRotation[i][j] << ", ";
	//	}
	//	std::cout << "\n";
	//}
	//std::cout << "\n\n";
	//std::cout << m_xpos << ", " << m_ypos << "\n" << std::endl;
	//glm::vec3 vector = glm::vec3(m_xpos, m_ypos, 0);
	//return glm::translate(glm::mat4(1.0f), vector);

//	switch (direction){
//	case(-1):
//		objectRotation = glm::rotate(objectRotation, -0.01f, glm::vec3(0, 1, 0));
//		break;
//	case(0):
//		objectRotation = glm::rotate(objectRotation, 0.0f, glm::vec3(0, 1, 0));
//		break;
//	case(1):
//		objectRotation = glm::rotate(objectRotation, 0.01f, glm::vec3(0, 1, 0));
//		break;
//	default:
//		break;
//	}
//	m_xpos += objectRotation[2][0] * speed;
//	m_ypos += objectRotation[2][2] * speed;
//	glm::vec3 vector = glm::vec3(m_xpos, m_ypos, 0);
//	return glm::translate(glm::mat4(1.0f), vector);
	return glm::mat4(1.0f);
}

int Car::entryPoint(Junction junction)
{
	if (currentJunction == junction.getName()) {
		return entryTurning;
	}
	int entryPoint2 = 0;
	float bestDist = -1;
	for (int i = 0; i < 4; i++) {
		float xVert = INT_MAX;
		float yVert = INT_MAX;
		if (i < 3) {
			xVert = (junction.GetOBB().vertOriginal[i].x + junction.GetOBB().vertOriginal[i + 1].x) / 2;
			yVert = (junction.GetOBB().vertOriginal[i].y + junction.GetOBB().vertOriginal[i + 1].y) / 2;
		}
		else {
			xVert = (junction.GetOBB().vertOriginal[i].x + junction.GetOBB().vertOriginal[0].x) / 2;
			yVert = (junction.GetOBB().vertOriginal[i].y + junction.GetOBB().vertOriginal[0].y) / 2;
		}
		//std::cout << xVert << ", " << yVert << std::endl;
		float distanceFromEdge = ((m_xpos - xVert) * (m_xpos - xVert)) + ((m_ypos - yVert) * (m_ypos - yVert));
		if (bestDist == -1 || distanceFromEdge < bestDist) {
			bestDist = distanceFromEdge;
			if (i == 3) {
				entryPoint2 = 0;
			}
			else if (i == 0) {
				entryPoint2 = 3;
			}
			else {
				entryPoint2 = i;
			}
		}
	}
	entryTurning = entryPoint2;
	std::cout << "entry = " << entryPoint2 << std::endl;
	return entryPoint2;
}

glm::mat4 Car::faceJunction(int entrance, glm::mat4 ModelViewMatrix) {
	switch (entrance) {
	case(0):
		return glm::rotate(ModelViewMatrix, glm::radians(-180.0f), glm::vec3(0.0, 0.0, 1.0));
		break;
	case(1):
		return glm::rotate(ModelViewMatrix, glm::radians(-0.0f), glm::vec3(0.0, 0.0, 1.0));
		break;
	case(2):
		return glm::rotate(ModelViewMatrix, glm::radians(-270.0f), glm::vec3(0.0, 0.0, 1.0));
		break;
	case(3):
		return glm::rotate(ModelViewMatrix, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
		break;
	}
}

int Car::decideDirection(Junction junction, int entryPoint) {
	if (currentJunction == junction.getName()) {
		return exit;
	}
	int numTurns = junction.getTurnings().size();
	std::vector<int> possibleTurnings;
	for (int i = 0; i < numTurns; i++) {
		if (i == entryPoint) {
			continue;
		}
		if (junction.getTurning(i) == true) {
			possibleTurnings.push_back(i);
		}
	}
	srand(time(NULL));
	int random = rand() % possibleTurnings.size();

	currentJunction = junction.getName();
	random = possibleTurnings.at(random);
	//std::cout << entryPoint << std::endl;
	//std::cout << random << std::endl;
	exitTurning = random;
	switch (entryPoint) {
	case(0):
		switch (random) {
		case(1):
			random = 0;
			break;
		case(2):
			random = -1;
			break;
		case(3):
			random = 1;
			break;
		}
		break;
	case(1):
		switch (random) {
		case(0):
			random = 0;
			break;
		case(2):
			random = 1;
			break;
		case(3):
			random = -1;
			break;
		}
		break;
	case(2):
		switch (random) {
		case(0):
			random = 1;
			break;
		case(1):
			random = -1;
			break;
		case(3):
			random = 0;
			break;
		}
		break;
	case(3):
		switch (random) {
		case(0):
			random = -1;
			break;
		case(1):
			random = 1;
			break;
		case(2):
			random = 0;
			break;
		}
		break;
	}
	//std::cout << random << std::endl;
	exit = random;
	std::cout << "direction = " << exit << std::endl;
	return random;
}

void Car::respawn(Junction junction) {
	//std::cout << "exitTurning = " << exitTurning << std::endl;
	switch (exitTurning) {
	case(0):
		if (m_xpos <= (junction.GetXPos() - (junction.getWidth() / 2))) {
			std::cout << "reset time " << std::endl;
			//std::cout << "angle = " << angle << std::endl;
			int newSpawn = this->setSpawn(junction);
			switch (newSpawn) {
			case(0):
				angle -= glm::radians(180.0f);
				break;
			case(1):
				break;
			case(2):
				angle -= glm::radians(270.0f);
				break;
			case(3):
				angle -= glm::radians(90.0f);
				break;
			}
			entryTurning = newSpawn;
		}
		break;
	case(1):
		if (m_xpos >= (junction.GetXPos() + (junction.getWidth() / 2))) {
			std::cout << "reset time " << std::endl;
			int newSpawn = this->setSpawn(junction);
			switch (newSpawn) {
			case(0):
				break;
			case(1):
				angle -= glm::radians(180.0f);
				break;
			case(2):
				angle -= glm::radians(90.0f);
				break;
			case(3):
				angle -= glm::radians(270.0f);
				break;
			}
			entryTurning = newSpawn;
		}
		break;
	case(2):
		if (m_ypos >= (junction.GetYPos() + (junction.getHeight() / 2))) {
			std::cout << "reset time " << std::endl;
			int newSpawn = this->setSpawn(junction);
			switch (newSpawn) {
			case(0):
				angle -= glm::radians(90.0f);
				break;
			case(1):
				angle -= glm::radians(270.0f);
				break;
			case(2):
				angle -= glm::radians(180.0f);
				break;
			case(3):
				break;
			}
			entryTurning = newSpawn;
		}
		break;
	case(3):
		if (m_ypos <= (junction.GetYPos() - (junction.getHeight() / 2))) {
			std::cout << "reset time " << std::endl;
			int newSpawn = this->setSpawn(junction);
			switch (newSpawn) {
			case(0):
				angle -= glm::radians(270.0f);
				break;
			case(1):
				angle -= glm::radians(90.0f);
				break;
			case(2):
				break;
			case(3):
				angle -= glm::radians(180.0f);
				break;
			}
			entryTurning = newSpawn;
		}
		break;
	default:
		this->setSpawn(junction);
		entryTurning = this->entryPoint(junction);
		//int direction = this->decideDirection(junction, entryTurning);
		switch (entryTurning) {
		case(0):
			angle += glm::radians(180.0f);
			break;
		case(1):
			break;
		case(2):
			angle += glm::radians(90.0f);
			break;
		case(3):
			angle += glm::radians(270.0f);
		}
		break;
	}
}

int Car::setSpawn(Junction junction) {
	int numTurns = junction.getTurnings().size();
	std::vector<int> possibleTurnings;
	for (int i = 0; i < numTurns; i++) {
		if (junction.getTurning(i) == true) {
			possibleTurnings.push_back(i);
		}
	}
	srand(time(NULL));
	int random = rand() % possibleTurnings.size();
	int turningIndex = possibleTurnings.at(random);
	switch (turningIndex) {
	case(0):
		this->SetXpos(junction.GetOBB().vertOriginal[0].x);
		this->SetYpos(junction.GetOBB().vertOriginal[0].y + (junction.getHeight() * 7 / 12));
		break;
	case(1):
		this->SetXpos(junction.GetOBB().vertOriginal[0].x + (junction.getWidth()));
		this->SetYpos(junction.GetOBB().vertOriginal[0].y + (junction.getHeight() * 5 / 12));
		break;
	case(2):
		this->SetXpos(junction.GetOBB().vertOriginal[0].x + (junction.getWidth() * 7 / 12));
		this->SetYpos(junction.GetOBB().vertOriginal[0].y + (junction.getHeight()));
		break;
	case(3):
		this->SetXpos(junction.GetOBB().vertOriginal[0].x + ((junction.getWidth() * 5 / 12)));
		this->SetYpos(junction.GetOBB().vertOriginal[0].y);
		break;
	}
	currentJunction = "";
	return turningIndex;
}

glm::mat4 Car::rotate(float speed, int direction, int entryPoint, Junction junction, float fps)
{
	glm::vec3 forVec2 = forVec;
	switch (direction) {
	case(-1):
		switch (entryPoint) {
		case(0):
			if (m_xpos >= junction.getXLeftSquare() && m_ypos <= junction.getYTopSquare()) {
				angle += 2.4f / fps;
			}
			break;
		case(1):
			if (m_xpos <= junction.getXRightSquare() && m_ypos >= junction.getYBotSquare()) {
				angle += 2.4f / fps;
			}
			break;
		case(2):
			if (m_ypos <= junction.getYTopSquare() && m_xpos <= junction.getXRightSquare()) {
				angle += 2.4f / fps;
			}
		case(3):
			if (m_ypos > junction.getYBotSquare() && m_xpos >= junction.getXLeftSquare()) {
				angle += 2.4f / fps;
			}
			break;
		}
		break;
	case(0):
		break;
	case(1):
		switch (entryPoint) {
		case(0):
			if (m_xpos >= junction.GetXPos() && m_ypos >= junction.GetYPos()) {
				angle -= 2.4f/fps;
			}
			break;
		case(1):
			if (m_xpos <= junction.GetXPos() && m_ypos <= junction.GetYPos()) {
				angle -= 2.4f/fps;		
			}
			break;
		case(2):
			if (m_ypos <= junction.GetYPos() && m_xpos <= junction.GetXPos()) {
				angle -= 2.4f/fps;

			}
			break;
		case(3):
			if (m_ypos >= junction.GetYPos() && m_xpos <= junction.GetXPos()) {
				angle -= 2.4f/fps;
			}
			break;
		}
		break;
	default:
		break;
	}
	glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	forVec2 = matrix * glm::vec4(forVec, 1.0f);
	m_xpos += forVec2.x * speed;
	m_ypos += forVec2.y * speed;
	glm::vec3 vector = glm::vec3(m_xpos, m_ypos, 0);
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), vector);
	if (angle > 360) {
		angle -= 360;
	}
	// Use on left test to determine if the car has turned correctly
	switch (exitTurning) {
	case(0):
		if (m_xpos <= junction.getXLeftSquare()) {
			glm::vec3 target = glm::vec3(-1.0, 0.0, 0.0);
			float area = (forVec2.x * target.y) - (target.x * forVec2.y);
			if (area < 0) {
				angle -= 1.2/fps;
			}
			if (area > 0) {
				angle += 1.2 / fps;
			}
		}
		break;
	case(1):
		if (m_xpos >= junction.getXRightSquare()) {
			glm::vec3 target = glm::vec3(1.0, 0.0, 0.0);
			float area = (forVec2.x * target.y) - (target.x * forVec2.y);
			if (area < 0) {
				angle -= 1.2 / fps;
			}
			if (area > 0) {
				angle += 1.2 / fps;
			}
		}
		break;
	case(2):
		if (m_ypos >= junction.getYTopSquare()) {
			glm::vec3 target = glm::vec3(0.0, 1.0, 0.0);
			float area = (forVec2.x * target.y) - (target.x * forVec2.y);
			if (area < 0) {
				angle -= 1.2 / fps;
			}
			if (area > 0) {
				angle += 1.2 / fps;
			}
		}
		break;
	case(3):
		if (m_ypos <= junction.getYBotSquare()) {
			glm::vec3 target = glm::vec3(0.0, -1.0, 0.0);
			float area = (forVec2.x * target.y) - (target.x * forVec2.y);
			if (area < 0) {
				angle -= 1.2 / fps;
			}
			if (area > 0) {
				angle += 1.2 / fps;
			}
		}
		break;
	}
	return glm::rotate(transform, angle, glm::vec3(0, 0, 1));

	//switch (exitTurning) {
	//case(0):
	//	switch (direction) {
	//	case(-1):
	//		if (angle > glm::radians(90.0f) && m_xpos <= junction.getXLeftSquare()) {
	//			angle = glm::radians(90.0f);
	//		}
	//		break;
	//	case(0):
	//		break;
	//	case(1):
	//		if (angle < glm::radians(90.0f) && m_xpos <= junction.getXLeftSquare()) {
	//			angle = glm::radians(90.0f);
	//		}
	//		break;
	//	}
	//	break;
	//case(1):
	//	switch (direction) {
	//	case(-1):
	//		if (angle < glm::radians(90.0f) && m_xpos <= junction.getXLeftSquare()) {
	//			angle = glm::radians(90.0f);
	//		}
	//		break;
	//	}
	//	break;
	//}
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
glm::mat4 Car::drive(float speed, int direction, float angle) {
	//switch (direction) {
	//		case(-1):
	//			angle -= 0.01;
	//			break;
	//		case(0):
	//			angle = -PI / 2;
	//			break;
	//		case(1):
	//			angle += 0.01;
	//			break;
	//		default:
	//			break;
	//		}

	//return glm::translate(glm::mat4(1.0f), glm::vec3(m_xpos -m_xpos, m_ypos -m_ypos, 0));
	glm::mat4 translateRotate = glm::mat4(1.0f);
	//translateRotate[0][0] = cos(angle);
	//translateRotate[1][0] = -sin(angle);
	//translateRotate[3][0] = (cos(angle) * (-m_xpos)) + (sin(angle) * (-m_ypos));
	//translateRotate[0][1] = sin(angle);
	//translateRotate[1][1] = cos(angle);
	//translateRotate[3][1] = (sin(angle) * (-m_xpos)) + (cos(angle) * (-m_ypos));
	//translateRotate[3][0] = 0;
	//translateRotate[3][1] = 0;
	//for (int i = 0; i < 4;i++) {
	//	for (int j = 0; j < 4; j++) {
	//		std::cout << translateRotate[j][i] << ", ";
	//	}
	//	std::cout << "\n";
	//}
	//std::cout << "\n\n";
	//return translateRotate;

	float x = m_xpos;
	float y = m_ypos;

	rayStart = glm::vec3((m_xpos), (m_ypos), 0);
	std::cout << m_xpos << ", " << m_ypos << std::endl;
	glm::vec3 rayEnd = glm::vec3((m_xpos * cos(angle)) - ((m_ypos + 1) * sin(angle)), (m_xpos * sin(angle)) + ((m_ypos + 1) * cos(angle)), 0);
	//rayStart = glm::vec3(0, 0, 0);
	//glm::vec3 rayEnd = glm::vec3((0 * cos(angle)) - (0 + 1 * sin(angle)), (0 * sin(angle)) + (0 + 1 * cos(angle)), 0);
	rayDirection = rayEnd - rayStart;
	//std::cout << rayEnd.x << ", " << rayEnd.y << std::endl;
	glm::mat4 matrix = glm::mat4(1.0f);
	// Orbit order
	matrix[0][0] = cos(angle);
	matrix[1][0] = (sin(angle) * -1);
	matrix[3][0] = m_xpos;
	//matrix[3][0] = m_xpos + (rayStart.x + (speed * rayDirection.x));
	matrix[0][1] = sin(angle);
	matrix[1][1] = cos(angle);
	matrix[3][1] = m_ypos;
	//matrix[3][1] = m_ypos + (rayStart.y + (speed * rayDirection.y));
	//std::cout << speed << std::endl;
	//std::cout << rayStart.x + (speed * rayDirection.x) << ", " << rayStart.y + (speed * rayDirection.y) << "\n\n" << std::endl;

	//m_xpos = rayStart.x + (speed * rayDirection.x);
	//m_ypos = rayStart.y + (speed * rayDirection.y);
	//m_xpos += 0.001f;
	glBegin(GL_TRIANGLES);
		glVertex2f(0, 0);
		glVertex2f(0.6, 0.6);
		glVertex2f(0, 0.2);
		//glVertex2f(rayStart.x, rayStart.y);
		//glVertex2f(rayEnd.x, rayEnd.y);
	glEnd();

	//glm::vec3 fuck = matrix * glm::vec4(rayStart, 1);
	//glm::vec3 off = matrix * glm::vec4(m_xpos, m_ypos + 1, 0, 1);

	//m_xpos = fuck.x + (speed * off.x);
	//m_ypos = fuck.y + (speed * off.y);

	//std::cout << matrix[3][0] << ", " << matrix[3][1] << std::endl;
	//m_xpos += matrix[3][0];
	//m_ypos += matrix[3][1];
	//m_xpos += speed / 400 * rayEnd.x;
	//m_ypos += speed / 400 * rayEnd.y;
	//m_xpos = (x * cos(angle)) - (y * sin(angle));
	//m_ypos = (x * sin(angle)) + (y * cos(angle));

	// Other order
	//matrix[0][0] = cos(angle);
	//matrix[1][0] = -sin(angle);
	//matrix[3][0] = (cos(angle) * (rayStart.x + (speed * rayDirection.x))) + ( (sin(angle) * -1) * (rayStart.y + (speed * rayDirection.y)) );
	//matrix[0][1] = sin(angle);
	//matrix[1][1] = cos(angle);
	//matrix[3][1] = (sin(angle) * (rayStart.x + (speed * rayDirection.x))) + (cos(angle) * (rayStart.y + (speed * rayDirection.y)));

	//Translate back then forward
	//matrix[0][0] = cos(angle);
	//matrix[1][0] = -sin(angle);
	//matrix[3][0] = (-cos(angle) * -m_xpos) + (sin(angle) * -m_ypos) + m_xpos + (rayStart.x + (speed * rayDirection.x));
	//matrix[0][1] = sin(angle);
	//matrix[1][1] = cos(angle);
	//matrix[3][1] = (-sin(angle) * -m_xpos) - (cos(angle) * -m_ypos) + m_ypos + (rayStart.y + (speed * rayDirection.y));


	//for (int i = 0; i < 4;i++) {
	//	for (int j = 0; j < 4; j++) {
	//		std::cout << matrix[j][i] << ", ";
	//	}
	//	std::cout << "\n";
	//}
	//std::cout << "\n\n";

	//std::cout << (matrix[0][0] * x) << ", " << (matrix[1][0] * y) << ", " << (matrix[3][0]) << std::endl;
	//std::cout << (matrix[0][1] * x) << ", " << (matrix[1][1] * y) << ", " << (matrix[3][1]) << std::endl;
	//std::cout << (matrix[0][1] * x) + (matrix[1][1] * y) + (matrix[3][1]) << std::endl;

	//m_xpos = (matrix[0][0] * x) + (matrix[1][0] * y) +(matrix[3][0]);
	//m_ypos = (matrix[0][1] * x) + (matrix[1][1] * y) +(matrix[3][1]);
	//return glm::translate(glm::mat4(1.0f), glm::vec3(0, speed * rayDirection.y, 0));

	//glm::mat4 test = glm::mat4(1.0f);
	//test = glm::translate(test, glm::vec3(-m_xpos, -m_ypos, 0));
	//test = glm::rotate(test, angle, glm::vec3(0, 0, 1));
	//test = glm::translate(test, glm::vec3(m_xpos, m_ypos, 0));
	//test = glm::translate(test, glm::vec3((rayStart.x + (speed * rayDirection.x)), (rayStart.y + (speed * rayDirection.y)), 0));
	//return test;

	return matrix;
}

void Car::setCurrentJunction(std::string name) {
	currentJunction = name;
}

std::string Car::getCurrentJunction() {
	return currentJunction;
}