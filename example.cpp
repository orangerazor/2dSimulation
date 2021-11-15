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


#include <iostream>
using namespace std;
int fps = 40;

glm::mat4 ViewMatrix;  // matrix for the modelling and viewing
glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
int screenWidth = 480, screenHeight = 480;

//booleans to handle when the arrow keys are pressed or released.
int secondElapsed = 0;
float secondsToHour = 10.0f;
int hourElapsed = 0;
int hour;
bool Left = false;
bool Right = false;
bool Up = false;
bool Down = false;
int direction = 0;
float speed = 0;
const double PI = 3.141592653589793238463;
float angle = 0;
float scale = 4.0f;
float zoom = 0.5;


Shader shader;
Car mySquare = Car::Car(glm::mat4(1.0f));
Car car2 = Car::Car(glm::mat4(1.0f));
std::vector<Car> cars(0);
std::vector<Junction> map(0);
//std::vector< std::vector<Junction> > map(2, column);
Junction junction = Junction::Junction("T", 0, 0, glm::mat4(1.0f), RoadType::X);
Junction crossJunction = Junction::Junction("X", 0, 0, glm::mat4(1.0f), RoadType::X);
Junction road1 = Junction::Junction("s1", 1, 0, glm::mat4(1.0f), RoadType::S);
Junction road2 = Junction::Junction("s2", 1, 0, glm::mat4(1.0f), RoadType::S);
Junction road3 = Junction::Junction("s3", 0, 0, glm::mat4(1.0f), RoadType::S);
Junction road4 = Junction::Junction("s4", 0, 0, glm::mat4(1.0f), RoadType::S);
Junction emptyJunction = Junction::Junction();
Map mapClass = Map::Map(1, 1);

TrafficLight* trafficLights[1][4];
TrafficLight trafficLight;
float coordinates[4][3];


//OPENGL FUNCTION PROTOTYPES
void display();				//used as callback in glut for display.
void reshape(int width, int height); //used as callback for reshape function in glut
void init();				//called in main when the program starts.

/*************    START OF OPENGL FUNCTIONS   ****************/

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width;
	screenHeight = height;

	glViewport(0, 0, width, height);						// set Viewport dimensions

	ProjectionMatrix = glm::ortho(-25.0/zoom, 25.0/zoom, -25.0/zoom, 25.0/zoom);
}

void readjustScreen(int x, int y, int width, int height) {
	screenWidth = width;
	screenHeight = height;

	glViewport(x, y, width, height);
}


void display()

{
	//std::cout << "x = " << mySquare.GetXPos() << "y = " << mySquare.GetYPos() << std::endl;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT);
	//cout << "green = " << junction.getTrafficLights()[2].getLights()[2] << endl;
	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));
	//cout << "height=" << (*mapClass.getMapJunction(0,0)).getHeight() << ", " << "width=" << (*mapClass.getMapJunction(0, 0)).getWidth() << endl;
	glEnable(GL_BLEND);
	if (hourElapsed >= 1000000 * secondsToHour) { //day n night cycle
		if (hour == 23) {
			hour = 0;
		}
		else{
			hour++;
		}
		hourElapsed = 0;
	}
	if (hour > 15 && hour < 17) {
		if (secondElapsed >= 500000) {
			if (cars.size() < 8) {
				Car toSpawn = Car(glm::mat4(1.0f));
				toSpawn.SetWidth(scale * (500 / 264.0f));
				toSpawn.SetHeight(scale);
				toSpawn.setIdentifier(cars.size());
				float red[3] = { 1,0,0 };
				toSpawn.Init(shader, red, "textures/car.png");
				cars.push_back(toSpawn);
			}
		}
	}
	else {
		if (secondElapsed >= 1000000) {
			if (cars.size() < 4) {
				Car toSpawn = Car(glm::mat4(1.0f));
				toSpawn.SetWidth(scale * (500 / 264.0f));
				toSpawn.SetHeight(scale);
				toSpawn.setIdentifier(cars.size());
				float red[3] = { 1,0,0 };
				toSpawn.Init(shader, red, "textures/car.png");
				cars.push_back(toSpawn);
			}
		}
	}
	if (secondElapsed >= 1000000) {
		//if (cars.size() < 8) {
		//	Car toSpawn = Car(glm::mat4(1.0f));
		//	toSpawn.SetWidth(scale * (500 / 264.0f));
		//	toSpawn.SetHeight(scale);
		//	toSpawn.setIdentifier(cars.size());
		//	float red[3] = { 1,0,0 };
		//	toSpawn.Init(shader, red, "textures/car.png");
		//	cars.push_back(toSpawn);
		//}
		for (int i = 0; i < mapClass.getMap().size(); i++) {
			for (int j = 0; j < mapClass.getMap()[0].size(); j++) {
				(*mapClass.getMapJunction(i, j)).trafficLightFlow();
			}
		}
		//cout << "left" << junction.getTrafficLights()[0].getLights()[0] << junction.getTrafficLights()[0].getLights()[1] << junction.getTrafficLights()[0].getLights()[2] << endl;
		//cout << "right" << junction.getTrafficLights()[1].getLights()[0] << junction.getTrafficLights()[1].getLights()[1] << junction.getTrafficLights()[1].getLights()[2] << endl;
		//cout << "forward" << junction.getTrafficLights()[2].getLights()[0] << junction.getTrafficLights()[2].getLights()[1] << junction.getTrafficLights()[2].getLights()[2] << endl;
		//cout << "back" << junction.getTrafficLights()[3].getLights()[0] << junction.getTrafficLights()[3].getLights()[1] << junction.getTrafficLights()[3].getLights()[2] << endl;
		secondElapsed = 0;

	}
	for (int i = 0; i < mapClass.getMap().size(); i++) {
		for (int j = 0; j < mapClass.getMap()[0].size(); j++) {
			/*cout << i << ", " << j << endl;
			cout << mapClass.getMap()[i][j].getName() << endl;*/
			glm::mat4 junctionRender = glm::mat4(1.0f);
			junctionRender = glm::translate(glm::mat4(1.0f), glm::vec3((*mapClass.getMapJunction(i, j)).GetXPos(), (*mapClass.getMapJunction(i, j)).GetYPos(), 0));
			junctionRender = glm::rotate(junctionRender, glm::radians((*mapClass.getMapJunction(i, j)).getOrientation() * 90.0f), glm::vec3(0.0, 0.0, 1.0));
			(*mapClass.getMapJunction(i, j)).Render(shader, junctionRender, ProjectionMatrix);
			for (int k = 0; k < 4; k++) {
				glm::mat4 moveLight = glm::mat4(1.0f);
				switch (k) {
				case(0):
					moveLight = glm::translate(moveLight, glm::vec3((*mapClass.getMapJunction(i, j)).getXLeftSquare() - ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getHeight() / 2), (*mapClass.getMapJunction(i, j)).getYTopSquare() + ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getWidth() / 2), 0.0));
					moveLight = glm::rotate(moveLight, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
					break;
				case(1):
					moveLight = glm::translate(moveLight, glm::vec3((*mapClass.getMapJunction(i, j)).getXRightSquare() + ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getHeight() / 2), (*mapClass.getMapJunction(i, j)).getYBotSquare() - ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getWidth() / 2), 0.0));
					moveLight = glm::rotate(moveLight, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
					break;
				case(2):
					moveLight = glm::translate(moveLight, glm::vec3((*mapClass.getMapJunction(i, j)).getXRightSquare() + (map[k].getTrafficLights()[j].getWidth() / 2), (*mapClass.getMapJunction(i, j)).getYTopSquare() + ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getHeight() / 2), 0.0));
					moveLight = glm::rotate(moveLight, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
					break;
				case(3):
					moveLight = glm::translate(moveLight, glm::vec3((*mapClass.getMapJunction(i, j)).getXLeftSquare() - ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getWidth() / 2), (*mapClass.getMapJunction(i, j)).getYBotSquare() - ((*mapClass.getMapJunction(i, j)).getTrafficLights()[k].getHeight() / 2), 0.0));
					break;
				}
				(*mapClass.getMapJunction(i, j)).getTrafficLights()[k].Render(shader, moveLight, ProjectionMatrix);
			}
		}
	}
		
		


	//glm::mat4 ModelViewMatrix = mySquare.turn(speed, direction);
	//std::cout << mySquare.GetXPos() << ", " << mySquare.GetYPos() << std::endl;
	//mySquare.IncPos(0, 0.01f);
	/*glm::mat4 modelviewmatrix = mysquare.drive(speed, direction, angle) * viewmatrix;*/
	//if (mySquare.IsInCollision(junction.GetOBB())) {
		//direction = -1;
		//if (mySquare.IsInCollision(junction.GetOBB())) {
		//if (mySquare.GetYPos() > junction.getYBotSquare()) {
		//	ModelViewMatrix = mySquare.rotate(4.0f / fps, -1, entryPoint, junction, fps);
		//}
		//else {
		//	ModelViewMatrix = mySquare.rotate(4.0f / fps, 0, entryPoint, junction, fps);
		//}

		//std::cout << "entry = " << entryPoint << std::endl;
		//switch (entryPoint) {
		//case(0):
		//	ModelViewMatrix = glm::rotate(ModelViewMatrix, glm::radians(coordinates[0][2]), glm::vec3(0, 0, 1));
		//	break;
		//case(1):
		//	ModelViewMatrix = glm::rotate(ModelViewMatrix, glm::radians(coordinates[1][2]), glm::vec3(0, 0, 1));
		//	break;
		//case(2):
		//	ModelViewMatrix = glm::rotate(ModelViewMatrix, glm::radians(coordinates[2][2]), glm::vec3(0, 0, 1));
		//	break;
		//case(3):
		//	ModelViewMatrix = glm::rotate(ModelViewMatrix, glm::radians(coordinates[3][2]), glm::vec3(0, 0, 1));
		//	break;
		//}
	//junction.Render(shader, glm::mat4(1.0f), ProjectionMatrix);
	//for (int i = 0; i < 4; i++) {
	//	glm::mat4 moveLight = glm::mat4(1.0f);
	//	switch (i) {
	//	case(0):
	//		moveLight = glm::translate(moveLight, glm::vec3(junction.getXLeftSquare() - (junction.getTrafficLights()[i].getHeight() / 2), junction.getYTopSquare() + (junction.getTrafficLights()[i].getWidth() / 2), 0.0));
	//		moveLight = glm::rotate(moveLight, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
	//		break;
	//	case(1):
	//		moveLight = glm::translate(moveLight, glm::vec3(junction.getXRightSquare()+ (junction.getTrafficLights()[i].getHeight() / 2), junction.getYBotSquare() - (junction.getTrafficLights()[i].getWidth() / 2), 0.0));
	//		moveLight = glm::rotate(moveLight, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//		break;
	//	case(2):
	//		moveLight = glm::translate(moveLight, glm::vec3(junction.getXRightSquare() + (junction.getTrafficLights()[i].getWidth() / 2), junction.getYTopSquare() + (junction.getTrafficLights()[i].getHeight() / 2), 0.0));
	//		break;
	//	case(3):
	//		moveLight = glm::translate(moveLight, glm::vec3(junction.getXLeftSquare() - (junction.getTrafficLights()[i].getWidth() / 2), junction.getYBotSquare() - (junction.getTrafficLights()[i].getHeight() / 2), 0.0));
	//		moveLight = glm::rotate(moveLight, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
	//		break;
	//	}
	//	junction.getTrafficLights()[i].Render(shader, moveLight, ProjectionMatrix);
	//}
	//std::cout << first.GetXPos() << ", " << first.GetYPos() << std::endl;

	glm::mat4 ModelViewMatrix = glm::mat4(1.0f);
	ViewMatrix = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(20, 0, 0), glm::vec3(0.0f, 0.0f, 1.0f));
	//ProjectionMatrix = ViewMatrix;//glm::translate(ViewMatrix, glm::vec3(0, 0, 20));
	for (int i = 0; i < cars.size(); i++) {
		cars[i].respawn((*mapClass.getMapJunction(0, 0)));
		int direction = cars[i].decideDirection((*mapClass.getMapJunction(0, 0)), cars[i].getEntryTurning());
		ModelViewMatrix =  cars[i].rotate(12.0f / fps, direction, cars[i].getEntryTurning(), (*mapClass.getMapJunction(0, 0)), fps, cars);
		for (int j = 0; j < cars.size(); j++) {
			if (j == i) {
				continue;
			}
			if (cars[i].IsInCollision(cars[j].GetOBB())) {
				cars[i].respawn((*mapClass.getMapJunction(0, 0)));
				int direction = cars[i].decideDirection((*mapClass.getMapJunction(0, 0)), cars[i].getEntryTurning());
				ModelViewMatrix = cars[i].rotate(12.0f / fps, direction, cars[i].getEntryTurning(), (*mapClass.getMapJunction(0, 0)), fps, cars);
			}
		}
		cars[i].Render(shader, ModelViewMatrix, ProjectionMatrix);
	}
	//mySquare.respawn(map[0]);
	//int direction = mySquare.decideDirection(map[0], mySquare.getEntryTurning());
	//ModelViewMatrix = mySquare.rotate(12.0f / fps, direction, mySquare.getEntryTurning(), map[0], fps);
	//ModelViewMatrix = mySquare.faceJunction(entryPoint, ModelViewMatrix);
	//ModelViewMatrix = glm::rotate(ModelViewMatrix, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	//mySquare.respawn(junction);
	//mySquare.Render(shader, ModelViewMatrix, ProjectionMatrix);

	//}
	//else {
		//mySquare.SetXpos(0);
		//mySquare.SetYpos(-8);
		//mySquare.setCurrentJunction("");
		//ModelViewMatrix = glm::rotate(ModelViewMatrix, glm::radians(coordinates[0][2]), glm::vec3(0, 0, 1));


	//}


	//car2.respawn(map[0]);
	//int direction2 = car2.decideDirection(map[0], car2.getEntryTurning());
	//ModelViewMatrix = car2.rotate(12.0f / fps, direction2, car2.getEntryTurning(), map[0], fps);
	//ModelViewMatrix = car2.faceJunction(entryPoint2, ModelViewMatrix);
	//ModelViewMatrix = glm::rotate(ModelViewMatrix, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	//car2.respawn(junction);
	//car2.Render(shader, ModelViewMatrix, ProjectionMatrix);
	

	glDisable(GL_BLEND);

	glutSwapBuffers();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	secondElapsed += chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	hourElapsed += chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	//cout << (1000.0f/chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) << endl;
	fps = 1000000.0f / chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	//cout << fps<<endl;

	//std::this_thread::sleep_for(std::chrono::milliseconds((1000 / fps) - chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()));

}

void init()
{
	FreeImage_Initialise();

	glClearColor(0.0,153.0f/256.0f,68.0f/256.0f,0.0);						//sets the clear colour to black
	 
	//Load the GLSL program 
	if (!shader.load("Basic", "./glslfiles/basicTexture.vert", "./glslfiles/basicTexture.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}
	float red[3] = { 1,0,0 };
	///This part commented is to scale the width of the sprite to match the dimensions of the car.png image.
	mySquare.SetWidth(scale *(500 / 264.0f));
	mySquare.SetHeight(scale);
	car2.SetWidth(scale * (500 / 264.0f));
	car2.SetHeight(scale);
	//cars.push_back(mySquare);
	//cars.push_back(car2);
	for (int i = 0; i < cars.size(); i++) {
		cars[i].SetWidth(scale * (500 / 264.0f));
		cars[i].SetHeight(scale);
		cars[i].setIdentifier(i);
	}
	
	map.push_back(crossJunction);
	map.push_back(road1);
	map.push_back(road2);
	
	
	for (int i = 0; i < mapClass.getMap().size(); i++) {
		for (int j = 0; j < mapClass.getMap()[0].size(); j++) {
			/*cout << "i= " << i << ", j= " << j << endl;*/
			/*mapClass.getMap()[i][j] = crossJunction;*/
			mapClass.addJunction(crossJunction, i, j);
			cout << (*mapClass.getMapJunction(i, j)).getName() << endl;
			Junction pointer = *mapClass.getMapJunction(i, j);
			(*mapClass.getMapJunction(i, j)).SetWidth(15.0f * scale * (2481 / 2481.0f));
			(*mapClass.getMapJunction(i, j)).SetHeight(15.0f * scale);
			(*mapClass.getMapJunction(i, j)).SetXpos(j* (*mapClass.getMapJunction(i, j)).getWidth());
			(*mapClass.getMapJunction(i, j)).SetYpos(i* (*mapClass.getMapJunction(i, j)).getHeight());
			cout << "x = " << (*mapClass.getMapJunction(i, j)).GetXPos() << ", y = " << (*mapClass.getMapJunction(i, j)).GetYPos() << endl;
			(*mapClass.getMapJunction(i, j)).calculateLines();
			//cout << (*mapClass.getMapJunction(i, j)).getYBotSquare();

			switch ((*mapClass.getMapJunction(i, j)).getType())
			{
			case(RoadType::S):
				(*mapClass.getMapJunction(i, j)).Init(shader, red, "textures/Road.png");
				break;
			case(RoadType::T):
				(*mapClass.getMapJunction(i, j)).Init(shader, red, "textures/Tjunction.png");
				break;
			case(RoadType::X):
			default:
				(*mapClass.getMapJunction(i, j)).Init(shader, red, "textures/Xjunction.png");
				break;
			}
			for (int k = 0; k < 4; k++) {
				if ((*mapClass.getMapJunction(i, j)).getType() == RoadType::S) {
					break;
				}
				if ((*mapClass.getMapJunction(i, j)).getTurnings()[k]) {
					(*mapClass.getMapJunction(i, j)).getTrafficLights()[k].SetHeight(scale);
					(*mapClass.getMapJunction(i, j)).getTrafficLights()[k].SetWidth(scale / 2);
					//std::cout << junction.getTrafficLights()[i].getHeight() << std::endl;
					(*mapClass.getMapJunction(i, j)).getTrafficLights()[k].Init(shader, red, "textures/blankTrafficLight.png");
					(*mapClass.getMapJunction(i, j)).getTrafficLights()[k].InitLights(shader, red, "textures/redTrafficLight.png", "textures/rAndATrafficLight.png",
						"textures/greenTrafficLight.png", "textures/blankTrafficLight.png");
				}
			}
		}
	}

	//junction.SetWidth(15.0f * scale * (2481 / 2481.0f));
	//junction.SetHeight(15.0f * scale);
	//junction.calculateLines();

	for (int i = 0; i < cars.size(); i++) {
		cars[i].Init(shader, red, "textures/car.png");
	}
	//mySquare.Init(shader, red, "textures/car.png");
	//car2.Init(shader, red, "textures/car.png");
	//junction.Init(shader, red, "textures/Xjunction.png");
	//std::cout << "size = " << junction.getTrafficLights().size() << std::endl;
	//for (int i = 0; i < 4; i++) {
	//	junction.getTrafficLights()[i].SetHeight(scale);
	//	junction.getTrafficLights()[i].SetWidth(scale/2);
	//	//std::cout << junction.getTrafficLights()[i].getHeight() << std::endl;
	//	junction.getTrafficLights()[i].Init(shader, red, "textures/blankTrafficLight.png");
	//	junction.getTrafficLights()[i].InitLights(shader, red, "textures/redTrafficLight.png", "textures/rAndATrafficLight.png",
	//		"textures/greenTrafficLight.png", "textures/blankTrafficLight.png");
	//}
	//TrafficLight::initTrafficTex();

	//left
	coordinates[0][0] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].x;
	coordinates[0][1] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].y + ((*mapClass.getMapJunction(0, 0)).getHeight()*7/12);
	coordinates[0][2] = -90.0f;
	//right
	coordinates[1][0] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].x+((*mapClass.getMapJunction(0, 0)).getWidth());
	coordinates[1][1] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].y + ((*mapClass.getMapJunction(0, 0)).getHeight() * 7 / 12);
	coordinates[1][2] = -90.0f;
	//top
	coordinates[2][0] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].x + ((*mapClass.getMapJunction(0, 0)).getWidth() * 7 / 12);
	coordinates[2][1] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].y + ((*mapClass.getMapJunction(0, 0)).getHeight());
	coordinates[2][2] = 90.0f;
	//bottom
	coordinates[3][0] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].x + (((*mapClass.getMapJunction(0, 0)).getWidth() * 5 / 12));
	coordinates[3][1] = (*mapClass.getMapJunction(0, 0)).GetOBB().vertOriginal[0].y;
	coordinates[3][2] = 270.0f;
	//for (int i = 0; i < mapClass.getHeight(); i++) {
	//	for (int j = 0; j < mapClass.getWidth(); j++) {
	//		cout << "i=" << i << ", " << "j=" << j << endl;
	//		mapClass.getMap()[i][j] = emptyJunction;
	//	}
	//}
	Junction middle = mapClass.getMiddle();
	cout << "x = " << middle.GetXPos() << ", y= " << middle.GetYPos() << endl;
	readjustScreen(60, 0, 300, 60);
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
		Car car(glm::mat4(1.0f));
		float red[3] = { 1,0,0 };
		car.SetWidth(scale * (500 / 264.0f));
		car.SetHeight(scale);
		car.Init(shader, red, "textures/car.png");
		car.newSpawn(map[0], 0);
		cars.push_back(car);
		//angle += 0.01f;
		//speed = 0;
		//mySquare.transform(1, 0);
		//mySquare.IncPos(-0.1f, 0.0f);
	}
	if (Right)
	{
		Car car(glm::mat4(1.0f));
		float red[3] = { 1,0,0 };
		car.SetWidth(scale * (500 / 264.0f));
		car.SetHeight(scale);
		car.Init(shader, red, "textures/car.png");
		car.newSpawn(map[0], 1);
		cars.push_back(car);
		//angle -= 0.01f;
		//mySquare.transform(1, 1);
		//mySquare.IncPos(0.1f, 0.0f);
	}
	if (Up)
	{
		Car car(glm::mat4(1.0f));
		float red[3] = { 1,0,0 };
		car.SetWidth(scale * (500 / 264.0f));
		car.SetHeight(scale);
		car.Init(shader, red, "textures/car.png");
		car.newSpawn(map[0], 2);
		cars.push_back(car);
		//speed += 0.03f;
		//mySquare.IncPos(0.0f, 0.1f);
		//mySquare.SetYpos(0.0f);
		//mySquare.SetXpos(0.0f);
		
	}
	if (Down)
	{
		Car car(glm::mat4(1.0f));
		float red[3] = { 1,0,0 };
		car.SetWidth(scale * (500 / 264.0f));
		car.SetHeight(scale);
		car.Init(shader, red, "textures/car.png");
		car.newSpawn(map[0], 3);
		cars.push_back(car);
		//speed -= 0.03f;
		//mySquare.IncPos(0.0f, -0.1f);
		//direction = 1;

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
		cout << " GLEW ERROR" << endl;
	}

	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	//initialise the objects for rendering
	init();

	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	glutSpecialFunc(special);
	glutSpecialUpFunc(specialUp);

	glutIdleFunc(idle);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}

