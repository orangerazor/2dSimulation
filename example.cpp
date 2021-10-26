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
#include <chrono>
#include <thread>


#include <iostream>
using namespace std;
int fps = 30;

glm::mat4 ViewMatrix;  // matrix for the modelling and viewing
glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
int screenWidth = 480, screenHeight = 480;

//booleans to handle when the arrow keys are pressed or released.
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
Junction junction = Junction::Junction("T", true, true, false, true, 0, glm::mat4(1.0f), RoadType::T);


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
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT);

	ViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, 0.0));

	glEnable(GL_BLEND);
	
	junction.Render(shader, glm::mat4(1.0f), ProjectionMatrix);
	//glm::mat4 ModelViewMatrix = mySquare.turn(speed, direction);
	//std::cout << mySquare.GetXPos() << ", " << mySquare.GetYPos() << std::endl;
	//mySquare.IncPos(0, 0.01f);
	/*glm::mat4 modelviewmatrix = mysquare.drive(speed, direction, angle) * viewmatrix;*/

	int entryPoint = mySquare.entryPoint(junction);
	int direction = mySquare.decideDirection(junction, entryPoint);
	glm::mat4 ModelViewMatrix = glm::mat4(1.0f);
	if (mySquare.IsInCollision(junction.GetOBB())) {
		ModelViewMatrix = mySquare.rotate(0.1f, direction, entryPoint, junction);
	}
	else {
		ModelViewMatrix = mySquare.rotate(0.1f, 0, entryPoint, junction);
	}
	//speed += 0.03f;
	/*ModelViewMatrix = glm::rotate(ModelViewMatrix, -1.5708f, glm::vec3(0, 0, 1));*/
	mySquare.Render(shader, ModelViewMatrix, ProjectionMatrix);
	
	glDisable(GL_BLEND);

	glutSwapBuffers();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::this_thread::sleep_for(std::chrono::milliseconds((1000 / fps) - chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()));

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
	mySquare.SetWidth(2.0f *(500 / 264.0f));
	mySquare.SetHeight(2.0f);
	junction.SetWidth(20.0f * (100 / 107.0f));
	junction.SetHeight(20.0f);

	float red[3] = { 1,0,0 };
	mySquare.Init(shader, red, "textures/car.png");
	junction.Init(shader, red, "textures/t-junction.png");
	//top
	//mySquare.SetXpos(junction.GetOBB().vertOriginal[0].x+(junction.getWidth()*3/8));
	//mySquare.SetYpos(junction.GetOBB().vertOriginal[3].y+(junction.getHeight()/2));
	//mySquare.setMatrix(glm::rotate(mySquare.getMatrix(), glm::radians(180.0f), glm::vec3(0, 1, 0)));
	//bottom
	mySquare.SetXpos(junction.GetOBB().vertOriginal[0].x+(junction.getWidth()*3/8));
	mySquare.SetYpos(junction.GetOBB().vertOriginal[0].y-(junction.getHeight()/2));
	//left
	/*mySquare.SetXpos(junction.GetOBB().vertOriginal[0].x - (junction.getWidth() * 3 / 8));
	mySquare.SetYpos(junction.GetOBB().vertOriginal[0].y + (junction.getHeight() * 5/8));
	mySquare.setMatrix(glm::rotate(mySquare.getMatrix(), glm::radians(90.0f), glm::vec3(0, 1, 0)));*/
	//right
	/*mySquare.SetXpos(junction.GetOBB().vertOriginal[1].x + (junction.getWidth() * 3 / 8));
	mySquare.SetYpos(junction.GetOBB().vertOriginal[1].y + (junction.getHeight() * 5/8));
	mySquare.setMatrix(glm::rotate(mySquare.getMatrix(), glm::radians(270.0f), glm::vec3(0, 1, 0)));*/

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
		speed = 0;
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

