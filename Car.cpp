#include "Car.h"
const double PI = 3.141592653589793238463;


Car::Car(glm::mat4 rotation) {
	rayStart = glm::vec3(0, 0, 0);
	rayDirection = glm::vec3(0, 1, 0);
	angle = 0;// 1.5708;
	objectRotation = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0,0,1));
	srand(time(0));
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

int Car::entryPoint()
{
	//std::cout << "Are you ever called deerie?" << std::endl;
	//convert to use a junction variable
	if (currentJunction == (*junction).getIdentifier()) {
		return entryTurning;
	}
	int entryPoint2 = 0;
	float bestDist = -1;
	for (int i = 0; i < 4; i++) {
		float xVert = INT_MAX;
		float yVert = INT_MAX;
		if (i < 3) {
			xVert = ((*junction).GetOBB().vert[i].x + (*junction).GetOBB().vert[i + 1].x) / 2;
			yVert = ((*junction).GetOBB().vert[i].y + (*junction).GetOBB().vert[i + 1].y) / 2;
		}
		else {
			xVert = ((*junction).GetOBB().vert[i].x + (*junction).GetOBB().vert[0].x) / 2;
			yVert = ((*junction).GetOBB().vert[i].y + (*junction).GetOBB().vert[0].y) / 2;
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

	//std::cout << "entrypoint 1 = " << entryPoint2 << std::endl;
	//std::cout << "orientation = " << junction->getOrientation() << std::endl;
	switch (junction->getOrientation()) {
	case 1:
		switch (entryPoint2) {
		case 0:
			entryPoint2 = 3;
			break;
		case 1:
			entryPoint2 = 2;
			break;
		case 2:
			entryPoint2 = 0;
			break;
		case 3:
			entryPoint2 = 1;
			break;
		}
		break;
	case 2:
		switch (entryPoint2) {
		case 0:
			entryPoint2 = 1;
			break;
		case 1:
			entryPoint2 = 0;
			break;
		case 2:
			entryPoint2 = 3;
			break;
		case 3:
			entryPoint2 = 2;
			break;
		}
		break;
	case 3:
		switch (entryPoint2) {
		case 0:
			entryPoint2 = 2;
			break;
		case 1:
			entryPoint2 = 3;
			break;
		case 2:
			entryPoint2 = 1;
			break;
		case 3:
			entryPoint2 = 0;
			break;
		}
		break;
	}
	entryTurning = entryPoint2;
	//std::cout << "entrypoint 2 = " << entryPoint2 << std::endl;
	//std::cout << "entry = " << entryPoint2 << std::endl;
	return entryPoint2;
}

int Car::decideDirection(int entryPoint) {

	//std::cout << "entryPoint = " << entryPoint << std::endl;

	if (currentJunction == this->junction->getIdentifier()) {
		return exit;
	}
	int numTurns = (*junction).getTurnings().size();
	std::vector<int> possibleTurnings;
	for (int i = 0; i < numTurns; i++) {
		if (i == entryPoint) {
			continue;
		}
		if ((*junction).getTurning(i) == true) {
			possibleTurnings.push_back(i);
		}
	}
	//srand(time(NULL));
	//std::cout << "possibleTurnings = " << possibleTurnings.size() << std::endl;
	int random = rand() % possibleTurnings.size();

	currentJunction = (*junction).getIdentifier();
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
	//std::cout << "direction = " << exit << std::endl;
	return random;
}

void Car::respawn(Junction *junction, int presetEntry) {
	this->junction = junction;
	//std::cout << "exitTurning = " << exitTurning << std::endl;
	switch (exitTurning) {
	case(0):
		if (m_xpos <= ((*junction).GetXPos() - ((*junction).getWidth() / 2))) {
			std::cout << "reset time " << std::endl;
			std::cout << "0" << std::endl;
			int newSpawn = this->setSpawn(presetEntry);
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
		if (m_xpos >= ((*junction).GetXPos() + ((*junction).getWidth() / 2))) {
			std::cout << "reset time " << std::endl;
			std::cout << "1" << std::endl;
			int newSpawn = this->setSpawn(presetEntry);
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
		if (m_ypos >= ((*junction).GetYPos() + ((*junction).getHeight() / 2))) {
			std::cout << "reset time " << std::endl;
			std::cout << "2" << std::endl;
			int newSpawn = this->setSpawn(presetEntry);
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
		if (m_ypos <= ((*junction).GetYPos() - ((*junction).getHeight() / 2))) {
			std::cout << "reset time " << std::endl;
			std::cout << "3" << std::endl;
			int newSpawn = this->setSpawn(presetEntry);
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
		if (presetEntry == -1) {
			entryTurning = this->setSpawn(presetEntry);
			//entryTurning = this->entryPoint();
			//std::cout << "entryTurning = " << entryTurning << std::endl;
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
		else {
			entryTurning = this->setSpawn(presetEntry);
			//entryTurning = this->entryPoint();
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
			
		}
	}
}

void Car::newSpawn(int entry) {
	switch (entry) {
	case(0):
		this->SetXpos((*junction).GetOBB().vertOriginal[0].x + ((*junction).getWidth() * 1/6));
		this->SetYpos((*junction).GetOBB().vertOriginal[0].y + ((*junction).getHeight() * 7 / 12));
		//angle += glm::radians(180.0f);
		break;
	case(1):
		this->SetXpos((*junction).GetOBB().vertOriginal[0].x + ((*junction).getWidth() * 5/6));
		this->SetYpos((*junction).GetOBB().vertOriginal[0].y + ((*junction).getHeight() * 5 / 12));
		//angle += glm::radians(180.0f);
		break;
	case(2):
		this->SetXpos((*junction).GetOBB().vertOriginal[0].x + ((*junction).getWidth() * 7 / 12));
		this->SetYpos((*junction).GetOBB().vertOriginal[0].y + ((*junction).getHeight() * 5/6));
		//angle += glm::radians(180.0f);
		break;
	case(3):
		this->SetXpos((*junction).GetOBB().vertOriginal[0].x + (((*junction).getWidth() * 5 / 12)));
		this->SetYpos((*junction).GetOBB().vertOriginal[0].y + ((*junction).getHeight() * 1 / 6));
		//angle += glm::radians(180.0f);
		break;
	}
	currentJunction = "";
}

int Car::setSpawn(int entry)
{
	
	int spawnEntrance;
	int numTurns = (*junction).getTurnings().size();
	std::vector<int> possibleTurnings;
	for (int i = 0; i < numTurns; i++) {
		if ((*junction).getTurning(i) == true) {
			possibleTurnings.push_back(i);
		}
	}
	int correctVert = 0;
	switch (junction->getOrientation()) {
	case 0:
		correctVert = 0;
		break;
	case 1:
		correctVert = 3;
		break;
	case 2:
		correctVert = 2;
		break;
	case 3:
		correctVert = 1;
		break;

	}
	
	if (entry == -1) {
		spawnEntrance = rand() % possibleTurnings.size();
	}
	else {
		spawnEntrance = entry;
	}
	std::cout << "entry = " << entry << std::endl;
	//int turningIndex = possibleTurnings.at(spawnEntrance);
	switch (entry) {
	case(0):
		this->SetXpos((*junction).GetOBB().vert[correctVert].x);
		this->SetYpos((*junction).GetOBB().vert[correctVert].y + ((*junction).getHeight() * 7 / 12));
		break;
	case(1):
		this->SetXpos((*junction).GetOBB().vert[correctVert].x + ((*junction).getWidth()));
		this->SetYpos((*junction).GetOBB().vert[correctVert].y + ((*junction).getHeight() * 5 / 12));
		break;
	case(2):
		this->SetXpos((*junction).GetOBB().vert[correctVert].x + ((*junction).getWidth() * 7 / 12));
		this->SetYpos((*junction).GetOBB().vert[correctVert].y + ((*junction).getHeight()));
		break;
	case(3):
		this->SetXpos((*junction).GetOBB().vert[correctVert].x + (((*junction).getWidth() * 5 / 12)));
		this->SetYpos((*junction).GetOBB().vert[correctVert].y);
		break;
	}
	currentJunction = "";
	
	//std::cout << "X coord car centre = " << m_xpos << std::endl;
	//std::cout << "Y coord car centre = " << m_ypos << std::endl;
	std::cout << "Set spawn = " << entry << std::endl;
	return entry;
}

void Car::mapCarRespawn(Map map)
{

	std::pair<int, int> spawnJunctionIndex = map.getSpawns()[rand() % map.getSpawns().size()];
	//std::cout << "first = " << spawnJunctionIndex.first << ", second = " << spawnJunctionIndex.second << std::endl;
	//std::cout << "identifier = " << (map.getMapJunction(spawnJunctionIndex.first, spawnJunctionIndex.second)->getIdentifier()) << std::endl;

	this->junction = (map.getMapJunction(spawnJunctionIndex.first, spawnJunctionIndex.second));
	this->respawn(this->junction, this->junction->getSpawnable().second);
	//std::cout << "thisJunc = " << this->junction->getIdentifier() << std::endl;
	//(*car).setJunction(mapClass.getMapJunction(spawnJunctionIndex.first, spawnJunctionIndex.second));
}

glm::mat4 Car::rotate(float speed, int direction, int entryPoint, float fps, std::vector<Car> collideCheck)
{
	for (int i = 0; i < collideCheck.size(); i++) {
		if (collideCheck[i] == this) {
			continue;
		}
		if (collideCheck[i].IsInCollision(collide)) {
			speed = 0;
		}
	}
	//std::cout << std::endl;
	glm::vec3 forVec2 = forVec;
	/*std::cout << "green = " << junction.getTrafficLights()[entryPoint].getLights()[2] << std::endl;*/
	if ((*junction).getType() == RoadType::T || (*junction).getType() == RoadType::X) { //for checking that the road actually has traffic lights to obey by
		switch (entryPoint) {
		case(0):
			if ((m_xpos + m_Width / 2) <= (*junction).getXLeftSquare() && m_xpos > ((*junction).getXLeftSquare() - (m_Height)) && !(*junction).getTrafficLights()[entryPoint].isGreen()) {
				//std::cout << "Bug A" << std::endl;
				speed = 0;
			}
			else if ((m_xpos+m_Width/2) >= (*junction).getXLeftSquare() + ((*junction).getWidth() * 1 / 12) && m_xpos <= (*junction).GetXPos() && (*junction).getTrafficLights()[1].getLights()[2] && direction == 1) {
				//std::cout << "Bug B" << std::endl;
				//std::cout << (*junction).getTrafficLights()[1].isGreen() << std::endl;
				speed = 0;
			}
			else {
				speed = speed;
			}
			break;
		case(1):
			if ((m_xpos - m_Width / 2) >= (*junction).getXRightSquare() && m_xpos < ((*junction).getXRightSquare() + (m_Height)) && !(*junction).getTrafficLights()[entryPoint].getLights()[2]) {
				speed = 0;
			}
			else if ((m_xpos - m_Width / 2) <= (*junction).getXRightSquare() - ((*junction).getWidth() * 1 / 12) && m_xpos >= (*junction).GetXPos() && (*junction).getTrafficLights()[0].isGreen() && direction == 1) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			break;
		case(2):
			if ((m_ypos-m_Width/2) >= (*junction).getYTopSquare() && m_ypos < ((*junction).getYTopSquare() + (m_Height)) && !(*junction).getTrafficLights()[entryPoint].getLights()[2]) {
				speed = 0;
			}
			else if ((m_ypos - m_Width / 2) <= (*junction).getYTopSquare() - ((*junction).getHeight() * 1 / 12) && m_ypos >= (*junction).GetYPos() && (*junction).getTrafficLights()[3].isGreen() && direction == 1) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			break;
		case(3):
			if ((m_ypos + m_Width / 2) <= (*junction).getYBotSquare() && m_ypos > ((*junction).getYBotSquare() - (m_Height)) && !(*junction).getTrafficLights()[entryPoint].getLights()[2]) {
				speed = 0;
			}
			else if ((m_ypos + m_Width / 2) >= (*junction).getYBotSquare() + ((*junction).getHeight() * 1 / 12) && m_ypos <= (*junction).GetYPos() && (*junction).getTrafficLights()[2].isGreen() && direction == 1) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			break;
		}
	}
	
	switch (direction) {
	case(-1):
		switch (entryPoint) {
		case(0):
			//if (m_xpos <= junction.getXLeftSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
			//	speed = 0;
			//	break;
			//}
			if (m_xpos >= (*junction).getXLeftSquare() && m_ypos <= (*junction).getYTopSquare() && speed != 0) {
				angle += 2.4f / fps;
			}
			break;
		case(1):
			//if (m_xpos >= junction.getXRightSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
			//	speed = 0;
			//	break;
			//}
			if (m_xpos <= (*junction).getXRightSquare() && m_ypos >= (*junction).getYBotSquare() && speed != 0) {
				angle += 2.4f / fps;
			}
			break;
		case(2):
			//if (m_ypos >= junction.getYTopSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
			//	speed = 0;
			//	break;
			//}
			if (m_ypos <= (*junction).getYTopSquare() && m_xpos <= (*junction).getXRightSquare() && speed != 0) {
				angle += 2.4f / fps;
			}
			break;
		case(3):
			//if (m_ypos <= junction.getYBotSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
			//	speed = 0;
			//	break;
			//}
			if (m_ypos >= (*junction).getYBotSquare() && m_xpos >= (*junction).getXLeftSquare() && speed != 0) {
				angle += 2.4f / fps;
			}
			break;
		}
		break;
	case(0):
		switch (entryPoint) {
		case(0):
			//if (m_xpos <= junction.getXLeftSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
			//	speed = 0;
			//	break;
			//}
			break;
		case(1):
			//if (m_xpos >= junction.getXRightSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
			//	speed = 0;
			//	break;
			//}
			break;
		case(2):
			//if (m_ypos >= junction.getYTopSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
			//	speed = 0;
			//	break;
			//}
			break;
		case(3):
			//if (m_ypos <= junction.getYBotSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
			//	speed = 0;
			//	break;
			//}
			break;
		}
		break;
	case(1):
		switch (entryPoint) {
		case(0):
			//if (m_xpos <= junction.getXLeftSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
			//	speed = 0;
			//	break;
			//}
			if (m_xpos >= (*junction).GetXPos() && m_ypos >= (*junction).GetYPos() && !(*junction).getTrafficLights()[1].isGreen() && speed != 0) {
				angle -= 2.4f/fps;
			}
			break;
		case(1):
			//if (m_xpos >= junction.getXRightSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
			//	speed = 0;
			//	break;
			//}
			if (m_xpos <= (*junction).GetXPos() && m_ypos <= (*junction).GetYPos() && !(*junction).getTrafficLights()[2].isGreen() && speed != 0) {
				angle -= 2.4f/fps;		
			}
			break;
		case(2):
			//if (m_ypos >= junction.getYTopSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
			//	speed = 0;
			//	break;
			//}
			if (m_ypos <= (*junction).GetYPos() && m_xpos >= (*junction).GetXPos() && !(*junction).getTrafficLights()[3].isGreen() && speed != 0) {
				angle -= 2.4f/fps;

			}
			break;
		case(3):
			//if (m_ypos <= junction.getYBotSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
			//	speed = 0;
			//	break;
			//}
			if (m_ypos >= (*junction).GetYPos() && m_xpos <= (*junction).GetXPos() && !(*junction).getTrafficLights()[2].isGreen() && speed != 0) {
				angle -= 2.4f / fps;
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
		if (m_xpos <= (*junction).getXLeftSquare() && speed != 0.0) {
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
		if (m_xpos >= (*junction).getXRightSquare() && speed != 0) {
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
		if (m_ypos >= (*junction).getYTopSquare() && speed != 0.0) {
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
		if (m_ypos <= (*junction).getYBotSquare() && speed != 0.0) {
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

Car::Car()
{
}

void Car::setCurrentJunction(std::string name) {
	currentJunction = name;
}

std::string Car::getCurrentJunction() {
	return currentJunction;
}