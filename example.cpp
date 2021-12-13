#include <iostream>
using namespace std;

//--- OpenGL ---
#include "GL\glew.h"
#include "GL\wglew.h"
#pragma comment(lib, "glew32.lib")
//--------------

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

#include "shaders\Shader.h"
#include "GL\freeglut.h"
#include "FreeImage.h"

#include "Sprite.h"
#include "Car.h"
#include "Junction.h"
#include "TrafficLight.h"
#include "Map.h"
#include <chrono>
#include <thread>
#include <utility>


#include <iostream>
#include <string>
using namespace std;
int fps = 40;

glm::mat4 ViewMatrix;  // matrix for the modelling and viewing
glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
glm::mat4 ModelMatrix;
glm::mat4 ModelViewMatrix;
int screenWidth = 1080, screenHeight = 1080;

//booleans to handle when the arrow keys are pressed or released.
int secondElapsed = 0;
float secondsToHour = 10000000000.0f;
int hourElapsed = 0;
int hour = 1000;
bool Left = false;
bool Right = false;
bool Up = false;
bool Down = false;
bool zoomOut = false;
bool zoomIn = false;
int direction = 0;
float speed = 0;
const double PI = 3.141592653589793238463;
float angle = 0;
float scale = 4.0f;
float zoom = 0.25;
float xpos = 0;
float ypos = 0;
int mapSelect = 1;
int carUniqueIdentifier = 0;
bool debug = false;


Shader shader;
std::vector<Car> cars(0);
//std::vector<Junction> map(0);
//std::vector< std::vector<Junction> > map(2, column)
Car pathfindingCar = Car::Car(glm::mat4(1.0f));
Car basicCar = Car::Car(glm::mat4(1.0f));
Junction tJunction = Junction::Junction("T", 0, 0, glm::mat4(1.0f), RoadType::T);
Junction xJunction = Junction::Junction("X", 0, 0, glm::mat4(1.0f), RoadType::X);
Junction road = Junction::Junction("R", 0, 0, glm::mat4(1.0f), RoadType::S);
Junction emptyJunction = Junction::Junction();
TrafficLight basicTrafficLight = TrafficLight::TrafficLight();
Map mapClass = Map::Map(7, 7);

//TrafficLight* trafficLights[1][4];
//TrafficLight trafficLight;
float coordinates[4][3];
vector<vector<Car>> buffer;


//OPENGL FUNCTION PROTOTYPES
void display();				//used as callback in glut for display.
void reshape(int width, int height); //used as callback for reshape function in glut
void init();				//called in main when the program starts.

/*************    START OF OPENGL FUNCTIONS   ****************/

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width;
	screenHeight = height;

	float aspect = (float)screenWidth / (float)screenHeight;

	glViewport(0, 0, width, height);						// set Viewport dimensions

	if (aspect >= 1.0) {
		ProjectionMatrix = glm::ortho((-25.0f * aspect) / zoom, 25.0f * aspect / zoom, -25.0f / zoom, 25.0f / zoom);
	}
	else {
		ProjectionMatrix = glm::ortho(-25.0f / zoom, 25.0f / zoom, -25.0f / aspect / zoom, 25.0f / aspect / zoom);
	}
	//ProjectionMatrix = glm::ortho(-25.0/zoom, 25.0/zoom, -25.0/zoom, 25.0/zoom);
}

void readjustScreen(int x, int y, int width, int height) {
	screenWidth = width;
	screenHeight = height;

	glViewport(x, y, width, height);
}

void display()

{

	ModelViewMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::mat4(1.0f);

	glm::vec3 pos = glm::vec3(xpos, ypos, 1.0);
	glm::vec3 juncMid = glm::vec3(mapClass.getMapJunction(1,1)->GetXPos(), mapClass.getMapJunction(1,1)->GetYPos(), 0.0);
	ViewMatrix = glm::lookAt(pos + juncMid, glm::vec3(pos.x + juncMid.x, pos.y + juncMid.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	if (hour == 1000) {
		glClearColor((0.0f), (153.0f / 256.0f), (68.0f / 256.0f), (0.0f));
	}
	if (hourElapsed >= 1000000 * secondsToHour) { //day n night cycle
		if (hour == 23) {
			hour = 0;
		}
		else{
			hour++;
		}
		hourElapsed = 0;
		switch (hour)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 23:
			glClearColor((0.0f) * 0.25, (153.0f / 256.0f) * 0.25, (68.0f / 256.0f) * 0.25, (0.0f) * 0.25);
			break;
		case 5:
		case 6:
		case 21:
		case 22:
			glClearColor((0.0f) * 0.4, (153.0f / 256.0f) * 0.4, (68.0f / 256.0f) * 0.4, (0.0f) * 0.4);
			break;
		case 7:
		case 8:
		case 9:
		case 19:
		case 20:
			glClearColor((0.0f) * 0.6, (153.0f / 256.0f) * 0.6, (68.0f / 256.0f) * 0.6, (0.0f) * 0.6);
			break;
		case 10:
		case 11:
		case 17:
		case 18:
			glClearColor((0.0f) * 0.8, (153.0f / 256.0f) * 0.8, (68.0f / 256.0f) * 0.8, (0.0f) * 0.8);
			break;
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		default:
			glClearColor((0.0f), (153.0f / 256.0f), (68.0f / 256.0f), (0.0f));
			break;
		}
	}
	switch (hour)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 22:
	case 23:
		//1000000
		if (secondElapsed >= 1000000) {
			if (cars.size() < 20) {
				Car toSpawn = Car::Car(glm::mat4(1.0f));
				toSpawn = Car(basicCar);
				toSpawn.setIdentifier(cars.size());
				toSpawn.setUniqueIdentifier(carUniqueIdentifier);
				carUniqueIdentifier++;
				toSpawn.setJunction(&emptyJunction);
				cars.push_back(toSpawn);
				/*std::cout << "car size = " << cars.size() << std::endl;
				for (int i = 0; i < cars.size(); i++) {
					std::cout << "I = " << i << std::endl;
					std::cout << "x = " << cars[i].GetXPos() << ", y = " << cars[i].GetYPos() << std::endl;
				}
				std::cout << "\n\n";*/
			}
		}
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 15:
	case 16:
	case 17:
	case 18:
		if (secondElapsed >= 500000) {
			if (cars.size() < 200) {
				Car toSpawn = Car(basicCar);
				toSpawn.setIdentifier(cars.size());
				toSpawn.setUniqueIdentifier(carUniqueIdentifier);
				carUniqueIdentifier++;
				toSpawn.setJunction(&emptyJunction);
				cars.push_back(toSpawn);
			}
		}
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 19:
	case 20:
	case 21:
		if (secondElapsed >= 1000000) {
			if (cars.size() < 100) {
				Car toSpawn = Car(basicCar);
				toSpawn.setIdentifier(cars.size());
				toSpawn.setUniqueIdentifier(carUniqueIdentifier);
				carUniqueIdentifier++;
				toSpawn.setJunction(&emptyJunction);
				cars.push_back(toSpawn);
			}
		}
		break;
	case 100:
		if (cars.size() < 1) {
			Car toSpawn = Car(basicCar);
			toSpawn.setIdentifier(cars.size());
			toSpawn.setUniqueIdentifier(carUniqueIdentifier);
			carUniqueIdentifier++;
			toSpawn.setJunction(&emptyJunction);
			cars.push_back(toSpawn);
		}
		break;
	case 1000:
		break;

	default:
		if (secondElapsed >= 1000000) {
			if (cars.size() < 75) {
				Car toSpawn = Car(basicCar);
				toSpawn.setIdentifier(cars.size());
				toSpawn.setUniqueIdentifier(carUniqueIdentifier);
				carUniqueIdentifier++;
				toSpawn.setJunction(&emptyJunction);
				cars.push_back(toSpawn);
			}
		}
	}
	if (secondElapsed >= 1000000) {
		//for (int i = 0; i < cars.size(); i++) {
		//	cars[i].carNewDirection();
		//}
		for (int i = 0; i < mapClass.getMap().size(); i++) {
			for (int j = 0; j < mapClass.getMap()[i].size(); j++) {
				if ((*mapClass.getMapJunction(i, j)).getType() == RoadType::X || (*mapClass.getMapJunction(i, j)).getType() == RoadType::T) {
					(*mapClass.getMapJunction(i, j)).trafficLightFlow();
					//cout << "1 = " << (*mapClass.getMapJunction(i, j)).getTrafficLights()[1].getLights()[0] << (*mapClass.getMapJunction(i, j)).getTrafficLights()[1].getLights()[1] << (*mapClass.getMapJunction(i, j)).getTrafficLights()[1].getLights()[2] << endl;
					//cout << "2 = " << (*mapClass.getMapJunction(i, j)).getTrafficLights()[2].getLights()[0] << (*mapClass.getMapJunction(i, j)).getTrafficLights()[2].getLights()[1] << (*mapClass.getMapJunction(i, j)).getTrafficLights()[2].getLights()[2] << endl;
					//cout << "3 = " << (*mapClass.getMapJunction(i, j)).getTrafficLights()[3].getLights()[0] << (*mapClass.getMapJunction(i, j)).getTrafficLights()[3].getLights()[1] << (*mapClass.getMapJunction(i, j)).getTrafficLights()[3].getLights()[2] << endl;
					//cout << "tl phase = " << (*mapClass.getMapJunction(i, j)).getTrafficLights();
				}
			}
		}
		secondElapsed = 0;
	}
	for (int i = 0; i < mapClass.getMap().size(); i++) {
		for (int j = 0; j < mapClass.getMap()[i].size(); j++) {
			glm::mat4 junctionRender = glm::mat4(1.0f);
			junctionRender = glm::translate(junctionRender, glm::vec3((*mapClass.getMapJunction(i, j)).GetXPos(), (*mapClass.getMapJunction(i, j)).GetYPos(), 0));
			junctionRender = glm::rotate(junctionRender, glm::radians((*mapClass.getMapJunction(i, j)).getOrientation() * 90.0f), glm::vec3(0.0, 0.0, 1.0));
			ModelViewMatrix = ViewMatrix * junctionRender;
			(*mapClass.getMapJunction(i, j)).Render(shader, ModelViewMatrix, ProjectionMatrix, junctionRender, hour);
			if ((*mapClass.getMapJunction(i, j)).getType() != (RoadType::N)) {
				for (int k = 0; k < 4; k++) {
					if ((*mapClass.getMapJunction(i, j)).getTurnings()[k]) {
						glm::mat4 moveLight = glm::mat4(1.0f);
						switch (k) {
						case(0):
							moveLight = glm::translate(moveLight, glm::vec3((*mapClass.getMapJunction(i, j)).getXLeftSquare() - ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getHeight() / 2), (*mapClass.getMapJunction(i, j)).getYTopSquare() + ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getWidth() / 2), 0.0));
							moveLight = glm::rotate(moveLight, glm::radians(270.0f), glm::vec3(0.0, 0.0, 1.0));
							break;
						case(1):
							moveLight = glm::translate(moveLight, glm::vec3((*mapClass.getMapJunction(i, j)).getXRightSquare() + ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getHeight() / 2), (*mapClass.getMapJunction(i, j)).getYBotSquare() - ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getWidth() / 2), 0.0));
							moveLight = glm::rotate(moveLight, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
							break;
						case(2):
							moveLight = glm::translate(moveLight, glm::vec3((*mapClass.getMapJunction(i, j)).getXRightSquare() + ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getWidth() / 2), (*mapClass.getMapJunction(i, j)).getYTopSquare() + ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getHeight() / 2), 0.0));
							moveLight = glm::rotate(moveLight, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
							break;
						case(3):
							moveLight = glm::translate(moveLight, glm::vec3((*mapClass.getMapJunction(i, j)).getXLeftSquare() - ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getWidth() / 2), (*mapClass.getMapJunction(i, j)).getYBotSquare() - ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getHeight() / 2), 0.0));
							break;
						}
						ModelViewMatrix = ViewMatrix * moveLight;
						(*mapClass.getMapJunction(i, j)).getTrafficLights()[k].Render(shader, ModelViewMatrix, ProjectionMatrix, moveLight, hour);
						


					}
				}
			}
		}
	}
	//works out if each car is colliding with a junction, else it respawns it
	for (int i = 0; i < cars.size(); i++) {
		for (int j = 0; j < mapClass.getMap().size(); j++) {
			for (int k = 0; k < mapClass.getMap()[0].size(); k++) {
				if (cars[i].getJunction()->getType() == RoadType::N) {
					//cout << "car junction type = " << cars[i].getJunction()->getType() << endl;
					goto respawn;
				}
				if (cars[i].IsInCollision(mapClass.getMapJunction(j, k)->GetOBB())) {					
					if (cars[i].getJunction() != mapClass.getMapJunction(j, k) && mapClass.getMapJunction(j, k)->getIdentifier() != cars[i].getPreviousJunction()) {
						if (mapClass.getMapJunction(j, k)->getType() != RoadType::N) {
							cars[i].setPreviousJunction(cars[i].getJunction()->getIdentifier());
							cars[i].setJunction((mapClass.getMapJunction(j, k)));
							cars[i].getJunction()->setIdentifier(mapClass.getMapJunction(j, k)->getIdentifier());
							cars[i].entryPoint();
						}
						else {
							goto respawn;
						}
					}
					goto end;
				}
			}
		}
	respawn:
		if (mapClass.getSpawns().size() > 0) {
			int randomSpawn = rand() % mapClass.getSpawns().size(), randomExit;
			//https://stackoverflow.com/questions/21813602/three-random-numbers-which-are-not-equal-to-each-other
			do randomExit = rand() % mapClass.getSpawns().size(); while (randomSpawn == randomExit);
			std::pair<int, int> spawnJunctionIndex = mapClass.getSpawns()[randomSpawn].first;
			std::pair<int, int> exitJunctionIndex = mapClass.getSpawns()[randomExit].first;
			cars[i].setJunction((mapClass.getMapJunction(spawnJunctionIndex.first, spawnJunctionIndex.second)));
			cars[i].respawn(cars[i].getJunction(), mapClass.getSpawns()[randomSpawn].second);
			cars[i].setPath(mapClass.pathfinder((*cars[i].getJunction()), mapClass.getSpawns()[randomSpawn].second,
				* mapClass.getMapJunction(exitJunctionIndex.first, exitJunctionIndex.second), mapClass.getSpawns()[randomExit].second).second);
			if (debug) {
				std::cout << "spawn = " << spawnJunctionIndex.first << ", " << spawnJunctionIndex.second << std::endl;
				std::cout << "exit = " << exitJunctionIndex.first << ", " << exitJunctionIndex.second << std::endl;
				for (int z = 0; z < cars[i].getPath().size(); z++) {
					std::cout << "z = " << z << ", " << cars[i].getPath()[z] << std::endl;
				}
			}
			cars[i].setPreviousJunction("");
		}
		
	end:
		continue;
	}

	int numSpawns = mapClass.getSpawns().size();
	vector<vector<pair<Car, int>>> filledSpawns;
	filledSpawns.resize(numSpawns);
	
	//vector<int> alreadyChecked;
	//alreadyChecked.resize(cars.size());
	//cout << "Num spawns = " << numSpawns << endl;
	for (int k = 0; k < numSpawns; k++) {
		OBB boxCheck;
		for (int i = 0; i < cars.size(); i++) {
			int carY = cars[i].getJunction()->getYPosition();
			int carX = cars[i].getJunction()->getXPosition();
			pair<int, int> carIndicies;
			carIndicies.first = carY;
			carIndicies.second = carX;
			if (mapClass.getSpawns()[k].first == carIndicies) {
				Junction* current = mapClass.getMapJunction(carY, carX);
				int entryPointBox = mapClass.getSpawns()[k].second;
				switch (entryPointBox) {
				case 0:
					boxCheck.vert[0].x = current->GetXPos() - (current->getWidth() / 2);
					boxCheck.vert[0].y = current->GetYPos();
					boxCheck.vert[1].x = current->getRightInner();
					boxCheck.vert[1].y = current->GetYPos();
					boxCheck.vert[2].x = current->getRightInner();
					boxCheck.vert[2].y = current->getYTopSquare();
					boxCheck.vert[3].x = current->GetXPos() - (current->getWidth() / 2);
					boxCheck.vert[3].y = current->getYTopSquare();
					break;
				case 1:
					boxCheck.vert[0].x = current->GetXPos() + (current->getWidth() / 2);
					boxCheck.vert[0].y = current->getYBotSquare();
					boxCheck.vert[1].x = current->getLeftInner();
					boxCheck.vert[1].y = current->getYBotSquare();
					boxCheck.vert[2].x = current->getLeftInner();
					boxCheck.vert[2].y = current->GetYPos();
					boxCheck.vert[3].x = current->GetXPos() + (current->getWidth() / 2);
					boxCheck.vert[3].y = current->GetYPos();
					break;
				case 2:
					boxCheck.vert[0].x = current->GetXPos();
					boxCheck.vert[0].y = current->getBotInner();
					boxCheck.vert[1].x = current->getXRightSquare();
					boxCheck.vert[1].y = current->getBotInner();
					boxCheck.vert[2].x = current->getXRightSquare();
					boxCheck.vert[2].y = current->GetYPos() + (current->getHeight() / 2);
					boxCheck.vert[3].x = current->GetXPos();
					boxCheck.vert[3].y = current->GetYPos() + (current->getHeight() / 2);
					break;
				case 3:
					boxCheck.vert[0].x = current->getXLeftSquare();
					boxCheck.vert[0].y = current->GetYPos() - (current->getHeight() / 2);
					boxCheck.vert[1].x = current->GetXPos();
					boxCheck.vert[1].y = current->GetYPos() - (current->getHeight() / 2);
					boxCheck.vert[2].x = current->GetXPos();
					boxCheck.vert[2].y = current->getTopInner();
					boxCheck.vert[3].x = current->getXLeftSquare();
					boxCheck.vert[3].y = current->getTopInner();
					break;
				}
				//cout << "Vert 0 x = " << cars[i].getSpawnOBB().vert[0].x << ", Y = " << cars[i].getSpawnOBB().vert[0].y << endl;
				//cout << "Vert 1 x = " << cars[i].getSpawnOBB().vert[1].x << ", Y = " << cars[i].getSpawnOBB().vert[1].y << endl;
				//cout << "Vert 2 x = " << cars[i].getSpawnOBB().vert[2].x << ", Y = " << cars[i].getSpawnOBB().vert[2].y << endl;
				//cout << "Vert 3 x = " << cars[i].getSpawnOBB().vert[3].x << ", Y = " << cars[i].getSpawnOBB().vert[3].y << endl;

				//cout << "Vert 0 x = " << boxCheck.vert[0].x << ", Y = " << boxCheck.vert[0].y << endl;
				//cout << "Vert 1 x = " << boxCheck.vert[1].x << ", Y = " << boxCheck.vert[1].y << endl;
				//cout << "Vert 2 x = " << boxCheck.vert[2].x << ", Y = " << boxCheck.vert[2].y << endl;
				//cout << "Vert 3 x = " << boxCheck.vert[3].x << ", Y = " << boxCheck.vert[3].y << endl;


				//if (i == 2) {
					//cout << "Vert 0 x = " << cars[i].GetOBB().vert[0].x << ", Y = " << cars[i].GetOBB().vert[0].y << endl;
					//cout << "Vert 1 x = " << cars[i].GetOBB().vert[1].x << ", Y = " << cars[i].GetOBB().vert[1].y << endl;
					//cout << "Vert 2 x = " << cars[i].GetOBB().vert[2].x << ", Y = " << cars[i].GetOBB().vert[2].y << endl;
					//cout << "Vert 3 x = " << cars[i].GetOBB().vert[3].x << ", Y = " << cars[i].GetOBB().vert[3].y << endl;
				//}

				//if (cars[i].getSpawnOBB().SAT2D(boxCheck) == 1) {
				//	cout << "True" << endl;
				//}
				//else {
				//	cout << "False" << endl;
				//}
				if (cars[i].IsInCollision(boxCheck) || cars[i].getSpawnOBB().SAT2D(boxCheck)) {
					pair<Car, int> carInBox;
					carInBox.first = cars[i];
					carInBox.second = i;
					filledSpawns.at(k).push_back(carInBox);
					//cout << "Inside the box is " << i << endl;
				}
			}
		}
			int indexOfBestCar = 0;
			float bestDist = INT_MAX;
			//cout << "Number of cars in spawn " << k << " = " << filledSpawns.at(k).size() << endl;
			for (int l = 0; l < filledSpawns.at(k).size(); l++) {
				// Check which car is closer to the centre of the junction and keep that car
				Car next = filledSpawns.at(k).at(l).first;
				Junction* current = next.getJunction();
				float distX = (next.GetXPos() - current->GetXPos()) * (next.GetXPos() - current->GetXPos());
				float distY = (next.GetYPos() - current->GetYPos()) * (next.GetYPos() - current->GetYPos());
				if (distX + distY < bestDist) {
					bestDist = distX + distY;
					indexOfBestCar = filledSpawns.at(k).at(l).second;
				}
			}
			vector<int> deletedCars;
			//cout << k << endl;
			for (int l = 0; l < filledSpawns.at(k).size(); l++) {
				Car next = filledSpawns.at(k).at(l).first;
				int index = filledSpawns.at(k).at(l).second;
				if (index != indexOfBestCar) {
					// Correct the index if other cars have been deleted
					for (int j = 0; j < deletedCars.size(); j++) {
						//cout << "Correction" << endl;
						if (deletedCars[j] < index) {
							index--;
						}
					}
					//cout << "nextCar position = " << next.GetXPos() << ", " << next.GetYPos() << endl;
					//cout << "Car position in car list = " << cars[index].GetXPos() << ", " << cars[index].GetYPos() << endl;
					//cout << "Car index = " << index << endl;
					//buffer.at(k).push_back(next);
					//cout << "DELETE" << endl;
					cars.erase(cars.begin() + index);
					deletedCars.push_back(index);
				}
			}
		//cout << "\n";
	}

	for (int i = 0; i < cars.size(); i++) {
		int direction = cars[i].decideDirection(cars[i].getEntryTurning());
		ModelMatrix = cars[i].rotate(12.0f / fps, direction, cars[i].getEntryTurning(), fps, cars);
		ModelViewMatrix = ViewMatrix * ModelMatrix;
		cars[i].Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix, 100);
		cars[i].setCurrentlyRendered(true);
	}

	//for (int i = 0; i < cars.size(); i++) {
	//	//cout << "i = " << i << endl;
	//	//bool avoidDoubleCheck = false;
	//	//for (int k = 0; k < alreadyChecked.size(); k++) {
	//	//	if (alreadyChecked.at(k) == i) {
	//	//		avoidDoubleCheck = true;
	//	//	}
	//	//}
	//	for (int j = 0; j < cars.size(); j++) {
	//		if (j == i) {
	//			continue;
	//		}
	//		//if (cars[i].IsInCollision(cars[j].getCollide()) && !avoidDoubleCheck) {
	//			int carY = stoi( cars[i].getJunction()->getIdentifier().substr(0,1) );
	//			int carX = stoi( cars[i].getJunction()->getIdentifier().substr(1, 1) );
	//			pair<int, int> carIndicies;
	//			carIndicies.first = carY;
	//			carIndicies.second = carX;
	//			for (int k = 0; k < numSpawns; k++) {
	//				if (mapClass.getSpawns()[k] == carIndicies) {
	//					// Determine which car is on the junction already and which one isn't so that the one not on the junction is never spawned rather than the one already spawned despawning
	//					OBB boxCheck;
	//					Junction* current = mapClass.getMapJunction(carX, carY);
	//					int entryPointBox = current->getSpawnable().second;
	//					switch (entryPointBox) {
	//					case 0:
	//						boxCheck.vert[0].x = current->GetXPos() - (current->getWidth() / 2);
	//						boxCheck.vert[0].y = current->getYBotSquare();
	//						boxCheck.vert[1].x = current->getRightInner();
	//						boxCheck.vert[1].y = current->getYBotSquare();
	//						boxCheck.vert[2].x = current->getRightInner();
	//						boxCheck.vert[2].y = current->getYTopSquare();
	//						boxCheck.vert[3].x = current->GetXPos() - (current->getWidth() / 2);
	//						boxCheck.vert[3].y = current->getYTopSquare();
	//						break;
	//					case 1:
	//						break;
	//					case 2:
	//						break;
	//					case 3:
	//						break;
	//					}
	//					if (cars[i].IsInCollision(boxCheck)) {
	//						filledSpawns.at(k).push_back(cars[i]);
	//						cout << "Inside the box is " << i << endl;
	//					}
	//					//cout << "Box check vert 0 = " << boxCheck.vert[0].x << ", " << boxCheck.vert[0].y << endl;
	//					//cout << "Box check vert 1 = " << boxCheck.vert[1].x << ", " << boxCheck.vert[1].y << endl;
	//					//cout << "Box check vert 2 = " << boxCheck.vert[2].x << ", " << boxCheck.vert[2].y << endl;
	//					//cout << "Box check vert 3 = " << boxCheck.vert[3].x << ", " << boxCheck.vert[3].y << endl;
	//					//cout << "Car vertex 0 = " << cars[i].GetOBB().vert[0].x << ", " << cars[i].GetOBB().vert[0].y << endl;
	//					//cout << "Car vertex 1 = " << cars[i].GetOBB().vert[1].x << ", " << cars[i].GetOBB().vert[1].y << endl;
	//					//cout << "Car vertex 2 = " << cars[i].GetOBB().vert[2].x << ", " << cars[i].GetOBB().vert[2].y << endl;
	//					//cout << "Car vertex 3 = " << cars[i].GetOBB().vert[3].x << ", " << cars[i].GetOBB().vert[3].y << endl;

	//					//cout << "i = " << i << " Render status = " << cars[i].getCurrentlyRendered() << endl;

	//						//alreadyChecked.push_back(j);
	//						//buffer.at(k).push_back(cars[i]);
	//						//cars.erase(cars.begin() + i);
	//						//if(!cars[j].IsInCollision(boxCheck)) {
	//						//	alreadyChecked.push_back(i);
	//						//	buffer.at(k).push_back(cars[j]);
	//						//	cars.erase(cars.begin() + j);
	//						//}

	//						//goto noRenderForYou;
	//					//}
	//					//cout << "Outside the box is " << i << endl;
	//				}
	//			//}
	//		}
	//	}
	//	for (int k = 0; k < numSpawns; k++) {
	//		if (!buffer.at(k).empty()) {
	//			Junction* check = mapClass.getMapJunction(mapClass.getSpawns().at(k).first, mapClass.getSpawns().at(k).second);
	//			int spawnEntry = check->getSpawnable().second;
	//			bool freeSpawn = false;
	//			switch (spawnEntry) {
	//			case 0:
	//				if (cars[i].GetXPos() <= check->getLeftInner() && cars[i].GetXPos() >= check->GetXPos() - (check->getWidth() / 2) &&
	//					cars[i].GetYPos() <= check->getYTopSquare() && cars[i].GetYPos() >= check->getYBotSquare()
	//					) {
	//					//filledSpawns.at(k) = true;
	//				}
	//				break;
	//			case 1:
	//				if (cars[i].GetXPos() >= check->getRightInner() && cars[i].GetXPos() <= check->GetXPos() + (check->getWidth() / 2) &&
	//					cars[i].GetYPos() <= check->getYTopSquare() && cars[i].GetYPos() >= check->getYBotSquare()
	//					) {
	//					//filledSpawns.at(k) = true;
	//				}
	//				break;
	//			case 2:
	//				if (cars[i].GetYPos() >= check->getTopInner() && cars[i].GetYPos() <= check->GetYPos() + (check->getHeight() / 2) &&
	//					cars[i].GetXPos() <= check->getXRightSquare() && cars[i].GetXPos() >= check->getXLeftSquare()
	//					) {
	//					//filledSpawns.at(k) = true;
	//				}
	//				break;
	//			case 3:
	//				if (cars[i].GetYPos() <= check->getBotInner() && cars[i].GetYPos() >= check->GetYPos() - (check->getHeight() / 2) &&
	//					cars[i].GetXPos() <= check->getXRightSquare() && cars[i].GetXPos() >= check->getXLeftSquare()
	//					) {
	//					//filledSpawns.at(k) = true;
	//				}
	//				break;
	//			}
	//		}
	//	}
	//	int direction = cars[i].decideDirection(cars[i].getEntryTurning());
	//	ModelMatrix = cars[i].rotate(12.0f / fps, direction, cars[i].getEntryTurning(), fps, cars);
	//	ModelViewMatrix = ViewMatrix * ModelMatrix;
	//	//cout << "i = " << i << endl;
	//	cars[i].Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix);
	//	cars[i].setCurrentlyRendered(true);
	//	goto hopSkipAndAJump;
	//noRenderForYou:
	//	//cars[i].SetXpos(INT_MAX);
	//	//cars[i].SetYpos(INT_MAX);
	//	//cout << "" << endl;
	//	continue;
	//hopSkipAndAJump:
	//	cout;
	//}
	//for (int i = 0; i < numSpawns; i++) {
	//	//if (filledSpawns.at(i) == false && !buffer.at(i).empty()) {
	//		//int direction = buffer.at(i).at(0).decideDirection(buffer.at(i).at(0).getEntryTurning());
	//		//ModelMatrix = buffer.at(i).at(0).rotate(12.0f / fps, direction, cars[i].getEntryTurning(), fps, cars);
	//		//ModelViewMatrix = ViewMatrix * ModelMatrix;
	//		//buffer.at(i).at(0).Render(shader, ModelViewMatrix, ProjectionMatrix, ModelMatrix);
	//	//}
	//}

	glDisable(GL_BLEND);

	glutSwapBuffers();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	//std::chrono::duration<double> elapsed = end - begin;


	secondElapsed += chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	hourElapsed += chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	fps = 1000000.0f / chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	//cout << "fps = " << fps << endl;
	//cout << "hour = " << hour << endl;
	//std::this_thread::sleep_for(std::chrono::milliseconds((1000 / fps) - chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()));

}

void init()
{
	


	float red[3] = { 1,0,0 };
	FreeImage_Initialise();

	glClearColor((0.0f)*0.25,(153.0f/256.0f)*0.25,(68.0f/256.0f) * 0.25,(0.0f)*0.25);						//sets the clear colour to black
	 
	//Load the GLSL program 
	if (!shader.load("Basic", "./glslfiles/basicTexture.vert", "./glslfiles/basicTexture.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}
	///This part commented is to scale the width of the sprite to match the dimensions of the car.png image.

	//mapClass.addJunction(Junction::Junction("X", 0, 0, glm::mat4(1.0f), RoadType::X),0,1);
	//mapClass.addJunction(Junction::Junction("X", 0, 0, glm::mat4(1.0f), RoadType::X),0,2);
	//mapClass.getMapJunction(0, 2)->setSpawnable(true, 1);
	//mapClass.getMapJunction(0, 2)->setOrientation(2);
	//mapClass.addJunction(Junction::Junction("X", 0, 0, glm::mat4(1.0f), RoadType::X), 0, 0);
	//mapClass.getMapJunction(0, 0)->setSpawnable(true, 0);
	tJunction.SetWidth(15.0f * scale * (2481 / 2481.0f));
	tJunction.SetHeight(15.0f * scale);
	tJunction.Init(shader, red, "textures/Tjunction.png");
	xJunction.SetWidth(15.0f * scale * (2481 / 2481.0f));
	xJunction.SetHeight(15.0f * scale);
	xJunction.Init(shader, red, "textures/Xjunction.png");
	road.SetWidth(15.0f * scale * (2481 / 2481.0f));
	road.SetHeight(15.0f * scale);
	road.Init(shader, red, "textures/Road.png");
	emptyJunction.SetWidth(15.0f * scale * (2481 / 2481.0f));
	emptyJunction.SetHeight(15.0f * scale);
	emptyJunction.Init(shader, red, "textures/blank.png");
	basicTrafficLight.SetHeight(4.0f * scale);
	basicTrafficLight.SetWidth(4.0f * scale / 2);
	basicTrafficLight.Init(shader, red, "textures/blankTrafficLight.png");
	basicTrafficLight.InitLights(shader, red, "textures/redTrafficLight.png", "textures/rAndATrafficLight.png", 
		"textures/greenTrafficLight.png", "textures/amberTrafficLight.png");
	basicCar.SetWidth(scale * (500 / 264.0f));
	basicCar.SetHeight(scale);
	basicCar.Init(shader, red, "textures/car.png");


	//mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 0, 1);
	//mapClass.getMapJunction(0, 1)->setOrientation(0, &basicTrafficLight);
	//mapClass.getMapJunction(0, 1)->setSpawnable(true, { 2 });

	//mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 1, 0);
	//mapClass.getMapJunction(1, 0)->setOrientation(1, &basicTrafficLight);
	//mapClass.getMapJunction(1, 0)->setSpawnable(true, { 0 });
	//mapClass.addJunction(Junction::Junction(&xJunction, &basicTrafficLight), 1, 1);
	//mapClass.getMapJunction(1, 1)->setOrientation(2, &basicTrafficLight);
	//mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 1, 2);
	//mapClass.getMapJunction(1, 2)->setOrientation(1, &basicTrafficLight);
	//mapClass.getMapJunction(1, 2)->setSpawnable(true, { 1 });

	//mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 2, 1);
	//mapClass.getMapJunction(2, 1)->setOrientation(0, &basicTrafficLight);
	//mapClass.getMapJunction(2, 1)->setSpawnable(true, { 3 });
	switch (mapSelect)
	{
	case 2:
		mapClass = Map::Map(3, 3);

		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 0, 1);
		mapClass.getMapJunction(0, 1)->setOrientation(0, &basicTrafficLight);
		mapClass.getMapJunction(0, 1)->setSpawnable(true, { 2 });

		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 1, 0);
		mapClass.getMapJunction(1, 0)->setOrientation(1, &basicTrafficLight);
		mapClass.getMapJunction(1, 0)->setSpawnable(true, { 0 });
		mapClass.addJunction(Junction::Junction(&xJunction, &basicTrafficLight), 1, 1);
		mapClass.getMapJunction(1, 1)->setOrientation(2, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 1, 2);
		mapClass.getMapJunction(1, 2)->setOrientation(1, &basicTrafficLight);
		mapClass.getMapJunction(1, 2)->setSpawnable(true, { 1 });

		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 2, 1);
		mapClass.getMapJunction(2, 1)->setOrientation(0, &basicTrafficLight);
		mapClass.getMapJunction(2, 1)->setSpawnable(true, { 3 });
		
		
		break;
	case 3:
		
		mapClass = Map::Map(2, 3);

		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 0, 0);
		mapClass.getMapJunction(0, 0)->setOrientation(1, &basicTrafficLight);
		mapClass.getMapJunction(0, 0)->setSpawnable(true, { 0 });
		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 0, 1);
		mapClass.getMapJunction(0, 1)->setOrientation(0, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 0, 2);
		mapClass.getMapJunction(0, 2)->setOrientation(1, &basicTrafficLight);
		mapClass.getMapJunction(0, 2)->setSpawnable(true, { 1 });

		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 1, 1);
		mapClass.getMapJunction(1, 1)->setOrientation(0, &basicTrafficLight);
		mapClass.getMapJunction(1, 1)->setSpawnable(true, { 3 });
		break;
	case 4:
		mapClass = Map::Map(7, 7);
		//row 1
		mapClass.addJunction(Junction::Junction(road), 0, 1);
		mapClass.getMapJunction(0, 1)->setSpawnable(true, { 2 });
		mapClass.addJunction(Junction::Junction(road), 0, 5);
		mapClass.getMapJunction(0, 5)->setSpawnable(true, { 2 });
		//row 2
		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 1, 1);
		mapClass.getMapJunction(1, 1)->setOrientation(1, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(road), 1, 2);
		mapClass.getMapJunction(1, 2)->setOrientation(1, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(road), 1, 3);
		mapClass.getMapJunction(1, 3)->setOrientation(1, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(road), 1, 4);
		mapClass.getMapJunction(1, 4)->setOrientation(1, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&xJunction, &basicTrafficLight), 1, 5);
		mapClass.getMapJunction(1, 5)->setOrientation(0, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(road), 1, 6);
		mapClass.getMapJunction(1, 6)->setOrientation(1, &basicTrafficLight);
		mapClass.getMapJunction(1, 6)->setSpawnable(true, { 1 });
		//row 3 
		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 2, 1);
		mapClass.getMapJunction(2, 1)->setOrientation(1, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 2, 2);
		mapClass.getMapJunction(2, 2)->setOrientation(0, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(road), 2, 3);
		mapClass.getMapJunction(2, 3)->setOrientation(1, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 2, 4);
		mapClass.getMapJunction(2, 4)->setOrientation(0, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 2, 5);
		mapClass.getMapJunction(2, 5)->setOrientation(3, &basicTrafficLight);
		//row 4
		mapClass.addJunction(Junction::Junction(road), 3, 1);
		mapClass.addJunction(Junction::Junction(road), 3, 2);
		mapClass.addJunction(Junction::Junction(road), 3, 4);
		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 3, 5);
		mapClass.getMapJunction(3, 5)->setOrientation(1, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(road), 3, 6);
		mapClass.getMapJunction(3, 6)->setOrientation(1, &basicTrafficLight);
		mapClass.getMapJunction(3, 6)->setSpawnable(true, { 1 });
		//row 5
		mapClass.addJunction(Junction::Junction(road), 4, 1);
		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 4, 2);
		mapClass.getMapJunction(4, 2)->setOrientation(1, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(road), 4, 3);
		mapClass.getMapJunction(4, 3)->setOrientation(1, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 4, 4);
		mapClass.getMapJunction(4, 4)->setOrientation(3, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(road), 4, 5);
		//row 6
		mapClass.addJunction(Junction::Junction(road), 5, 0);
		mapClass.getMapJunction(5, 0)->setOrientation(1, &basicTrafficLight);
		mapClass.getMapJunction(5, 0)->setSpawnable(true, { 0 });
		mapClass.addJunction(Junction::Junction(&xJunction, &basicTrafficLight), 5, 1);
		mapClass.getMapJunction(5, 1)->setOrientation(0, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&xJunction, &basicTrafficLight), 5, 2);
		mapClass.getMapJunction(5, 2)->setOrientation(0, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(road), 5, 3);
		mapClass.getMapJunction(5, 3)->setOrientation(1, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 5, 4);
		mapClass.getMapJunction(5, 4)->setOrientation(2, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 5, 5);
		mapClass.getMapJunction(5, 5)->setOrientation(3, &basicTrafficLight);
		//row 7
		mapClass.addJunction(Junction::Junction(road), 6, 1);
		mapClass.getMapJunction(6, 1)->setSpawnable(true, { 3 });
		mapClass.addJunction(Junction::Junction(road), 6, 2);
		mapClass.getMapJunction(6, 2)->setSpawnable(true, { 3 });
		mapClass.addJunction(Junction::Junction(road), 6, 5);
		mapClass.getMapJunction(6, 5)->setSpawnable(true, { 3 });

		break;
	case 9:
		
		mapClass = Map::Map(4, 4);
		//row 1
		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 0, 1);
		mapClass.getMapJunction(0, 1)->setOrientation(0, &basicTrafficLight);
		mapClass.getMapJunction(0, 1)->setSpawnable(true, { 2 });
		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 0, 2);
		mapClass.getMapJunction(0, 2)->setOrientation(0, &basicTrafficLight);
		mapClass.getMapJunction(0, 2)->setSpawnable(true, { 2 });

		//row 2
		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 1, 0);
		mapClass.getMapJunction(1, 0)->setOrientation(1, &basicTrafficLight);
		mapClass.getMapJunction(1, 0)->setSpawnable(true, { 0 });
		mapClass.addJunction(Junction::Junction(&xJunction, &basicTrafficLight), 1, 1);
		mapClass.getMapJunction(1, 1)->setOrientation(0, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&xJunction, &basicTrafficLight), 1, 2);
		mapClass.getMapJunction(1, 2)->setOrientation(0, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 1, 3);
		mapClass.getMapJunction(1, 3)->setOrientation(1, &basicTrafficLight);
		mapClass.getMapJunction(1, 3)->setSpawnable(true, { 1 });

		//row 3
		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 2, 0);
		mapClass.getMapJunction(2, 0)->setOrientation(1, &basicTrafficLight);
		mapClass.getMapJunction(2, 0)->setSpawnable(true, { 0 });
		mapClass.addJunction(Junction::Junction(&xJunction, &basicTrafficLight), 2, 1);
		mapClass.getMapJunction(2, 1)->setOrientation(0, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&xJunction, &basicTrafficLight), 2, 2);
		mapClass.getMapJunction(2, 2)->setOrientation(0, &basicTrafficLight);
		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 2, 3);
		mapClass.getMapJunction(2, 3)->setOrientation(1, &basicTrafficLight);
		mapClass.getMapJunction(2, 3)->setSpawnable(true, { 1 });

		//row 4 
		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 3, 1);
		mapClass.getMapJunction(3, 1)->setOrientation(0, &basicTrafficLight);
		mapClass.getMapJunction(3, 1)->setSpawnable(true, { 3 });
		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 3, 2);
		mapClass.getMapJunction(3, 2)->setOrientation(0, &basicTrafficLight);
		mapClass.getMapJunction(3, 2)->setSpawnable(true, { 3 });
	case 1:
	default:
		mapClass = Map::Map(3, 3);

		mapClass.addJunction(Junction::Junction(&road, &basicTrafficLight), 0, 0);
		mapClass.getMapJunction(0, 0)->setOrientation(1, &basicTrafficLight);

		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 0, 2);
		mapClass.getMapJunction(0, 2)->setOrientation(0, &basicTrafficLight);

		mapClass.addJunction(Junction::Junction(&xJunction, &basicTrafficLight), 2, 0);
		mapClass.getMapJunction(2, 0)->setOrientation(1, &basicTrafficLight);

		mapClass.addJunction(Junction::Junction(&tJunction, &basicTrafficLight), 2, 2);
		mapClass.getMapJunction(2, 2)->setOrientation(2, &basicTrafficLight);
		mapClass.getMapJunction(2, 2)->setSpawnable(true, { 0 });

		break;
	}


	//mapClass.addJunction(road, 0, 1);
	//(*mapClass.getMapJunction(0, 1)).setOrientation(1);
	//(*mapClass.getMapJunction(0, 1)).setSpawnable(true, 0);
	//mapClass.addJunction(road, 0, 1);
	//(*mapClass.getMapJunction(0, 1)).setOrientation(0);
	//(*mapClass.getMapJunction(0, 1)).setSpawnable(true, 2);
	//mapClass.addJunction(xJunction, 1, 1);
	//mapClass.addJunction(road, 1, 0);
	//(*mapClass.getMapJunction(1, 0)).setOrientation(1);
	//(*mapClass.getMapJunction(1, 0)).setSpawnable(true, 0);
	//mapClass.addJunction(road, 1, 2);
	//(*mapClass.getMapJunction(1, 2)).setOrientation(1);
	//(*mapClass.getMapJunction(1, 2)).setSpawnable(true, 1);
	//mapClass.addJunction(road, 2, 1);
	//(*mapClass.getMapJunction(2, 1)).setOrientation(0);
	//(*mapClass.getMapJunction(2, 1)).setSpawnable(true, 3);
	//
	for (int i = 0; i < mapClass.getMap().size(); i++) {
		for (int j = 0; j < mapClass.getMap()[0].size(); j++) {
			Junction pointer = *mapClass.getMapJunction(i, j);
			(*mapClass.getMapJunction(i, j)).setPosition(j, i);
			(*mapClass.getMapJunction(i, j)).SetXpos(j* (*mapClass.getMapJunction(i, j)).getWidth());
			(*mapClass.getMapJunction(i, j)).SetYpos(-i* (*mapClass.getMapJunction(i, j)).getHeight());
			(*mapClass.getMapJunction(i, j)).calculateLines();
			(*mapClass.getMapJunction(i, j)).setIdentifier(std::to_string(i) + std::to_string(j));

			//switch ((*mapClass.getMapJunction(i, j)).getType())
			//{
			//case(RoadType::N):
			//	(*mapClass.getMapJunction(i, j)).Init(shader, red, "textures/blank.png");
			//	break;
			//case(RoadType::S):
			//	(*mapClass.getMapJunction(i, j)).Init(shader, red, "textures/Road.png");
			//	break;
			//case(RoadType::T):
			//	(*mapClass.getMapJunction(i, j)).Init(shader, red, "textures/Tjunction.png");
			//	break;
			//case(RoadType::X):
			//default:
			//	(*mapClass.getMapJunction(i, j)).Init(shader, red, "textures/Xjunction.png");
			//	break;
			//}
			for (int k = 0; k < 4; k++) {
				if ((*mapClass.getMapJunction(i, j)).getType() == RoadType::S) {
					break;
				}
				if ((*mapClass.getMapJunction(i, j)).getTurnings()[k]) {
					(*mapClass.getMapJunction(i, j)).getTrafficLights()[k] = TrafficLight::TrafficLight(basicTrafficLight);
				}
			}
		}
	}
	mapClass.initialiseSpawns();
	buffer.resize(mapClass.getSpawns().size());

	for (int i = 0; i < cars.size(); i++) {
		cars[i].Init(shader, red, "textures/car.png");
	}
	//mapClass.aStar({ *mapClass.getMapJunction(1, 6) }, {}, 1, { *mapClass.getMapJunction(0, 5) }, 3, 7);
	//std::vector<int> path = mapClass.pathfinder({ *mapClass.getMapJunction(1, 6) }, 1, { *mapClass.getMapJunction(0, 5) }, 2).second;
	//for (int i = 0; i < path.size(); i++) {
	//	std::cout << "i = " << i << ", " << path[i] << std::endl;
	//} 
	Junction middle = mapClass.getMiddle();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Left = true;
		break;
	case GLUT_KEY_RIGHT:
		Right = true;
		break;
	case GLUT_KEY_UP:
		Up = true;
		break;
	case GLUT_KEY_DOWN:
		Down = true;
		break;
	}
}

void keyboard_down(unsigned char key, int x, int y) {
	switch (key) {
	case(','):
		zoomOut = true;
		break;
	case('.'):
		zoomIn = true;
		break;
	case('1'):
		mapSelect = 1;
		init();
		glClearColor((0.0f), (153.0f / 256.0f), (68.0f / 256.0f), (0.0f));
		cars = {};
		hour = 1000;
		secondsToHour = 10000000000.0f;
		init();
		break;
	case('2'):
		mapSelect = 2;
		cars = {};
		hour = 0;
		secondsToHour = 10.0f;
		init();
		break;
	case('3'):
		mapSelect = 3;
		cars = {};
		hour = 0;
		secondsToHour = 10.0f;
		init();
		break;
	case('4'):
		mapSelect = 4;
		cars = {};
		hour = 0;
		secondsToHour = 10.0f;
		init();
		break;
	case('5'):
		glClearColor((0.0f), (153.0f / 256.0f), (68.0f / 256.0f), (0.0f));
		cars = {};
		hour = 100;
		secondsToHour = 10000000000;
		break;
	case('6'):
		debug = !debug;
		break;

	}
	

	


}

void keyboard_up(unsigned char key, int x, int y) {
	switch (key) {
	case(','):
		zoomOut = false;
		break;
	case('.'):
		zoomIn = false;
		break;
	}

}

void specialUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		Left = false;
		break;
	case GLUT_KEY_RIGHT:
		Right = false;
		break;
	case GLUT_KEY_UP:
		Up = false;
		break;
	case GLUT_KEY_DOWN:
		Down = false;
		break;
	}
}

void processKeys()
{
	if (Left)
	{
		xpos -= 1000/fps;
	}
	if (Right)
	{
		xpos += 1000 / fps;
	}
	if (Up)
	{
		ypos += 1000 / fps;	
	}
	if (Down)
	{
		ypos -= 1000 / fps;
	}
	if (zoomIn) {
		zoom += 0.001;
		reshape(screenWidth, screenHeight);
	}
	if (zoomOut) {
		if (zoom > 0) {
			zoom -= 0.001;
			reshape(screenWidth, screenHeight);
		}
	}
}

void idle()
{
	processKeys();

	glutPostRedisplay();
}
/**************** END OPENGL FUNCTIONS *************************/

// FREEGLUT WINDOW SET UP
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL FreeGLUT Example: Image loading");

	glutReshapeFunc(reshape);

	//This initialises glew - it must be called after the window is created.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << " GLEW ERROR" << endl;
	}

	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	std::cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	//initialise the objects for rendering
	init();

	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard_down);
	glutKeyboardUpFunc(keyboard_up);
	glutSpecialUpFunc(specialUp);

	glutIdleFunc(idle);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}

/*
CODE DUMP

		Car car(glm::mat4(1.0f));
		float red[3] = { 1,0,0 };
		car.SetWidth(scale * (500 / 264.0f));
		car.SetHeight(scale);
		car.Init(shader, red, "textures/car.png");
		car.newSpawn(map[0], 3);
		cars.push_back(car);
		speed -= 0.03f;
		mySquare.IncPos(0.0f, -0.1f);
		direction = 1;

		Car car(glm::mat4(1.0f));
		float red[3] = { 1,0,0 };
		car.SetWidth(scale * (500 / 264.0f));
		car.SetHeight(scale);
		car.Init(shader, red, "textures/car.png");
		car.newSpawn(map[0], 2);
		cars.push_back(car);
		speed += 0.03f;
		mySquare.IncPos(0.0f, 0.1f);
		mySquare.SetYpos(0.0f);
		mySquare.SetXpos(0.0f);

		Car car(glm::mat4(1.0f));
		float red[3] = { 1,0,0 };
		car.SetWidth(scale * (500 / 264.0f));
		car.SetHeight(scale);
		car.Init(shader, red, "textures/car.png");
		car.newSpawn(map[0], 1);
		cars.push_back(car);
		angle -= 0.01f;
		mySquare.transform(1, 1);
		mySquare.IncPos(0.1f, 0.0f);

		Car car(glm::mat4(1.0f));
		float red[3] = { 1,0,0 };
		car.SetWidth(scale * (500 / 264.0f));
		car.SetHeight(scale);
		car.Init(shader, red, "textures/car.png");
		car.newSpawn(map[0], 0);
		cars.push_back(car);
		angle += 0.01f;
		speed = 0;
		mySquare.transform(1, 0);
		mySquare.IncPos(-0.1f, 0.0f);

	mySquare.Init(shader, red, "textures/car.png");
	car2.Init(shader, red, "textures/car.png");
	junction.Init(shader, red, "textures/Xjunction.png");
	std::cout << "size = " << junction.getTrafficLights().size() << std::endl;
	for (int i = 0; i < 4; i++) {
		junction.getTrafficLights()[i].SetHeight(scale);
		junction.getTrafficLights()[i].SetWidth(scale/2);
		//std::cout << junction.getTrafficLights()[i].getHeight() << std::endl;
		junction.getTrafficLights()[i].Init(shader, red, "textures/blankTrafficLight.png");
		junction.getTrafficLights()[i].InitLights(shader, red, "textures/redTrafficLight.png", "textures/rAndATrafficLight.png",
			"textures/greenTrafficLight.png", "textures/blankTrafficLight.png");
	}
	TrafficLight::initTrafficTex();

	junction.SetWidth(15.0f * scale * (2481 / 2481.0f));
	junction.SetHeight(15.0f * scale);
	junction.calculateLines();

	left
	coordinates[0][0] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].x;
	coordinates[0][1] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].y + ((*mapClass.getMapJunction(0, 0)).getHeight()*7/12);
	coordinates[0][2] = -90.0f;
	right
	coordinates[1][0] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].x+((*mapClass.getMapJunction(0, 0)).getWidth());
	coordinates[1][1] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].y + ((*mapClass.getMapJunction(0, 0)).getHeight() * 7 / 12);
	coordinates[1][2] = -90.0f;
	top
	coordinates[2][0] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].x + ((*mapClass.getMapJunction(0, 0)).getWidth() * 7 / 12);
	coordinates[2][1] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].y + ((*mapClass.getMapJunction(0, 0)).getHeight());
	coordinates[2][2] = 90.0f;
	bottom
	coordinates[3][0] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].x + (((*mapClass.getMapJunction(0, 0)).getWidth() * 5 / 12));
	coordinates[3][1] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].y;
	coordinates[3][2] = 270.0f;


					//for (int i = 0; i < cars.size(); i++) {
				//	Junction currentJunc = *mapClass.getMapJunction(1, 0);
				//	float carPosX = cars[i].GetXPos();
				//	float carPosY = cars[i].GetYPos();
				//	if (
				//		carPosX <= currentJunc.getLeftInner() &&
				//		carPosX >= (currentJunc.GetXPos() - (currentJunc.getWidth() * 1/2)) &&
				//		carPosY <= currentJunc.getYTopSquare() &&
				//		carPosY >= currentJunc.getYBotSquare() &&

				//		carPosX >= currentJunc.getRightInner() &&
				//		carPosX <= currentJunc.GetXPos() + (currentJunc.getWidth() * 1/2) &&
				//		carPosY <= currentJunc.getYTopSquare() &&
				//		carPosY >= currentJunc.getYBotSquare() &&

				//		carPosY >= currentJunc.getTopInner() &&
				//		carPosY <= currentJunc.GetYPos() + (currentJunc.getHeight() * 1/2) &&
				//		carPosX <= currentJunc.getXRightSquare() &&
				//		carPosX >= currentJunc.getXLeftSquare() &&

				//		carPosY <= currentJunc.getBotInner() &&
				//		carPosY >= currentJunc.GetYPos() - (currentJunc.getWidth() * 1 / 2) &&
				//		carPosX <= currentJunc.getXRightSquare() &&
				//		carPosX >= currentJunc.getXLeftSquare()
				//		) {
				//		//cout << "MATRIX DODGE" << endl;
				//		//goto noCar;
				//	}
				//}

					//if (hour > 16 && hour < 18) {
	//	if (secondElapsed >= 500000) {
	//		if (cars.size() < 200) {
	//			Car toSpawn = Car(glm::mat4(1.0f));
	//			toSpawn.SetWidth(scale * (500 / 264.0f));
	//			toSpawn.SetHeight(scale);
	//			toSpawn.setJunction(&emptyJunction);
	//			toSpawn.setIdentifier(cars.size());
	//			float red[3] = { 1,0,0 };
	//			toSpawn.Init(shader, red, "textures/car.png");
	//			//toSpawn.setJunction(mapClass.getMapJunction(1, 0));
	//			//toSpawn.respawn((mapClass.getMapJunction(1, 0)));
	//			cars.push_back(toSpawn);
	//		}
	//	}
	//	break;
	//case 10:
	//case 11:
	//case 12:
	//case 13:
	//case 14:
	//case 19:
	//case 20:
	//case 21:
	//	if (secondElapsed >= 1000000) {
	//		if (cars.size() < 75) {
	//			Car toSpawn = Car(glm::mat4(1.0f));
	//			toSpawn.SetWidth(scale * (500 / 264.0f));
	//			toSpawn.SetHeight(scale);
	//			toSpawn.setIdentifier(cars.size());
	//			float red[3] = { 1,0,0 };
	//			toSpawn.Init(shader, red, "textures/car.png");
	//			//toSpawn.setJunction(mapClass.getMapJunction(1, 0));
	//			//toSpawn.respawn((mapClass.getMapJunction(1, 0)));
	//			cars.push_back(toSpawn);
	//		}
	//	}
	//	break;
	//}

	//if (hour > 16 && hour < 18) {
	//	if (secondElapsed >= 500000) {
	//		if (cars.size() < 16) {
	//			Car toSpawn = Car(glm::mat4(1.0f));
	//			toSpawn.SetWidth(scale * (500 / 264.0f));
	//			toSpawn.SetHeight(scale);
	//			toSpawn.setIdentifier(cars.size());
	//			float red[3] = { 1,0,0 };
	//			toSpawn.Init(shader, red, "textures/car.png");
	//			//toSpawn.setJunction(mapClass.getMapJunction(1, 0));
	//			//toSpawn.respawn((mapClass.getMapJunction(1, 0)));
	//			cars.push_back(toSpawn);
	//		}
	//	}
	//}
	//else {
*/

