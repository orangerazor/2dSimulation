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
	angle = 0;
	objectRotation = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0,0,1));
	srand(time(0));
	forVec = objectRotation * glm::vec4(forVec, 1.0f);
	junction = &Junction();
	path = {};
	this->uniqueIdentifier = 0;
}

int Car::entryPoint()
{
	// Checks to see if the entry point has already been calculated for this junction
	if (currentJunction == (*junction).getIdentifier()) {
		return entryTurning;
	}
	// Loop through the 4 possible spawn points of a junction, calculating the distance between the car and these points 
	// using pythagorus' theorum ( (x1-x2)^2 ) + ( (y1-y2)^2 ) 
	// and choosing the closest to be set as entry point
	int entryPoint2 = 0;
	float bestDist = -1;
	for (int i = 0; i < 4; i++) {
		float xVert = INT_MAX;
		float yVert = INT_MAX;
		// Get the mid points between two verticies
		// Look at vertex at i and i + 1
		// If we want the edge between vertex 3 and 0, use the else stamement
		if (i < 3) {
			xVert = ((*junction).GetOBB().vert[i].x + (*junction).GetOBB().vert[i + 1].x) / 2;
			yVert = ((*junction).GetOBB().vert[i].y + (*junction).GetOBB().vert[i + 1].y) / 2;
		}
		else {
			xVert = ((*junction).GetOBB().vert[i].x + (*junction).GetOBB().vert[0].x) / 2;
			yVert = ((*junction).GetOBB().vert[i].y + (*junction).GetOBB().vert[0].y) / 2;
		}
		// Calculate this distance using pythagorus' theorum
		float distanceFromEdge = ((m_xpos - xVert) * (m_xpos - xVert)) + ((m_ypos - yVert) * (m_ypos - yVert));
		// If the best distance hasn't been set or the distance for this edge is better than the last, set it to the current distance
		if (bestDist == -1 || distanceFromEdge < bestDist) {
			bestDist = distanceFromEdge;
			// The OBB moves clockwise around the model but our entry indexes are not ordered in this way, so we adjust the entry point to be correct.
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

	// Depending on the orientation the OBB verticies are moved around the model, this switch statement fixes this by shifting the entry point based on amount rotated
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
	return entryPoint2;
}

int Car::decideDirection(int entryPoint) {

	int exitPoint;
	// If the direction has already been set for this junction then return the current decided direction
	if (currentJunction == this->junction->getIdentifier()) {
		return exit;
	}
	currentJunction = (*junction).getIdentifier();
	// If a path hasn't already been set
	if (this->path.empty()) {
		// Get the number of possible turnings for the cars current junction
		int numTurns = (*junction).getTurnings().size();
		std::vector<int> possibleTurnings;
		// Loop through each turning adding its index to a list
		for (int i = 0; i < numTurns; i++) {
			if (i == entryPoint) {
				continue;
			}
			if ((*junction).getTurning(i) == true) {
				possibleTurnings.push_back(i);
			}
		}
		// Choose a random index in the list
		exitPoint = rand() % possibleTurnings.size();
		// Get the index of the exit at this random index in the list of possible exits
		exitPoint = possibleTurnings.at(exitPoint);
		exitTurning = exitPoint;
	}
	// If a path is set, get the next exit from the top of the stack and pop
	else {
		exitPoint = path[path.size()-1];
		exitTurning = exitPoint;
		path.pop_back();
	}
	
	// Based on the entry and exits points, set the direction of the car (-1 for left, 0 for straight, 1 for right)
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
	exit = exitPoint;
	direction = exitPoint;
	return exitPoint;
}

void Car::respawn(Junction *junction, int presetEntry) {
	
	this->junction = junction;
	// Get the index of the new spawn entrance
	int newSpawn = this->setSpawn(presetEntry);
	// Rotate the car based on the spawn so it is facing the centre of the junction
	switch (newSpawn) {
	case(0):
		angle = glm::radians(180.0f);
		break;
	case(1):
		angle = glm::radians(0.0f);
		break;
	case(2):
		angle = glm::radians(90.0f);
		break;
	case(3):
		angle = glm::radians(270.0f);
		break;
	}
	entryTurning = newSpawn;
	glm::mat4 spawn = glm::translate(glm::mat4(1.0f), glm::vec3(m_xpos, m_ypos, 0));
	spawn = glm::rotate(spawn, angle, glm::vec3(0.0, 0.0, 1.0));
	// Sets the temporay OBB that will be reset in render, so we can avoid the cars spawning on top of each other before they are rendered
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
	// Loop through the number of possible spawns for a junction adding them to a list
	int numTurns = (*junction).getTurnings().size();
	std::vector<int> possibleTurnings;
	for (int i = 0; i < numTurns; i++) {
		if ((*junction).getTurning(i) == true) {
			possibleTurnings.push_back(i);
		}
	}
	// Adjust the OBB verticies for the orientation as if the model is rotated the expected positions of verticies will change
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
	
	// If entry has not been set randomly choose a spawn from list, otherwise just pass the preset entry
	if (entry == -1) {
		spawnEntrance = rand() % possibleTurnings.size();
	}
	else {
		spawnEntrance = entry;
	}
	// Set the position of the car based on the entry calculated
	// Uses the uniform distances of the junction to ensure the car is spawned in the correct location
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
	// Reset the current junction for this car as it has been spawned in a new junction.
	currentJunction = "";
	return entry;
}

glm::mat4 Car::rotate(float speed, int direction, int entryPoint, float fps, std::vector<Car> collideCheck)
{
	/***********************************************************************/
	// Check collisions for this car

	// Loop through the list of all other cars
	for (int i = 0; i < collideCheck.size(); i++) {
		// If the car being checked is the same as the current car, avoid the collision test
		if (collideCheck[i].getUniqueIdentifier() == this->getUniqueIdentifier()) {
			continue;
		}
		// Otherwise if the other car is inside the collide box out in front of the this car set the speed to zero and avoid all movement and angle calculations
		if (collideCheck[i].IsInCollision(collide)) {
			speed = 0;
			goto moving;
		}
	/***********************************************************************/
		// Car keeping the box clear

		// Get the distances in the x and y between this car and the other car
		float otherCarX = collideCheck[i].GetXPos();
		float otherCarY = collideCheck[i].GetYPos();
		float inFrontOrBehindX = otherCarX - m_xpos;
		float inFrontOrBehindY = otherCarY - m_ypos;
		// Look at the entry point of this car and check
		// 1) The direction of the other car
		// 2) That the other car is inside the box
		// 3) The distance between this car and the other car in axis parallel to entryPoint (x for left and right, y for top and bottom)
		switch (entryPoint) {
		case 0:
			// Other Car is in front
			if (inFrontOrBehindX > 0) {
				// Other Car is moving right
				if (collideCheck[i].getDirection() == 1) {
					// Other Car is in the box in the same lane as this car
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->GetYPos()) {
						//If the distance is less than 3 cars and this car isn't exiting the junction (so would be already clear of the box), stop
						if (inFrontOrBehindX < m_Width * 3 && m_ypos < junction->getYTopSquare()) {
							speed = 0;
						}
					}
					//Other Car is coming other way, in the box and turning right so give way to it
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->GetYPos() && otherCarY >= junction->getYBotSquare()) {
						//Don't go past the line to keep box clear
						if ((junction->getXLeftSquare() - m_xpos) < (m_Width * 4/3) && m_xpos + (m_Width / 2) < junction->getXLeftSquare()) {
							speed = 0;
						}
					}
				}
				// Both Cars are going straight and are not on a straight road
				// Check to see if this car would block junction by moving straight on
				if (collideCheck[i].getDirection() == 0 && direction == 0 && junction->getType() != RoadType::S) {
					// Other Car is in box and in same lane as this car
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->getXLeftSquare() && otherCarY >= junction->GetYPos() && otherCarY <= junction->getYTopSquare()) {
						//If the distance is below the width of 4 cars then stop
						if (inFrontOrBehindX < m_Width * 4) {
							speed = 0;
						}
					}
				}
				// Check if a car has turned left in the way of this car so it would block junction
				// Other car is turning left and this car is either going straight on or right
				if (collideCheck[i].getDirection() == -1 && direction != -1) {
					// Other Car is in box and in same lane as this car 
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->GetYPos()) {
						// If the distance is less than 3 cars then wait
						if (inFrontOrBehindX < m_Width * 3) {
							speed = 0;
						}
					}
				}
			}
			break;
		case 1:
			// Other Car is in front
			if (inFrontOrBehindX < 0) {
				// Other Car is moving right
				if (collideCheck[i].getDirection() == 1) {
					// Other Car is in the box in the same lane as this car
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->GetYPos() && otherCarY >= junction->getYBotSquare()) {
						//If the distance is less than 3 cars and this car isn't exiting the junction (so would be already clear of the box), stop
						if (inFrontOrBehindX > -m_Width * 3 && m_ypos > junction->getYBotSquare()) {
							speed = 0;
						}
					}
					//Other Car is coming other way, in the box and turning right so give way to it
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->GetYPos()) {
						//Don't go past the line to keep box clear
						if ((junction->getXRightSquare() - m_xpos) > (-m_Width * 4 / 3) && m_xpos - (m_Width / 2) > junction->getXRightSquare()) {
							speed = 0;
						}
					}
				}
				// Both Cars are going straight and are not on a straight road
				// Check to see if this car would block junction by moving straight on
				if (collideCheck[i].getDirection() == 0 && direction == 0 && junction->getType() != RoadType::S) {
					// Other Car is in box and in same lane as this car 
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->getXLeftSquare() && otherCarY >= junction->getYBotSquare() && otherCarY <= junction->GetYPos()) {
						// If the distance is less than 3 cars then wait
						if (inFrontOrBehindX > -m_Width * 4) {
							speed = 0;
						}
					}
				}
				// Check if a car has turned left in the way of this car so it would block junction
				// Other car is turning left and this car is either going straight on or right
				if (collideCheck[i].getDirection() == -1 && direction != -1) {
					// Other Car is in box and in same lane as this car 
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->getXLeftSquare() && otherCarY >= junction->getYBotSquare() && otherCarY <= junction->GetYPos()) {
						// If the distance is less than 3 cars then wait
						if (inFrontOrBehindX > -m_Width * 3) {
							speed = 0;
						}
					}
				}
			}
			break;
		case 2:
			// Other Car is in front
			if (inFrontOrBehindY < 0) {
				// Other Car is moving right
				if (collideCheck[i].getDirection() == 1) {
					// Other Car is in the box in the same lane as this car
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->GetXPos() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->getYBotSquare()) {
						//If the distance is less than 3 cars and this car isn't exiting the junction (so would be already clear of the box), stop
						if (inFrontOrBehindY > -m_Width * 3 && m_xpos < junction->getXLeftSquare()) {
							speed = 0;
						}
					}
					//Other Car is coming other way, in the box and turning right so give way to it
					if (otherCarX <= junction->GetXPos() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->getYBotSquare()) {
						//Don't go past the line to keep box clear
						if ((junction->getYTopSquare() - m_ypos) > (-m_Width * 4 / 3) && m_ypos - (m_Width / 2) > junction->getYTopSquare()) {
							speed = 0;
						}
					}
				}
				// Both Cars are going straight and are not on a straight road
				// Check to see if this car would block junction by moving straight on
				if (collideCheck[i].getDirection() == 0 && direction == 0 && junction->getType() != RoadType::S) {
					// Other Car is in box and in same lane as this car 
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->GetXPos() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->getYBotSquare()) {
						// If the distance is less than 3 cars then wait
						if (inFrontOrBehindY > -m_Width * 4) {
							speed = 0;
						}
					}
				}
				// Check if a car has turned left in the way of this car so it would block junction
				// Other car is turning left and this car is either going straight on or right
				if (collideCheck[i].getDirection() == -1 && direction != -1) {
					// Other Car is in box and in same lane as this car 
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->GetXPos() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->getYBotSquare()) {
						// If the distance is less than 3 cars then wait
						if (inFrontOrBehindY > -m_Width * 3) {
							speed = 0;
						}
					}
				}
			}
			break;
		case 3:
			// Other Car is in front
			if (inFrontOrBehindY > 0) {
				// Other Car is moving right
				if (collideCheck[i].getDirection() == 1) {
					// Other Car is in the box in the same lane as this car
					if (otherCarX <= junction->GetXPos() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->getYBotSquare()) {
						//If the distance is less than 3 cars and this car isn't exiting the junction (so would be already clear of the box), stop
						if (inFrontOrBehindY < m_Width * 3 && m_xpos > junction->getXRightSquare()) {
							speed = 0;
						}
					}
					//Other Car is coming other way, in the box and turning right so give way to it
					if (otherCarX <= junction->getXRightSquare() && otherCarX >= junction->GetXPos() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->getYBotSquare()) {
						//Don't go past the line to keep box clear
						if ((junction->getYBotSquare() - m_ypos) < (m_Width * 4 / 3) && m_ypos + (m_Width / 2) < junction->getYBotSquare()) {
							speed = 0;
						}
					}
				}
				// Both Cars are going straight and are not on a straight road
				// Check to see if this car would block junction by moving straight on
				if (collideCheck[i].getDirection() == 0 && direction == 0 && junction->getType() != RoadType::S) {
					// Other Car is in box and in same lane as this car 
					if (otherCarX <= junction->GetXPos() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->getYBotSquare()) {
						// If the distance is less than 3 cars then wait
						if (inFrontOrBehindY < m_Width * 4) {
							speed = 0;
						}
					}
				}
				// Check if a car has turned left in the way of this car so it would block junction
				// Other car is turning left and this car is either going straight on or right
				if (collideCheck[i].getDirection() == -1 && direction != -1) {
					// Other Car is in box and in same lane as this car 
					if (otherCarX <= junction->GetXPos() && otherCarX >= junction->getXLeftSquare() && otherCarY <= junction->getYTopSquare() && otherCarY >= junction->getYBotSquare()) {
						// If the distance is less than 3 cars then wait
						if (inFrontOrBehindY > m_Width * 3) {
							speed = 0;
						}
					}
				}
			}
			break;
		}
	}
	
	// A copy of the original vector for the car is made and rotated by angle for use in next calculations
	glm::vec3 forVec2 = forVec;
	glm::mat4 matrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	forVec2 = matrix * glm::vec4(forVec, 1.0f);

	/***********************************************************************/
	// Movements based on junction

	float intersectDistTop = INT_MAX;
	float intersectDistBot = INT_MAX;
	float intersectDistRight = INT_MAX;
	float intersectDistLeft = INT_MAX;

	float intersectDistHorizontal = INT_MAX;
	float intersectDistVertical = INT_MAX;

	// Distance along the car moving vector until it hits different lines on the junction
	intersectDistRight = (junction->getXRightSquare() - m_xpos) / (forVec2.x);
	intersectDistLeft = (junction->getXLeftSquare() - m_xpos) / (forVec2.x);
	intersectDistTop = (junction->getYTopSquare() - m_ypos) / (forVec2.y);
	intersectDistBot = (junction->getYBotSquare() - m_ypos) / (forVec2.y);

	// Distance for the lines in the middle of the junction i.e lane dividers
	intersectDistHorizontal = (junction->GetYPos() - m_ypos) / (forVec2.y);
	intersectDistVertical = (junction->GetXPos() - m_xpos) / (forVec2.x);

	// Handle each junction type separately
	switch (junction->getType()) {
	case RoadType::S:
		// In this case the speed and angle will never be changed as a result of the junction
		break;
	case RoadType::T:
		// Use each entry Point to determine how the car should repond.
		// Each entry point has a different check for which value should be checked for stopping at lights and turning circle
		switch (entryPoint) {
		case 0:
			// Makes the car stop at the line when the lights are red
			// If the distance in x from the car is less than the 2/3 width of the car stop unless the lights are green 
			if (abs(intersectDistLeft) < (m_Width * 4/6) && !junction->getTrafficLights()[entryPoint].getLights()[2]) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			// In the case that the car is already past the lights, force the car to keep moving
			if (m_xpos + (m_Width / 2) >= junction->getXLeftSquare()) {
				speed = 12.0f / fps;
			}

			// Handles the angle changing for the car, checking the direction of the car to decide when it should turn
			// The car is in the box as x distance between centre and left box line is negative
			if (intersectDistLeft <= 0 && speed > 0.01) {
				switch (direction) {
				// If direction is left turn straight away
				case -1:
					angle += 2.4 / fps;
					break;
				// Don't turn at all if going straight
				case 0:
					break;
				case 1:
					// Only start turning when past the middle of the junction so as to not be on the wrong side of the road
					if (intersectDistVertical < 0 && m_ypos > junction->getYBotSquare()) {
						angle -= 2.4 / fps;
					}
					break;
				}
			}
			break;
		case 1:
			// Same as other entry points but adjusted to use different points on junction to turn
			if (abs(intersectDistRight) < (m_Width * 4/6) && !junction->getTrafficLights()[entryPoint].getLights()[2]) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			if (m_xpos - (m_Width / 2) <= junction->getXRightSquare()) {
				speed = 12.0f / fps;
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
					if (intersectDistVertical < 0 && m_ypos < junction->getYTopSquare()) {
						angle -= 2.4 / fps;
					}
					break;
				}
			}
			break;
		case 2:
			// Same as other entry points but adjusted to use different points on junction to turn
			if (abs(intersectDistTop) < (m_Width * 4/6) && !junction->getTrafficLights()[entryPoint].getLights()[2]) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			if (m_ypos - (m_Width / 2) <= junction->getYTopSquare()) {
				speed = 12.0f / fps;
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
					if (intersectDistHorizontal < 0 && m_xpos > junction->getXLeftSquare()) {
						angle -= 2.4 / fps;
					}
					break;
				}
			}
			break;
		case 3:
			// Same as other entry points but adjusted to use different points on junction to turn
			if (abs(intersectDistBot) < (m_Width * 4/6) && !junction->getTrafficLights()[entryPoint].getLights()[2]) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			if (m_ypos + (m_Width / 2) >= junction->getYBotSquare()) {
				speed = 12.0f / fps;
			}
			if (intersectDistBot <= 0 && speed > 0.01) {
				switch (direction) {
				case -1:
					angle += 2.4 / fps;
					break;
				case 0:
					break;
				case 1:
					if(intersectDistHorizontal < 0 && m_xpos < junction->getXRightSquare()){
						angle -= 2.4 / fps;
					}
					break;
				}
			}
			break;
		}
		break;
	case RoadType::X:
		// Same code as for the T junction
		switch (entryPoint) {
		case 0:
			if (abs(intersectDistLeft) < (m_Width * 4 / 6) && !junction->getTrafficLights()[entryPoint].getLights()[2] && (m_xpos + (m_Width / 2) < junction->getXLeftSquare()) ) {
				speed = 0;
			}
			else {
				speed = speed;
			}
			if (m_xpos + (m_Width / 2) >= junction->getXLeftSquare()) {
				speed = 12.0 / fps;
			}
			if (intersectDistLeft <= 0 && speed > 0.01) {
				switch (direction) {
				case -1:
					angle += 2.4 / fps;
					break;
				case 0:
					break;
				case 1:
					if (intersectDistVertical < 0 && m_ypos < junction->getYBotSquare()) {
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
			if (m_xpos - (m_Width / 2) <= junction->getXRightSquare()) {
				speed = 12.0 / fps;
			}
			if (intersectDistRight <= 0 && speed > 0.01) {
				switch (direction) {
				case -1:
					angle += 2.4 / fps;
					break;
				case 0:
					break;
				case 1:
					if (intersectDistVertical < 0 && m_ypos < junction->getYTopSquare()) {
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
			if (intersectDistTop <= 0 && speed > 0.01) {
				switch (direction) {
				case -1:
					angle += 2.4 / fps;
					break;
				case 0:
					break;
				case 1:
					if (intersectDistHorizontal < 0 && m_xpos > junction->getXLeftSquare()) {
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
					if (intersectDistHorizontal < 0 && m_xpos < junction->getXRightSquare()) {
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

	/***********************************************************************/
	// Prototype code for further 3rd party maintance and expansion
	// This code was tested and implemented on a smaller scale, though in the larger project it caused too many bugs to be kept in for the final build

		/*******************************************************************/
		// Use line calculations on the junction and the distance from forward ray to these to avoid collisions	

		/*
		* Different cases for if the car is inside the box, or outside
		* Based on the entry point and direction will use absolute values of distances relevant to increase turning angle if collision is imminent
		* For example if going straight on for entry Point 0 it will use y values of the top of the box and centre and if the t value if less than 2 turn away from this edge of the road
		* 
		* Inside the box
		if (m_xpos <= junction->getXRightSquare() && m_xpos >= junction->getXLeftSquare() &&
			m_ypos <= junction->getYTopSquare() && m_ypos >= junction->getYBotSquare()
			) {
			switch (entryPoint) {
			case 0:
				switch (direction) {
				case -1:
					if (abs(intersectDistVertical) < 2 && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					break;
				case 0:
					if (abs(intersectDistTop) < 2 && speed > 0.01) {
						angle += 0.2 / fps;
					}
					if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					break;
				case 1:
					if (abs(intersectDistTop) < 2 && speed > 0.01) {
						angle += 0.2 / fps;
					}
					if (abs(intersectDistHorizontal) < 2 && m_xpos < junction->GetXPos() && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					if (abs(intersectDistRight) < 2 && speed > 0.01) {
						angle += 0.2 / fps;
					}
					if (abs(intersectDistVertical) < 2 && m_xpos > junction->GetXPos() && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					break;
				}
				break;
			case 1:
				switch (direction) {
				case -1:
					if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					if (abs(intersectDistVertical) < 2 && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					break;
				case 0:
					if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					if (abs(intersectDistBot) < 2 && speed > 0.01) {
						angle += 0.2 / fps;
					}
					break;
				case 1:
					if (abs(intersectDistBot) < 2 && speed > 0.01) {
						angle += 0.2 / fps;
					}
					if (abs(intersectDistHorizontal) < 2 && m_xpos > junction->GetXPos() && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					if (abs(intersectDistLeft) < 2 && speed > 0.01) {
						angle += 0.2 / fps;
					}
					if (abs(intersectDistVertical) < 2 && m_xpos < junction->GetXPos() && speed > 0.01) {
						angle += 0.2 / fps;
					}
					break;
				}
				break;
			case 2:
				switch (direction) {
				case -1:
					if (abs(intersectDistVertical) < 2 && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					break;
				case 0:
					if (abs(intersectDistRight) < 2 && speed > 0.01) {
						angle += 0.2 / fps;
					}
					if (abs(intersectDistVertical) < 2 && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					break;
				case 1:
					if (abs(intersectDistRight) < 2 && speed > 0.01) {
						angle += 0.2 / fps;
					}
					if (abs(intersectDistVertical) < 2 && m_ypos > junction->GetYPos() && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					if (abs(intersectDistBot) < 2 && speed > 0.01) {
						angle += 0.2 / fps;
					}
					if (abs(intersectDistHorizontal) < 2 && m_ypos < junction->GetYPos() && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					break;
				}
				break;
			case 3:
				switch (direction) {
				case -1:
					if (abs(intersectDistVertical) < 2 && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					break;
				case 0:
					if (abs(intersectDistLeft) < 2 && speed > 0.01) {
						angle += 0.2 / fps;
					}
					if (abs(intersectDistVertical) < 2 && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					break;
				case 1:
					if (abs(intersectDistLeft) < 2 && speed > 0.01) {
						angle += 0.2 / fps;
					}
					if (abs(intersectDistVertical) < 2 && m_ypos < junction->GetYPos() && speed > 0.01) {
						angle -= 0.2 / fps;
					}
					if (abs(intersectDistTop) < 2 && speed > 0.01) {
						angle += 0.2 / fps;
					}
					if (abs(intersectDistHorizontal) < 2 && m_ypos > junction->GetYPos() && speed > 0.01) {
						angle += 0.2 / fps;
					}
					break;
				}
				break;
			}
		}
		else {
			//Left
			if (m_xpos <= junction->getXLeftSquare()) {
				float tValCentre = (junction->GetXPos() - m_xpos) / forVec2.x;
				if (abs(intersectDistTop) < 2 && speed > 0.01) {
					//Check which way the car is facing
					// Facing centre of junction
					if (tValCentre > 0) {
						angle -= 0.2 / fps;
					}
					//Facing out of junction
					else {
						angle += 0.2 / fps;
					}
				}
				if (abs(intersectDistBot) < 2 && speed > 0.01) {
					//Check which way the car is facing
					if (tValCentre > 0) {
						angle += 0.2 / fps;
					}
					else {
						angle -= 0.2 / fps;
					}
				}
				//Check for imminent collisions in front along road lines
				if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
					if (tValCentre > 0) {
						angle += 0.2 / fps;
					}
					else {
						angle += 0.2 / fps;
					}
				}
			}
			//Right
			if (m_xpos >= junction->getXRightSquare() && speed > 0.01) {
				float tValCentre = (junction->GetXPos() - m_xpos) / forVec2.x;
				if (abs(intersectDistBot) < 2) {
					//Check which way the car is facing
					// Facing centre of junction
					if (tValCentre > 0) {
						angle -= 0.2 / fps;
					}
					//Facing out of junction
					else {
						angle += 0.2 / fps;
					}
				}
				if (abs(intersectDistTop) < 2 && speed > 0.01) {
					//Check which way the car is facing
					if (tValCentre > 0) {
						angle += 0.2 / fps;
					}
					else {
						angle -= 0.2 / fps;
					}
				}
				//Check for imminent collisions in front along road lines
				if (abs(intersectDistHorizontal) < 2 && speed > 0.01) {
					if (tValCentre > 0) {
						angle -= 0.2 / fps;
					}
					else {
						angle -= 0.2 / fps;
					}
				}
			}
			//Top
			if (m_ypos >= junction->getYTopSquare()) {
				float tValCentre = (junction->GetYPos() - m_ypos) / forVec2.y;
				if (abs(intersectDistLeft) < 2 && speed > 0.01) {
					//Check which way the car is facing
					// Facing centre of junction
					if (tValCentre > 0) {
						angle -= 0.2 / fps;
					}
					//Facing out of junction
					else {
						angle += 0.2 / fps;
					}
				}
				if (abs(intersectDistRight) < 2 && speed > 0.01) {
					//Check which way the car is facing
					if (tValCentre > 0) {
						angle += 0.2 / fps;
					}
					else {
						angle -= 0.2 / fps;
					}
				}
				//Check for imminent collisions in front along road lines
				if (abs(intersectDistVertical) < 2 && speed > 0.01) {
					if (tValCentre > 0) {
						angle += 0.2 / fps;
					}
					else {
						angle += 0.2 / fps;
					}
				}
			}
			//Bottom
			if (m_ypos <= junction->getYBotSquare()) {
				float tValCentre = (junction->GetYPos() - m_ypos) / forVec2.y;
				if (abs(intersectDistLeft) < 2 && speed > 0.01) {
					//Check which way the car is facing
					// Facing centre of junction
					if (tValCentre > 0) {
						angle -= 0.2 / fps;
					}
					//Facing out of junction
					else {
						angle += 0.2 / fps;
					}
				}
				if (abs(intersectDistRight) < 2 && speed > 0.01) {
					//Check which way the car is facing
					if (tValCentre > 0) {
						angle += 0.2 / fps;
					}
					else {
						angle -= 0.2 / fps;
					}
				}
				//Check for imminent collisions in front along road lines
				if (abs(intersectDistVertical) < 2 && speed > 0.01) {
					if (tValCentre > 0) {
						angle += 0.2 / fps;
					}
					else {
						angle += 0.2 / fps;
					}
				}
			}
		}
		*/

		/*******************************************************************/
		// Get distance in axis perpendicular to forward ray, i.e in x for top and bottom and y for left and right
		// Use this distance to determine if the car is not in the centre of the road and it should centralise itself

		/*
		* 1) Calculates whether the car is facing into or out of the junction
		* 2) Checks which road the car is on (always outside the junction) using comparison with junction lines (x < left square means left road)
		* 3) Gets distance from edges in perpendicular axis and if this is below a threshold will adjust angle to move into centre
		//Left
		if (m_xpos < junction->getXLeftSquare()) {
			float tValCentre = (junction->GetXPos() - m_xpos) / forVec2.x;
			//Facing centre
			if (tValCentre > 0) {
				if (abs(m_ypos - junction->getYTopSquare()) < 4 && speed > 0.01) {
					angle -= 0.2 / fps;
				}
				if (abs(m_ypos - junction->GetYPos()) < 4 && speed > 0.01) {
					angle += 0.2 / fps;
				}
			}
			else {
				if (abs(m_ypos - junction->getYBotSquare()) < 4 && speed > 0.01) {
					angle -= 0.2 / fps;
				}
				if (abs(m_ypos - junction->GetYPos()) < 4 && speed > 0.01) {
					angle += 0.2 / fps;
				}
			}
		}
		//Right
		if (m_xpos > junction->getXRightSquare()) {
			float tValCentre = (junction->GetXPos() - m_xpos) / forVec2.x;
			if (tValCentre > 0) {
				if (abs(m_ypos - junction->GetYPos()) < 4 && speed > 0.01) {
					angle += 0.2 / fps;
				}
				if (abs(m_ypos - junction->getYBotSquare()) < 4 && speed > 0.01) {
					angle -= 0.2 / fps;
				}
			}
			else {
				if (abs(m_ypos - junction->GetYPos()) < 4 && speed > 0.01) {
					angle += 0.2 / fps;
				}
				if (abs(m_ypos - junction->getYTopSquare()) < 4 && speed > 0.01) {
					angle -= 0.2 / fps;
				}
			}
		}
		//Top
		if (m_ypos > junction->getYTopSquare()) {
			float tValCentre = (junction->GetYPos() - m_ypos) / forVec2.y;
			if (tValCentre > 0) {
				if (abs(m_xpos - junction->getXRightSquare()) < 4 && speed > 0.01) {
					angle -= 0.2 / fps;
				}
				if (abs(m_xpos - junction->GetXPos()) < 4 && speed > 0.01) {
					angle += 0.2 / fps;
				}
			}
			else {
				if (abs(m_xpos - junction->GetXPos()) < 4 && speed > 0.01) {
					angle += 0.2 / fps;
				}
				if (abs(m_xpos - junction->getXLeftSquare()) < 4 && speed > 0.01) {
					angle -= 0.2 / fps;
				}
			}
		}
		//Bottom
		if (m_ypos < junction->getYBotSquare()) {
			float tValCentre = (junction->GetYPos() - m_ypos) / forVec2.y;
			if (tValCentre > 0) {
				if (abs(m_xpos - junction->getXLeftSquare()) < 4 && speed > 0.01) {
					angle -= 0.2 / fps;
				}
				if (abs(m_xpos - junction->GetXPos()) < 4 && speed > 0.01) {
					angle += 0.2 / fps;
				}
			}
			else {
				if (abs(m_xpos - junction->getXRightSquare()) < 4 && speed > 0.01) {
					angle -= 0.2 / fps;
				}
				if (abs(m_xpos - junction->GetXPos()) < 4 && speed > 0.01) {
					angle += 0.2 / fps;
				}
			}
		}
	}
	*/

	/******************************************************************/
	// On left test for adjusting car to face out of junction

	//Update forVec2 with the rotation for turning applied to it so that it is correct for on left test
	forVec2 = forVec;
	matrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	forVec2 = matrix * glm::vec4(forVec, 1.0f);

	// Applies this correction after the junction has finished turning
	// Uses the exitTurning index to detect when the car is heading out the junction and calculate the target vector based on exit
	// Takes cross product of target and forward ray and correct angle
	switch (exitTurning) {
	case(0):
		if (m_xpos <= (*junction).getXLeftSquare() && speed != 0.0) {
			// Target calculated based on exit
			glm::vec3 target = glm::vec3(-1.0, 0.0, 0.0);
			// Cross product of vectors
			float area = (forVec2.x * target.y) - (target.x * forVec2.y);
			// Adjust angle based on whether area was negative or positive
			if (area < -0){
				angle -= 0.6/fps;
			}
			if (area > 0) {
				angle += 0.6 / fps;
			}
		}
		break;
	case(1):
		// Works similar to other exits except with different target vector
		if (m_xpos >= (*junction).getXRightSquare() && speed != 0.0) {
			glm::vec3 target = glm::vec3(1.0, 0.0, 0.0);
			float area = (forVec2.x * target.y) - (target.x * forVec2.y);
			if (area < -0) {
				angle -= 0.6 / fps;
			}
			if (area > 0) {
				angle += 0.6 / fps;
			}
		}
		break;
	case(2):
		// Works similar to other exits except with different target vector
		if (m_ypos >= (*junction).getYTopSquare() && speed != 0.0) {
			glm::vec3 target = glm::vec3(0.0, 1.0, 0.0);
			float area = (forVec2.x * target.y) - (target.x * forVec2.y);
			if (area < -0) {
				angle -= 0.6 / fps;
			}
			if (area > 0) {
				angle += 0.6 / fps;
			}
		}
		break;
	case(3):
		// Works similar to other exits except with different target vector
		if (m_ypos <= (*junction).getYBotSquare() && speed != 0.0) {
			glm::vec3 target = glm::vec3(0.0, -1.0, 0.0);
			float area = (forVec2.x * target.y) - (target.x * forVec2.y);
			if (area < -0) {
				angle -= 0.6 / fps;
			}
			if (area > 0) {
				angle += 0.6 / fps;
			}
		}
		break;
	}

	//Update forVec2 again for new adjustments
	forVec2 = forVec;
	matrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
	forVec2 = matrix * glm::vec4(forVec, 1.0f);

	/******************************************************************/
	// Finally moving the car

moving:
		// Update the x and y position of the car by moving along the forward vector by a distance of speed.
		m_xpos += forVec2.x * speed;
		m_ypos += forVec2.y * speed;
		// Create a vector of this new position and create a translation matrix for it
		glm::vec3 vector = glm::vec3(m_xpos, m_ypos, 0);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), vector);
		if (angle > 360) {
			angle -= 360;
		}
	// Combine the rotation and translation using the float angle and updated x and y position
	return glm::rotate(transform, angle, glm::vec3(0, 0, 1));
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