#include "Car.h"
const double PI = 3.141592653589793238463;


Car::Car(Car* old)
{
	this->m_vaoID = old->m_vaoID;
	this->m_vboID[3] = old->m_vboID[3];
	this->m_NumberOfVerts = old->m_NumberOfVerts;

	this->m_TexName = old->m_TexName;

	this->obb = old->obb;
	this->collide = old->collide;
	this->spawnOBB = old->spawnOBB;

	this->m_Width = old->m_Width;
	this->m_Height = old->m_Height;

	this->m_xpos = old->m_xpos;
	this->m_ypos = old->m_ypos;

	this->objectRotation = old->objectRotation;

	rayStart = glm::vec3(0, 0, 0);
	rayDirection = glm::vec3(0, 1, 0);
	angle = 0;// 1.5708;
	objectRotation = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0, 0, 1));
	srand(time(0));
	forVec = objectRotation * glm::vec4(forVec, 1.0f);
	junction = &Junction();
	path = {};
	this->uniqueIdentifier = 0;
}

Car::Car(glm::mat4 rotation) {
	rayStart = glm::vec3(0, 0, 0);
	rayDirection = glm::vec3(0, 1, 0);
	angle = 0;// 1.5708;
	objectRotation = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0,0,1));
	srand(time(0));
	forVec = objectRotation * glm::vec4(forVec, 1.0f);
	junction = &Junction();
	path = {};
	this->uniqueIdentifier = 0;



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
	int exitPoint;
	if (currentJunction == this->junction->getIdentifier()) {
		return exit;
	}
	currentJunction = (*junction).getIdentifier();
	if (this->path.empty()) {
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
		exitPoint = rand() % possibleTurnings.size();
		exitPoint = possibleTurnings.at(exitPoint);
		exitTurning = exitPoint;
	}
	else {
		exitPoint = path[path.size()-1];
		exitTurning = exitPoint;
		path.pop_back();
	}
	
	switch (entryPoint) {
	case(0):
		switch (exitPoint) {
		case(1):
			exitPoint = 0;
			break;
		case(2):
			exitPoint = -1;
			break;
		case(3):
			exitPoint = 1;
			break;
		}
		break;
	case(1):
		switch (exitPoint) {
		case(0):
			exitPoint = 0;
			break;
		case(2):
			exitPoint = 1;
			break;
		case(3):
			exitPoint = -1;
			break;
		}
		break;
	case(2):
		switch (exitPoint) {
		case(0):
			exitPoint = 1;
			break;
		case(1):
			exitPoint = -1;
			break;
		case(3):
			exitPoint = 0;
			break;
		}
		break;
	case(3):
		switch (exitPoint) {
		case(0):
			exitPoint = -1;
			break;
		case(1):
			exitPoint = 1;
			break;
		case(2):
			exitPoint = 0;
			break;
		}
		break;
	}
	//std::cout << exitPoint << std::endl;
	exit = exitPoint;
	std::cout << "direction = " << exit << std::endl;
	direction = exitPoint;
	return exitPoint;
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
	glm::mat4 spawn = glm::translate(glm::mat4(1.0f), glm::vec3(m_xpos, m_ypos, 0));
	spawn = glm::rotate(spawn, angle, glm::vec3(0.0, 0.0, 1.0));
	spawnOBB.transformPoints(spawn);
}

void Car::newSpawn(int entry) {
	switch (entry) {
	case(0):
		this->SetXpos((*junction).GetOBB().vertOriginal[0].x +((*junction).getWidth() * 1 / 6));
		this->SetYpos((*junction).GetOBB().vertOriginal[0].y +((*junction).getHeight() * 7 / 12));
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
	setCurrentlyRendered(false);
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
	//std::cout << "entry = " << entry << std::endl;
	//int turningIndex = possibleTurnings.at(spawnEntrance);
	switch (entry) {
	case(0):
		this->SetXpos((*junction).GetOBB().vert[correctVert].x - (m_Height / 2));
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
	//std::cout << "Set spawn = " << entry << std::endl;
	return entry;
}

glm::mat4 Car::rotate(float speed, int direction, int entryPoint, float fps, std::vector<Car> collideCheck)
{
	for (int i = 0; i < collideCheck.size(); i++) {
		if (collideCheck[i].getUniqueIdentifier() == this->getUniqueIdentifier()) {
			continue;
		}
		if (collideCheck[i].IsInCollision(collide)) {
			//std::cout << "index of collision = " << m_xpos << ", " << m_ypos << std::endl;
			//std::cout << "Index of other car = " << collideCheck[i].GetXPos() << ", " << collideCheck[i].GetYPos() << std::endl;
			if (m_xpos < junction->getXRightSquare() && m_xpos > junction->getXLeftSquare() && m_ypos < junction->getYTopSquare() && m_ypos > junction->getYBotSquare() && junction->getType() == RoadType::X) {
				std::cout << "Obb = (" << collideCheck[i].obb.vert[0].x << ", " << collideCheck[i].obb.vert[0].y << ") (" << collideCheck[i].obb.vert[1].x << ", " << collideCheck[i].obb.vert[1].y << ") (" << collideCheck[i].obb.vert[2].x << ", "
					<< collideCheck[i].obb.vert[2].y << ") (" << collideCheck[i].obb.vert[3].x << ", " << collideCheck[i].obb.vert[3].y << ")" << std::endl;

				std::cout << "Obb 2 = (" << collide.vert[0].x << ", " << collide.vert[0].y << ") (" << collide.vert[1].x << ", " << collide.vert[1].y << ") (" << collide.vert[2].x << ", "
					<< collide.vert[2].y << ") (" << collide.vert[3].x << ", " << collide.vert[3].y << ")" << std::endl;

				std::cout << "coordinates = (" << m_xpos << ", " << m_ypos << ")\n(" << collideCheck[i].GetXPos() << ", " << collideCheck[i].GetYPos() << ")\n\n";
			}
			speed = 0;
			goto moving;
		}
		//Check to see if car infront is turning right and if so give it space to turn before moving this car and check other lane for right turners and give way to them
		float otherCarX = collideCheck[i].GetXPos();
		float otherCarY = collideCheck[i].GetYPos();
		float inFrontOrBehindX = otherCarX - m_xpos;
		float inFrontOrBehindY = otherCarY - m_ypos;
		switch (entryPoint) {
		case 0:
			// Car is in front
			if (inFrontOrBehindX > 0) {
				if (collideCheck[i].getDirection() == 1) {
					// Car in box
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->GetYPos()) {
						//Check distance
						if (inFrontOrBehindX < m_Width * 3 && m_ypos > junction->getYTopSquare()) {
							//std::cout << "Avoiding right turn in front" << std::endl;
							speed = 0;
						}
					}
					//Car is coming other way but turning right so give way to it
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->GetYPos() && otherCarY >= junction->getYBotSquare()) {
						//Don't go past the line
						if ((junction->getXLeftSquare() - m_xpos) < (m_Width * 4/3) && m_xpos + (m_Width / 2) < junction->getXLeftSquare()) {
							//std::cout << "Avoiding right turn other side of road" << std::endl;
							speed = 0;
						}
					}
				}
			}
			break;
		case 1:
			// Car is in front
			if (inFrontOrBehindX < 0) {
				if (collideCheck[i].getDirection() == 1) {
					// Car in box
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->GetYPos() && otherCarY >= junction->getYBotSquare()) {
						//Check distance
						if (inFrontOrBehindX > -m_Width * 3 && m_ypos > junction->getYBotSquare()) {
							//std::cout << "Avoiding right turn in front" << std::endl;
							speed = 0;
						}
					}
					//Car is coming other way but turning right so give way to it
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->GetYPos()) {
						//Don't go past the line
						if ((junction->getXRightSquare() - m_xpos) < (-m_Width * 4 / 3) && m_xpos - (m_Width / 2) > junction->getXRightSquare()) {
							//std::cout << "Avoiding right turn other side of road" << std::endl;
							speed = 0;
						}
					}
				}
			}
			break;
		case 2:
			// Car is in front
			if (inFrontOrBehindY < 0) {
				if (collideCheck[i].getDirection() == 1) {
					// Car in box
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->GetXPos() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->getYBotSquare()) {
						//Check distance
						if (inFrontOrBehindY > -m_Width * 3 && m_xpos < junction->getXLeftSquare()) {
							//std::cout << "Avoiding right turn in front" << std::endl;
							speed = 0;
						}
					}
					//Car is coming other way but turning right so give way to it
					if (otherCarX <= junction->GetXPos() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->getYBotSquare()) {
						//Don't go past the line
						if ((junction->getYTopSquare() - m_ypos) < (-m_Width * 4 / 3) && m_ypos - (m_Width / 2) > junction->getYTopSquare()) {
							//std::cout << "Avoiding right turn other side of road" << std::endl;
							speed = 0;
						}
					}
				}
			}
			break;
		case 3:
			// Car is in front
			if (inFrontOrBehindY > 0) {
				if (collideCheck[i].getDirection() == 1) {
					// Car in box
					if (otherCarX <= junction->GetXPos() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->getYBotSquare()) {
						//Check distance
						if (inFrontOrBehindY < m_Width * 3 && m_xpos > junction->getXRightSquare()) {
							//std::cout << "Avoiding right turn in front" << std::endl;
							speed = 0;
						}
					}
					//Car is coming other way but turning right so give way to it
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->GetXPos() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->getYBotSquare()) {
						//Don't go past the line
						if ((junction->getYBotSquare() - m_ypos) < (m_Width * 4 / 3) && m_ypos + (m_Width / 2) < junction->getYBotSquare()) {
							//std::cout << "Avoiding right turn other side of road" << std::endl;
							speed = 0;
						}
					}
				}
			}
			break;
		}
	}
	
	glm::vec3 forVec2 = forVec;
	glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	forVec2 = matrix * glm::vec4(forVec, 1.0f);
	// Use forVec2 to determine how far away a collision is in front of the car
	// Check the junction edges of the current junction but also any cars out in front
	float intersectDistTop = INT_MAX;
	float intersectDistBot = INT_MAX;
	float intersectDistRight = INT_MAX;
	float intersectDistLeft = INT_MAX;

	float intersectDistHorizontal = INT_MAX;
	float intersectDistVertical = INT_MAX;

	intersectDistRight = (junction->getXRightSquare() - m_xpos) / (forVec2.x);
	intersectDistLeft = (junction->getXLeftSquare() - m_xpos) / (forVec2.x);
	intersectDistTop = (junction->getYTopSquare() - m_ypos) / (forVec2.y);
	intersectDistBot = (junction->getYBotSquare() - m_ypos) / (forVec2.y);

	intersectDistHorizontal = (junction->GetYPos() - m_ypos) / (forVec2.y);
	intersectDistVertical = (junction->GetXPos() - m_xpos) / (forVec2.x);
	//angle += 0.4f / fps;
	switch (junction->getType()) {
	case RoadType::S:
		break;
	case RoadType::T:
		//Check for lights
		switch (entryPoint) {
		case 0:
			// If the distance in x from the car is less than the 2/3 width of the car stop unless the lights are green 
			if (abs(intersectDistLeft) < (m_Width * 4/6) && !junction->getTrafficLights()[entryPoint].getLights()[2]) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			if (m_xpos + (m_Width / 2) >= junction->getXRightSquare()) {
				speed = 12.0 / fps;
			}
			//In the box and need to turn correctly
			if (intersectDistLeft <= 0 && speed > 0.01) {
				switch (direction) {
				case -1:
					angle += 2.4 / fps;
					break;
				case 0:
					break;
				case 1:
					if (intersectDistVertical < 0 && intersectDistBot > 0) {
						angle -= 2.4 / fps;
					}
					break;
				}
			}
			break;
		case 1:
			if (abs(intersectDistRight) < (m_Width * 4/6) && !junction->getTrafficLights()[entryPoint].getLights()[2]) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			if (m_xpos - (m_Width / 2) <= junction->getXLeftSquare()) {
				speed = 12.0 / fps;
			}
			//In box so turn
			if (intersectDistRight <= 0 && speed > 0.01) {
				switch (direction) {
				case -1:
					angle += 2.4 / fps;
					break;
				case 0:
					break;
				case 1:
					if (intersectDistVertical < 0 && intersectDistTop > 0) {
						angle -= 2.4 / fps;
					}
					break;
				}
			}
			break;
		case 2:
			if (abs(intersectDistTop) < (m_Width * 4/6) && !junction->getTrafficLights()[entryPoint].getLights()[2]) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			if (m_ypos - (m_Width / 2) <= junction->getYTopSquare()) {
				speed = 12.0 / fps;
			}
			//In box so turn
			if (intersectDistTop <= 0 && speed > 0.01) {
				switch (direction) {
				case -1:
					angle += 2.4 / fps;
					break;
				case 0:
					break;
				case 1:
					if (intersectDistHorizontal < 0 && intersectDistLeft > 0) {
						angle -= 2.4 / fps;
					}
					break;
				}
			}
			break;
		case 3:
			if (abs(intersectDistBot) < (m_Width * 4/6) && !junction->getTrafficLights()[entryPoint].getLights()[2]) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			if (m_ypos + (m_Width / 2) >= junction->getYBotSquare()) {
				speed = 12.0 / fps;
			}
			if (intersectDistBot <= 0 && speed > 0.01) {
				switch (direction) {
				case -1:
					angle += 2.4 / fps;
					break;
				case 0:
					break;
				case 1:
					if (intersectDistHorizontal < 0 && intersectDistRight > 0) {
						angle -= 2.4 / fps;
					}
					break;
				}
			}
			break;
		}
		break;
	case RoadType::X:
		switch (entryPoint) {
		case 0:
			// If the distance in x from the car is less than the 2/3 width of the car stop unless the lights are green 
			if (abs(intersectDistLeft) < (m_Width * 4 / 6) && !junction->getTrafficLights()[entryPoint].getLights()[2] && (m_xpos + (m_Width / 2) < junction->getXLeftSquare()) ) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			if (m_xpos + (m_Width / 2) >= junction->getXRightSquare()) {
				speed = 12.0 / fps;
			}
			//In the box and need to turn correctly
			if (intersectDistLeft <= 0 && speed > 0.01) {
				switch (direction) {
				case -1:
					angle += 2.4 / fps;
					break;
				case 0:
					break;
				case 1:
					if (intersectDistVertical < 0 && intersectDistBot > 0) {
						angle -= 2.4 / fps;
					}
					break;
				}
			}
			break;
		case 1:
			if (abs(intersectDistRight) < (m_Width * 4 / 6) && !junction->getTrafficLights()[entryPoint].getLights()[2] && (m_xpos - (m_Width / 2) > junction->getXRightSquare()) ) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			if (m_xpos - (m_Width / 2) <= junction->getXLeftSquare()) {
				speed = 12.0 / fps;
			}
			//In box so turn
			if (intersectDistRight <= 0 && speed > 0.01) {
				switch (direction) {
				case -1:
					angle += 2.4 / fps;
					break;
				case 0:
					break;
				case 1:
					if (intersectDistVertical < 0 && intersectDistTop > 0) {
						angle -= 2.4 / fps;
					}
					break;
				}
			}
			break;
		case 2:
			if (abs(intersectDistTop) < (m_Width * 4 / 6) && !junction->getTrafficLights()[entryPoint].getLights()[2] && (m_ypos - (m_Width / 2) > junction->getYTopSquare()) ) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			if (m_ypos - (m_Width / 2) <= junction->getYTopSquare()) {
				speed = 12.0 / fps;
			}
			//In box so turn
			if (intersectDistTop <= 0 && speed > 0.01) {
				switch (direction) {
				case -1:
					angle += 2.4 / fps;
					break;
				case 0:
					break;
				case 1:
					if (intersectDistHorizontal < 0 && intersectDistLeft > 0) {
						angle -= 2.4 / fps;
					}
					break;
				}
			}
			break;
		case 3:
			if (abs(intersectDistBot) < (m_Width * 4 / 6) && !junction->getTrafficLights()[entryPoint].getLights()[2] && (m_ypos + (m_Width / 2) < junction->getYBotSquare()) ) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			if (m_ypos + (m_Width / 2) >= junction->getYBotSquare()) {
				speed = 12.0 / fps;
			}
			if (intersectDistBot <= 0 && speed > 0.01) {
				switch (direction) {
				case -1:
					angle += 2.4 / fps;
					break;
				case 0:
					break;
				case 1:
					if (intersectDistHorizontal < 0 && intersectDistRight > 0) {
						angle -= 2.4 / fps;
					}
					break;
				}
			}
			break;
		}
		break;
	case RoadType::N:
		break;
	}

	//Check to see if any line collisions are imminent in front of the car
	//In the middle box or not
	//if (m_xpos <= junction->getXRightSquare() && m_xpos >= junction->getXLeftSquare() &&
	//	m_ypos <= junction->getYTopSquare() && m_ypos >= junction->getYBotSquare()
	//	) {
	//	switch (entryPoint) {
	//	case 0:
	//		switch (direction) {
	//		case -1:
	//			if (abs(intersectDistVertical) < 2 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			break;
	//		case 0:
	//			if (abs(intersectDistTop) < 2 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			break;
	//		case 1:
	//			if (abs(intersectDistTop) < 2 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(intersectDistHorizontal) < 2 && m_xpos < junction->GetXPos() && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(intersectDistRight) < 2 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(intersectDistVertical) < 2 && m_xpos > junction->GetXPos() && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			break;
	//		}
	//		break;
	//	case 1:
	//		switch (direction) {
	//		case -1:
	//			if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(intersectDistVertical) < 2 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			break;
	//		case 0:
	//			if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(intersectDistBot) < 2 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			break;
	//		case 1:
	//			if (abs(intersectDistBot) < 2 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(intersectDistHorizontal) < 2 && m_xpos > junction->GetXPos() && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(intersectDistLeft) < 2 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(intersectDistVertical) < 2 && m_xpos < junction->GetXPos() && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			break;
	//		}
	//		break;
	//	case 2:
	//		switch (direction) {
	//		case -1:
	//			if (abs(intersectDistVertical) < 2 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			break;
	//		case 0:
	//			if (abs(intersectDistRight) < 2 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(intersectDistVertical) < 2 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			break;
	//		case 1:
	//			if (abs(intersectDistRight) < 2 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(intersectDistVertical) < 2 && m_ypos > junction->GetYPos() && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(intersectDistBot) < 2 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(intersectDistHorizontal) < 2 && m_ypos < junction->GetYPos() && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			break;
	//		}
	//		break;
	//	case 3:
	//		switch (direction) {
	//		case -1:
	//			if (abs(intersectDistVertical) < 2 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			break;
	//		case 0:
	//			if (abs(intersectDistLeft) < 2 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(intersectDistVertical) < 2 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			break;
	//		case 1:
	//			if (abs(intersectDistLeft) < 2 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(intersectDistVertical) < 2 && m_ypos < junction->GetYPos() && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(intersectDistTop) < 2 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(intersectDistHorizontal) < 2 && m_ypos > junction->GetYPos() && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			break;
	//		}
	//		break;
	//	}
	//}
	//else {
	//	//Left
	//	if (m_xpos <= junction->getXLeftSquare()) {
	//		float tValCentre = (junction->GetXPos() - m_xpos) / forVec2.x;
	//		if (abs(intersectDistTop) < 2 && speed > 0.01) {
	//			//Check which way the car is facing
	//			// Facing centre of junction
	//			if (tValCentre > 0) {
	//				angle -= 0.2 / fps;
	//			}
	//			//Facing out of junction
	//			else {
	//				angle += 0.2 / fps;
	//			}
	//		}
	//		if (abs(intersectDistBot) < 2 && speed > 0.01) {
	//			//Check which way the car is facing
	//			if (tValCentre > 0) {
	//				angle += 0.2 / fps;
	//			}
	//			else {
	//				angle -= 0.2 / fps;
	//			}
	//		}
	//		//Check for imminent collisions in front along road lines
	//		if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
	//			if (tValCentre > 0) {
	//				angle += 0.2 / fps;
	//			}
	//			else {
	//				angle += 0.2 / fps;
	//			}
	//		}
	//	}
	//	//Right
	//	if (m_xpos >= junction->getXRightSquare() && speed > 0.01) {
	//		float tValCentre = (junction->GetXPos() - m_xpos) / forVec2.x;
	//		if (abs(intersectDistBot) < 2) {
	//			//Check which way the car is facing
	//			// Facing centre of junction
	//			if (tValCentre > 0) {
	//				angle -= 0.2 / fps;
	//			}
	//			//Facing out of junction
	//			else {
	//				angle += 0.2 / fps;
	//			}
	//		}
	//		if (abs(intersectDistTop) < 2 && speed > 0.01) {
	//			//Check which way the car is facing
	//			if (tValCentre > 0) {
	//				angle += 0.2 / fps;
	//			}
	//			else {
	//				angle -= 0.2 / fps;
	//			}
	//		}
	//		//Check for imminent collisions in front along road lines
	//		if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
	//			if (tValCentre > 0) {
	//				angle -= 0.2 / fps;
	//			}
	//			else {
	//				angle -= 0.2 / fps;
	//			}
	//		}
	//	}
	//	//Top
	//	if (m_ypos >= junction->getYTopSquare()) {
	//		float tValCentre = (junction->GetYPos() - m_ypos) / forVec2.y;
	//		if (abs(intersectDistLeft) < 2 && speed > 0.01) {
	//			//Check which way the car is facing
	//			// Facing centre of junction
	//			if (tValCentre > 0) {
	//				angle -= 0.2 / fps;
	//			}
	//			//Facing out of junction
	//			else {
	//				angle += 0.2 / fps;
	//			}
	//		}
	//		if (abs(intersectDistRight) < 2 && speed > 0.01) {
	//			//Check which way the car is facing
	//			if (tValCentre > 0) {
	//				angle += 0.2 / fps;
	//			}
	//			else {
	//				angle -= 0.2 / fps;
	//			}
	//		}
	//		//Check for imminent collisions in front along road lines
	//		if (abs(intersectDistVertical) < 2 && speed > 0.01) {
	//			if (tValCentre > 0) {
	//				angle += 0.2 / fps;
	//			}
	//			else {
	//				angle += 0.2 / fps;
	//			}
	//		}
	//	}
	//	//Bottom
	//	if (m_ypos <= junction->getYBotSquare()) {
	//		float tValCentre = (junction->GetYPos() - m_ypos) / forVec2.y;
	//		if (abs(intersectDistLeft) < 2 && speed > 0.01) {
	//			//Check which way the car is facing
	//			// Facing centre of junction
	//			if (tValCentre > 0) {
	//				angle -= 0.2 / fps;
	//			}
	//			//Facing out of junction
	//			else {
	//				angle += 0.2 / fps;
	//			}
	//		}
	//		if (abs(intersectDistRight) < 2 && speed > 0.01) {
	//			//Check which way the car is facing
	//			if (tValCentre > 0) {
	//				angle += 0.2 / fps;
	//			}
	//			else {
	//				angle -= 0.2 / fps;
	//			}
	//		}
	//		//Check for imminent collisions in front along road lines
	//		if (abs(intersectDistVertical) < 2 && speed > 0.01) {
	//			if (tValCentre > 0) {
	//				angle += 0.2 / fps;
	//			}
	//			else {
	//				angle += 0.2 / fps;
	//			}
	//		}
	//	}
	//}

	//Check vector perpendicular to forward vector to make adjustments to the car i.e move to the centre of the road
	// Get perpendicular vector with any magnitude
	//float xPerp = forVec2.y / forVec2.x;
	//float perpMag = sqrt((xPerp * xPerp) + 1.0);
	//float forVecMag = sqrt((forVec2.x * forVec2.x) + (forVec2.y * forVec2.y));
	//glm::vec3 perp = glm::vec3(xPerp / perpMag, 1.0 / perpMag, 0.0) * forVecMag;

	//float perpDistLeft = (junction->getXLeftSquare() - m_xpos) / perp.x;
	//float perpDistRight = (junction->getXRightSquare() - m_xpos) / perp.x;
	//float perpDistTop = (junction->getYTopSquare() - m_ypos) / perp.y;
	//float perpDistBot = (junction->getYBotSquare() - m_ypos) / perp.y;

	//float perpDistHorizontal = (junction->GetYPos() - m_ypos) / perp.y;
	//float perpDistVertical = (junction->GetXPos() - m_xpos) / perp.x;

	//if (m_xpos <= junction->getXRightSquare() && m_xpos >= junction->getXLeftSquare() &&
	//	m_ypos <= junction->getYTopSquare() && m_ypos >= junction->getYBotSquare()
	//	) {

	//}
	//else {
	//	//Left
	//	if (m_xpos < junction->getXLeftSquare()) {
	//		float tValCentre = (junction->GetXPos() - m_xpos) / forVec2.x;
	//		//Facing centre
	//		if (tValCentre > 0) {
	//			if (abs(m_ypos - junction->getYTopSquare()) < 4 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(m_ypos - junction->GetYPos()) < 4 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			//if (abs(perpDistTop) < 4) {
	//			//	//std::cout << "Moving to avoid top line to side" << std::endl;
	//			//	angle -= 2.4 / fps;
	//			//}
	//			//if (abs(perpDistHorizontal) < 4) {
	//			//	//std::cout << "Moving to avoid middle horizontal line to side" << std::endl;
	//			//	angle += 2.4 / fps;
	//			//}
	//		}
	//		else {
	//			if (abs(m_ypos - junction->getYBotSquare()) < 4 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(m_ypos - junction->GetYPos()) < 4 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			//if (abs(perpDistBot) < 4) {
	//			//	//std::cout << "Moving to avoid bot line to side" << std::endl;
	//			//	angle -= 2.4 / fps;
	//			//}
	//			//if (abs(perpDistHorizontal) < 4) {
	//			//	//std::cout << "Moving to avoid middle horizontal line to side" << std::endl;
	//			//	angle += 2.4 / fps;
	//			//}
	//		}
	//	}
	//	//Right
	//	if (m_xpos > junction->getXRightSquare()) {
	//		float tValCentre = (junction->GetXPos() - m_xpos) / forVec2.x;
	//		if (tValCentre > 0) {
	//			if (abs(m_ypos - junction->GetYPos()) < 4 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(m_ypos - junction->getYBotSquare()) < 4 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			//if (abs(perpDistBot) < 4) {
	//			//	//std::cout << "Moving to avoid bot line to side" << std::endl;
	//			//	angle += 2.4 / fps;
	//			//}
	//			//if (abs(perpDistHorizontal) < 4) {
	//			//	//std::cout << "Moving to avoid middle horizontal line to side" << std::endl;
	//			//	angle -= 2.4 / fps;
	//			//}
	//		}
	//		else {
	//			if (abs(m_ypos - junction->GetYPos()) < 4 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(m_ypos - junction->getYTopSquare()) < 4 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			//if (abs(perpDistTop) < 4) {
	//			//	angle += 2.4 / fps;
	//			//}
	//			//if (abs(perpDistHorizontal) < 4) {
	//			//	angle -= 2.4 / fps;
	//			//}
	//		}
	//	}
	//	//Top
	//	if (m_ypos > junction->getYTopSquare()) {
	//		float tValCentre = (junction->GetYPos() - m_ypos) / forVec2.y;
	//		if (tValCentre > 0) {
	//			if (abs(m_xpos - junction->getXRightSquare()) < 4 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(m_xpos - junction->GetXPos()) < 4 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			//if (abs(perpDistRight) < 4) {
	//			//	//std::cout << "Moving to avoid right line to side" << std::endl;
	//			//	angle += 2.4 / fps;
	//			//}
	//			//if (abs(perpDistVertical) < 4) {
	//			//	//std::cout << "Moving to avoid middle vertical line to side" << std::endl;
	//			//	angle -= 2.4 / fps;
	//			//}
	//		}
	//		else {
	//			if (abs(m_xpos - junction->GetXPos()) < 4 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			if (abs(m_xpos - junction->getXLeftSquare()) < 4 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			//if (abs(perpDistLeft) < 4) {
	//			//	angle += 2.4 / fps;
	//			//}
	//			//if (abs(perpDistVertical) < 4) {
	//			//	angle -= 2.4 / fps;
	//			//}
	//		}
	//	}
	//	//Bottom
	//	if (m_ypos < junction->getYBotSquare()) {
	//		float tValCentre = (junction->GetYPos() - m_ypos) / forVec2.y;
	//		//std::cout << "Perp dist Right = " << abs(perpDistRight) << " Perp dist vertical = " << abs(perpDistVertical) << std::endl;
	//		if (tValCentre > 0) {
	//			if (abs(m_xpos - junction->getXLeftSquare()) < 4 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(m_xpos - junction->GetXPos()) < 4 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			//if (abs(perpDistLeft) < 4) {
	//			//	//std::cout << "Moving to avoid left line to side" << std::endl;
	//			//	angle += 2.4 / fps;
	//			//}
	//			//if (abs(perpDistVertical) < 4) {
	//			//	//std::cout << "Moving to avoid middle vertical line to side" << std::endl;
	//			//	angle -= 2.4 / fps;
	//			//}
	//		}
	//		else {
	//			if (abs(m_xpos - junction->getXRightSquare()) < 4 && speed > 0.01) {
	//				angle -= 0.2 / fps;
	//			}
	//			if (abs(m_xpos - junction->GetXPos()) < 4 && speed > 0.01) {
	//				angle += 0.2 / fps;
	//			}
	//			//if (abs(perpDistRight) < 4) {
	//			//	std::cout << "Moving to avoid right line to side" << std::endl;
	//			//	angle -= 2.4 / fps;
	//			//}
	//			//if (abs(perpDistVertical) < 4) {
	//			//	std::cout << "Moving to avoid middle vertical line to side" << std::endl;
	//			//	angle += 2.4 / fps;
	//			//}
	//		}
	//	}
	//}

	// Use on left test to determine if the car has turned correctly and correct angle
	switch (exitTurning) {
	case(0):
		if (m_xpos <= (*junction).getXLeftSquare() && speed != 0.0) {
			glm::vec3 target = glm::vec3(-1.0, 0.0, 0.0);
			float area = (forVec2.x * target.y) - (target.x * forVec2.y);
			if (area < -0){// && abs(m_ypos - junction->getYBotSquare()) > 4) {
				//std::cout << "Correction" << std::endl;
				angle -= 0.6/fps;
			}
			if (area > 0) {// && abs(m_ypos - junction->GetYPos()) > 4) {
				//std::cout << "Correction" << std::endl;
				angle += 0.6 / fps;
			}
		}
		break;
	case(1):
		if (m_xpos >= (*junction).getXRightSquare() && speed != 0.0) {
			glm::vec3 target = glm::vec3(1.0, 0.0, 0.0);
			float area = (forVec2.x * target.y) - (target.x * forVec2.y);
			if (area < -0) {// && abs(m_ypos - junction->getYTopSquare()) > 4) {
				//std::cout << "Correction" << std::endl;
				angle -= 0.6 / fps;
			}
			if (area > 0) {// && abs(m_ypos - junction->GetYPos()) > 4) {
				//std::cout << "Correction" << std::endl;
				angle += 0.6 / fps;
			}
		}
		break;
	case(2):
		if (m_ypos >= (*junction).getYTopSquare() && speed != 0.0) {
			glm::vec3 target = glm::vec3(0.0, 1.0, 0.0);
			float area = (forVec2.x * target.y) - (target.x * forVec2.y);
			if (area < -0) {// && abs(m_xpos - junction->getXLeftSquare()) > 4) {
				//std::cout << "Correction" << std::endl;
				angle -= 0.6 / fps;
			}
			if (area > 0) {// && abs(m_xpos - junction->GetXPos()) > 4) {
				//std::cout << "Correction" << std::endl;
				angle += 0.6 / fps;
			}
		}
		break;
	case(3):
		if (m_ypos <= (*junction).getYBotSquare() && speed != 0.0) {
			glm::vec3 target = glm::vec3(0.0, -1.0, 0.0);
			float area = (forVec2.x * target.y) - (target.x * forVec2.y);
			if (area < -0) {// && abs(m_xpos - junction->getXRightSquare()) > 4) {
				//std::cout << "Correction" << std::endl;
				angle -= 0.6 / fps;
			}
			if (area > 0) {// && abs(m_xpos - junction->GetXPos()) > 4) {
				//std::cout << "Correction" << std::endl;
				angle += 0.6 / fps;
			}
		}
		break;
	}

	moving:
		m_xpos += forVec2.x * speed;
		m_ypos += forVec2.y * speed;
		glm::vec3 vector = glm::vec3(m_xpos, m_ypos, 0);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), vector);
		if (angle > 360) {
			angle -= 360;
		}
	return glm::rotate(transform, angle, glm::vec3(0, 0, 1));


	//return glm::mat4(1.0f);

	////std::cout << std::endl;
	//glm::vec3 forVec2 = forVec;
	///*std::cout << "green = " << junction.getTrafficLights()[entryPoint].getLights()[2] << std::endl;*/
	//if ((*junction).getType() == RoadType::T || (*junction).getType() == RoadType::X) { //for checking that the road actually has traffic lights to obey by
	//	switch (entryPoint) {
	//	case(0):
	//		if ((m_xpos + m_Width / 2) <= (*junction).getXLeftSquare() && m_xpos > ((*junction).getXLeftSquare() - (m_Height)) && !(*junction).getTrafficLights()[entryPoint].isGreen()) {
	//			//std::cout << "Bug A" << std::endl;
	//			speed = 0;
	//		}
	//		else if ((m_xpos+m_Width/2) >= (*junction).getXLeftSquare() + ((*junction).getWidth() * 1 / 12) && m_xpos <= (*junction).GetXPos() && (*junction).getTrafficLights()[1].getLights()[2] && direction == 1) {
	//			//std::cout << "Bug B" << std::endl;
	//			//std::cout << (*junction).getTrafficLights()[1].isGreen() << std::endl;
	//			speed = 0;
	//		}
	//		else {
	//			speed = speed;
	//		}
	//		break;
	//	case(1):
	//		if ((m_xpos - m_Width / 2) >= (*junction).getXRightSquare() && m_xpos < ((*junction).getXRightSquare() + (m_Height)) && !(*junction).getTrafficLights()[entryPoint].getLights()[2]) {
	//			speed = 0;
	//		}
	//		else if ((m_xpos - m_Width / 2) <= (*junction).getXRightSquare() - ((*junction).getWidth() * 1 / 12) && m_xpos >= (*junction).GetXPos() && (*junction).getTrafficLights()[0].isGreen() && direction == 1) {
	//			speed = 0;
	//		}
	//		else {
	//			speed = speed;
	//		}
	//		break;
	//	case(2):
	//		if ((m_ypos-m_Width/2) >= (*junction).getYTopSquare() && m_ypos < ((*junction).getYTopSquare() + (m_Height)) && !(*junction).getTrafficLights()[entryPoint].getLights()[2]) {
	//			speed = 0;
	//		}
	//		else if ((m_ypos - m_Width / 2) <= (*junction).getYTopSquare() - ((*junction).getHeight() * 1 / 12) && m_ypos >= (*junction).GetYPos() && (*junction).getTrafficLights()[3].isGreen() && direction == 1) {
	//			speed = 0;
	//		}
	//		else {
	//			speed = speed;
	//		}
	//		break;
	//	case(3):
	//		if ((m_ypos + m_Width / 2) <= (*junction).getYBotSquare() && m_ypos > ((*junction).getYBotSquare() - (m_Height)) && !(*junction).getTrafficLights()[entryPoint].getLights()[2]) {
	//			speed = 0;
	//		}
	//		else if ((m_ypos + m_Width / 2) >= (*junction).getYBotSquare() + ((*junction).getHeight() * 1 / 12) && m_ypos <= (*junction).GetYPos() && (*junction).getTrafficLights()[2].isGreen() && direction == 1) {
	//			speed = 0;
	//		}
	//		else {
	//			speed = speed;
	//		}
	//		break;
	//	}
	//}
	//
	//switch (direction) {
	//case(-1):
	//	switch (entryPoint) {
	//	case(0):
	//		//if (m_xpos <= junction.getXLeftSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
	//		//	speed = 0;
	//		//	break;
	//		//}
	//		if (m_xpos >= (*junction).getXLeftSquare() && m_ypos <= (*junction).getYTopSquare() && speed != 0) {
	//			angle += 2.4f / fps;
	//		}
	//		break;
	//	case(1):
	//		//if (m_xpos >= junction.getXRightSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
	//		//	speed = 0;
	//		//	break;
	//		//}
	//		if (m_xpos <= (*junction).getXRightSquare() && m_ypos >= (*junction).getYBotSquare() && speed != 0) {
	//			angle += 2.4f / fps;
	//		}
	//		break;
	//	case(2):
	//		//if (m_ypos >= junction.getYTopSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
	//		//	speed = 0;
	//		//	break;
	//		//}
	//		if (m_ypos <= (*junction).getYTopSquare() && m_xpos <= (*junction).getXRightSquare() && speed != 0) {
	//			angle += 2.4f / fps;
	//		}
	//		break;
	//	case(3):
	//		//if (m_ypos <= junction.getYBotSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
	//		//	speed = 0;
	//		//	break;
	//		//}
	//		if (m_ypos >= (*junction).getYBotSquare() && m_xpos >= (*junction).getXLeftSquare() && speed != 0) {
	//			angle += 2.4f / fps;
	//		}
	//		break;
	//	}
	//	break;
	//case(0):
	//	switch (entryPoint) {
	//	case(0):
	//		//if (m_xpos <= junction.getXLeftSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
	//		//	speed = 0;
	//		//	break;
	//		//}
	//		break;
	//	case(1):
	//		//if (m_xpos >= junction.getXRightSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
	//		//	speed = 0;
	//		//	break;
	//		//}
	//		break;
	//	case(2):
	//		//if (m_ypos >= junction.getYTopSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
	//		//	speed = 0;
	//		//	break;
	//		//}
	//		break;
	//	case(3):
	//		//if (m_ypos <= junction.getYBotSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
	//		//	speed = 0;
	//		//	break;
	//		//}
	//		break;
	//	}
	//	break;
	//case(1):
	//	switch (entryPoint) {
	//	case(0):
	//		//if (m_xpos <= junction.getXLeftSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
	//		//	speed = 0;
	//		//	break;
	//		//}
	//		if (m_xpos >= (*junction).GetXPos() && m_ypos >= (*junction).GetYPos() && !(*junction).getTrafficLights()[1].isGreen() && speed != 0) {
	//			angle -= 2.4f/fps;
	//		}
	//		break;
	//	case(1):
	//		//if (m_xpos >= junction.getXRightSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
	//		//	speed = 0;
	//		//	break;
	//		//}
	//		if (m_xpos <= (*junction).GetXPos() && m_ypos <= (*junction).GetYPos() && !(*junction).getTrafficLights()[2].isGreen() && speed != 0) {
	//			angle -= 2.4f/fps;		
	//		}
	//		break;
	//	case(2):
	//		//if (m_ypos >= junction.getYTopSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
	//		//	speed = 0;
	//		//	break;
	//		//}
	//		if (m_ypos <= (*junction).GetYPos() && m_xpos >= (*junction).GetXPos() && !(*junction).getTrafficLights()[3].isGreen() && speed != 0) {
	//			angle -= 2.4f/fps;

	//		}
	//		break;
	//	case(3):
	//		//if (m_ypos <= junction.getYBotSquare() && !junction.getTrafficLights()[entryPoint].getLights()[2]) {
	//		//	speed = 0;
	//		//	break;
	//		//}
	//		if (m_ypos >= (*junction).GetYPos() && m_xpos <= (*junction).GetXPos() && !(*junction).getTrafficLights()[2].isGreen() && speed != 0) {
	//			angle -= 2.4f / fps;
	//		}
	//		break;
	//	}
	//	break;
	//default:
	//	break;
	//}
	//glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	//forVec2 = matrix * glm::vec4(forVec, 1.0f);
	//m_xpos += forVec2.x * speed;
	//m_ypos += forVec2.y * speed;
	//glm::vec3 vector = glm::vec3(m_xpos, m_ypos, 0);
	//glm::mat4 transform = glm::translate(glm::mat4(1.0f), vector);
	//if (angle > 360) {
	//	angle -= 360;
	//}
	//// Use on left test to determine if the car has turned correctly
	//switch (exitTurning) {
	//case(0):
	//	if (m_xpos <= (*junction).getXLeftSquare() && speed != 0.0) {
	//		glm::vec3 target = glm::vec3(-1.0, 0.0, 0.0);
	//		float area = (forVec2.x * target.y) - (target.x * forVec2.y);
	//		if (area < 0) {
	//			angle -= 1.2/fps;
	//		}
	//		if (area > 0) {
	//			angle += 1.2 / fps;
	//		}
	//	}
	//	break;
	//case(1):
	//	if (m_xpos >= (*junction).getXRightSquare() && speed != 0) {
	//		glm::vec3 target = glm::vec3(1.0, 0.0, 0.0);
	//		float area = (forVec2.x * target.y) - (target.x * forVec2.y);
	//		if (area < 0) {
	//			angle -= 1.2 / fps;
	//		}
	//		if (area > 0) {
	//			angle += 1.2 / fps;
	//		}
	//	}
	//	break;
	//case(2):
	//	if (m_ypos >= (*junction).getYTopSquare() && speed != 0.0) {
	//		glm::vec3 target = glm::vec3(0.0, 1.0, 0.0);
	//		float area = (forVec2.x * target.y) - (target.x * forVec2.y);
	//		if (area < 0) {
	//			angle -= 1.2 / fps;
	//		}
	//		if (area > 0) {
	//			angle += 1.2 / fps;
	//		}
	//	}
	//	break;
	//case(3):
	//	if (m_ypos <= (*junction).getYBotSquare() && speed != 0.0) {
	//		glm::vec3 target = glm::vec3(0.0, -1.0, 0.0);
	//		float area = (forVec2.x * target.y) - (target.x * forVec2.y);
	//		if (area < 0) {
	//			angle -= 1.2 / fps;
	//		}
	//		if (area > 0) {
	//			angle += 1.2 / fps;
	//		}
	//	}
	//	break;
	//}
	//return glm::rotate(transform, angle, glm::vec3(0, 0, 1));
//return glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(-30.10,0.0,0.0)), glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
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