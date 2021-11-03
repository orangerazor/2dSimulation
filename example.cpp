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
bool Left = false;
bool Right = false;
bool Up = false;
bool Down = false;
int direction = 0;
float speed = 0;
const double PI = 3.141592653589793238463;
float angle = 0;

Shader shader;
Car mySquare = Car::Car(glm::mat4(1.0f));
Car car2 = Car::Car(glm::mat4(1.0f));
Junction junction = Junction::Junction("T", 0, 0, glm::mat4(1.0f), RoadType::T);
Junction crossJunction = Junction::Junction("X", 0, 0, glm::mat4(1.0f), RoadType::X);
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

	ProjectionMatrix = glm::ortho(-25.0, 25.0, -25.0, 25.0); 
}


void display()

{
	//std::cout << "x = " << mySquare.GetXPos() << "y = " << mySquare.GetYPos() << std::endl;
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT);

	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));

	glEnable(GL_BLEND);
	if (secondElapsed >= 1000000) {
		junction.trafficLightFlow();
		//cout << "left" << junction.getTrafficLights()[0].getLights()[0] << junction.getTrafficLights()[0].getLights()[1] << junction.getTrafficLights()[0].getLights()[2] << endl;
		//cout << "right" << junction.getTrafficLights()[1].getLights()[0] << junction.getTrafficLights()[1].getLights()[1] << junction.getTrafficLights()[1].getLights()[2] << endl;
		//cout << "back" << junction.getTrafficLights()[3].getLights()[0] << junction.getTrafficLights()[3].getLights()[1] << junction.getTrafficLights()[3].getLights()[2] << endl;
		secondElapsed = 0;
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
	junction.Render(shader, glm::mat4(1.0f), ProjectionMatrix);
	for (int i = 0; i < 4; i++) {
		glm::mat4 moveLight = glm::mat4(1.0f);
		switch (i) {
		case(0):
			moveLight = glm::translate(moveLight, glm::vec3(junction.getXLeftSquare() - (junction.getTrafficLights()[i].getHeight() / 2), junction.getYTopSquare() + (junction.getTrafficLights()[i].getWidth() / 2), 0.0));
			moveLight = glm::rotate(moveLight, glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
			break;
		case(1):
			moveLight = glm::translate(moveLight, glm::vec3(junction.getXRightSquare()+ (junction.getTrafficLights()[i].getHeight() / 2), junction.getYBotSquare() - (junction.getTrafficLights()[i].getWidth() / 2), 0.0));
			moveLight = glm::rotate(moveLight, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
			break;
		case(2):
			moveLight = glm::translate(moveLight, glm::vec3(junction.getXRightSquare() + (junction.getTrafficLights()[i].getWidth() / 2), junction.getYTopSquare() + (junction.getTrafficLights()[i].getHeight() / 2), 0.0));
			break;
		case(3):
			moveLight = glm::translate(moveLight, glm::vec3(junction.getXLeftSquare() - (junction.getTrafficLights()[i].getWidth() / 2), junction.getYBotSquare() - (junction.getTrafficLights()[i].getHeight() / 2), 0.0));
			moveLight = glm::rotate(moveLight, glm::radians(180.0f), glm::vec3(0.0, 0.0, 1.0));
			break;
		}
		junction.getTrafficLights()[i].Render(shader, moveLight, ProjectionMatrix);
	}
	//std::cout << first.GetXPos() << ", " << first.GetYPos() << std::endl;

	glm::mat4 ModelViewMatrix = glm::mat4(1.0f);

	mySquare.respawn(junction);
	//int entryPoint = mySquare.entryPoint(junction);
	int direction = mySquare.decideDirection(junction, mySquare.getEntryTurning());
	ModelViewMatrix = mySquare.rotate(12.0f / fps, direction, mySquare.getEntryTurning(), junction, fps);
	//ModelViewMatrix = mySquare.faceJunction(entryPoint, ModelViewMatrix);
	//ModelViewMatrix = glm::rotate(ModelViewMatrix, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	//mySquare.respawn(junction);
	mySquare.Render(shader, ModelViewMatrix, ProjectionMatrix);

	//}
	//else {
		//mySquare.SetXpos(0);
		//mySquare.SetYpos(-8);
		//mySquare.setCurrentJunction("");
		//ModelViewMatrix = glm::rotate(ModelViewMatrix, glm::radians(coordinates[0][2]), glm::vec3(0, 0, 1));

		
	//}


	car2.respawn(junction);
	//int entryPoint2 = car2.entryPoint(junction);
	int direction2 = car2.decideDirection(junction, car2.getEntryTurning());
	ModelViewMatrix = car2.rotate(12.0f / fps, direction2, car2.getEntryTurning(), junction, fps);
	//ModelViewMatrix = car2.faceJunction(entryPoint2, ModelViewMatrix);
	//ModelViewMatrix = glm::rotate(ModelViewMatrix, glm::radians(-90.0f), glm::vec3(0, 0, 1));
	//car2.respawn(junction);
	car2.Render(shader, ModelViewMatrix, ProjectionMatrix);
	
	glDisable(GL_BLEND);

	glutSwapBuffers();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	secondElapsed += chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	//cout << (1000.0f/chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) << endl;
	fps = 1000000.0f / chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	//cout << fps<<endl;

	//std::this_thread::sleep_for(std::chrono::milliseconds((1000 / fps) - chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()));

}

void init()
{
	FreeImage_Initialise();

	glClearColor(0.0,0.0,1.0,0.0);						//sets the clear colour to black
	 
	//Load the GLSL program 
	if (!shader.load("Basic", "./glslfiles/basicTexture.vert", "./glslfiles/basicTexture.frag"))
	{
		std::cout << "failed to load shader" << std::endl;
	}

	///This part commented is to scale the width of the sprite to match the dimensions of the car.png image.
	mySquare.SetWidth(4.0f *(500 / 264.0f));
	mySquare.SetHeight(4.0f);
	car2.SetWidth(4.0f * (500 / 264.0f));
	car2.SetHeight(4.0f);

	junction.SetWidth(60.0f * (2481 / 2481.0f));
	junction.SetHeight(60.0f);
	junction.calculateLines();

	float red[3] = { 1,0,0 };
	mySquare.Init(shader, red, "textures/car.png");
	car2.Init(shader, red, "textures/car.png");
	junction.Init(shader, red, "textures/Xjunction.png");
	//std::cout << "size = " << junction.getTrafficLights().size() << std::endl;
	for (int i = 0; i < 4; i++) {
		junction.getTrafficLights()[i].SetHeight(4.0f);
		junction.getTrafficLights()[i].SetWidth(2.0f);
		//std::cout << junction.getTrafficLights()[i].getHeight() << std::endl;
		junction.getTrafficLights()[i].Init(shader, red, "textures/blankTrafficLight.png");
	}
	//TrafficLight::initTrafficTex();

	//left
	coordinates[0][0] = junction.GetOBB().vertOriginal[0].x;
	coordinates[0][1] = junction.GetOBB().vertOriginal[0].y + (junction.getHeight()*7/12);
	coordinates[0][2] = -90.0f;
	//right
	coordinates[1][0] = junction.GetOBB().vertOriginal[0].x+(junction.getWidth());
	coordinates[1][1] = junction.GetOBB().vertOriginal[0].y + (junction.getHeight() * 7 / 12);
	coordinates[1][2] = -90.0f;
	//top
	coordinates[2][0] = junction.GetOBB().vertOriginal[0].x + (junction.getWidth() * 7 / 12);
	coordinates[2][1] = junction.GetOBB().vertOriginal[0].y + (junction.getHeight());
	coordinates[2][2] = 90.0f;
	//bottom
	coordinates[3][0] = junction.GetOBB().vertOriginal[0].x + ((junction.getWidth() * 5 / 12));
	coordinates[3][1] = junction.GetOBB().vertOriginal[0].y;
	coordinates[3][2] = 270.0f;

	//top
	
	//bottom
	//mySquare.SetXpos(junction.GetOBB().vertOriginal[0].x + ((junction.getWidth() * 5 / 12)));
	//mySquare.SetYpos(junction.GetOBB().vertOriginal[0].y);
	//left
	//car2.SetXpos(junction.GetOBB().vertOriginal[0].x + 10);
	//car2.SetYpos(junction.GetOBB().vertOriginal[0].y + (junction.getHeight() * 7 / 12));
	//mySquare.SetXpos(junction.GetOBB().vertOriginal[0].x - (junction.getWidth() * 3 / 8));
	//mySquare.SetYpos(junction.GetOBB().vertOriginal[0].y + (junction.getHeight() * 5/8));
	//mySquare.setMatrix(glm::rotate(mySquare.getMatrix(), glm::radians(90.0f), glm::vec3(0, 1, 0)));
	//right
	//mySquare.SetXpos(junction.GetOBB().vertOriginal[1].x + (junction.getWidth() * 3 / 8));
	//mySquare.SetYpos(junction.GetOBB().vertOriginal[1].y + (junction.getHeight() * 5/8));
	//mySquare.setMatrix(glm::rotate(mySquare.getMatrix(), glm::radians(270.0f), glm::vec3(0, 1, 0)));

	//mySquare.SetXpos(0);
	//mySquare.SetYpos(4);
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
		angle += 0.01f;
		//speed = 0;
		//mySquare.transform(1, 0);
		//mySquare.IncPos(-0.1f, 0.0f);
	}
	if (Right)
	{
		angle -= 0.01f;
		//mySquare.transform(1, 1);
		//mySquare.IncPos(0.1f, 0.0f);
	}
	if (Up)
	{
		speed += 0.03f;
		//mySquare.IncPos(0.0f, 0.1f);
		//mySquare.SetYpos(0.0f);
		//mySquare.SetXpos(0.0f);
		
	}
	if (Down)
	{
		speed -= 0.03f;
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

