#include "Sprite.h"
#include "shaders\Shader.h"
#include <string>
#include "ImageLoading.h"

#include <string>

#include <iostream>
#include "TrafficLight.h"



Sprite::Sprite()
{
	m_vaoID = 0;
	m_vboID[0] = 0;
	m_vboID[1] = 0;
	m_Width = 0.0f;
	m_Height = 0.0f;
	m_NumberOfVerts = 0;
	m_xpos = 0;
	m_ypos = 0;
	objectRotation = glm::mat4(1.0f);
}

void Sprite::SetWidth(float size)
{
	m_Width = size;
}

void Sprite::SetHeight(float size)
{
	m_Height = size;
	//std::cout << m_Height << std::endl;
}

void Sprite::SetXpos(float x)
{
	m_xpos = x;
}
void Sprite::SetYpos(float y)
{
	m_ypos = y;
}
void Sprite::setMatrix(glm::mat4 matrix) {
	objectRotation = matrix;
}

glm::mat4 Sprite::getMatrix() {
	return objectRotation;
}

float Sprite::GetXPos()
{
	return m_xpos;
}
float Sprite::GetYPos()
{
	return m_ypos;
}

float Sprite::getWidth() {
	return m_Width;
}

float Sprite::getHeight() {
	return m_Height;
}

void Sprite::IncPos(float x, float y)
{
	m_xpos += x;
	m_ypos += y;
}
glm::mat4 Sprite::transform(float speed, int direction)
{
	if (direction == 0) {
		//std::cout << "noot noot";
		objectRotation = glm::rotate(objectRotation, -0.01f, glm::vec3(0, 1, 0));
	}
	if (direction == 1) {
		//std::cout << "noot noot";
		objectRotation = glm::rotate(objectRotation, 0.01f, glm::vec3(0, 1, 0));
	}
	m_xpos += objectRotation[2][0] * speed;
	m_ypos += objectRotation[2][2] * speed;
	glm::vec3 vector = glm::vec3(m_xpos, m_ypos, 0);
	return glm::translate(glm::mat4(1.0f), vector);
}
void Sprite::Init(Shader& shader, float colour[3], std::string filename)
{
	//load png image
	int imageHeight = 0;
	int imageWidth = 0;
	
	//create the texture on the GPU
	glGenTextures(1, &m_TexName);
	glBindTexture(GL_TEXTURE_2D,m_TexName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  //or use GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	bool success = ImageLoading::loadImage(filename);
	if (!success) {
		std::cout << "Unable to load image file" << std::endl;
		glDeleteTextures(1, &m_TexName);
		return;
	}
	else
	{
		std::cout << "Image loaded " << std::endl;
	}
	

	//Create the geometry
	m_NumberOfVerts = 6;
	float vert[18];	// create a vertex array

	float halfWidth = m_Width / 2.0f;
	float halfHeight = m_Height / 2.0f;

	vert[0] = -halfWidth; vert[1] = halfHeight; vert[2] = 0.0; //x,y,z values for each vertex
	vert[3] = -halfWidth; vert[4] = -halfHeight; vert[5] = 0.0;
	vert[6] = halfWidth; vert[7] = -halfHeight; vert[8] = 0.0;

	vert[9] = -halfWidth; vert[10] = halfHeight; vert[11] = 0.0;
	vert[12] = halfWidth; vert[13] = halfHeight; vert[14] = 0.0;
	vert[15] = halfWidth; vert[16] = -halfHeight; vert[17] = 0.0;

	/********INIT CORNERS FOR OBB***********/

	obb.vertOriginal[0].x = -halfWidth;
	obb.vertOriginal[0].y = -halfHeight;

	obb.vertOriginal[1].x = halfWidth;
	obb.vertOriginal[1].y = -halfHeight;

	obb.vertOriginal[2].x = halfWidth;
	obb.vertOriginal[2].y = halfHeight;

	obb.vertOriginal[3].x = -halfWidth;
	obb.vertOriginal[3].y = halfHeight;

	/*************************************/
	collide.vertOriginal[0].x = -3 * m_Width / 4;
	collide.vertOriginal[0].y = -halfHeight;

	collide.vertOriginal[1].x = halfWidth;
	collide.vertOriginal[1].y = -halfHeight;

	collide.vertOriginal[2].x = halfWidth;
	collide.vertOriginal[2].y = halfHeight;

	collide.vertOriginal[3].x = -3 * m_Width / 4;
	collide.vertOriginal[3].y = halfHeight;
	/*******************/


	float tex[12];
	tex[0] = 0.0f;	 tex[1] = 1.0;
	tex[2] = 0.0f;	 tex[3] = 0.0;
	tex[4] = 1.0f;	 tex[5] = 0.0;

	tex[6] = 0.0f;	 tex[7] = 1.0;
	tex[8] = 1.0f;	 tex[9] = 1.0;
	tex[10] = 1.0f;	 tex[11] = 0.0;


	float col[18];	// colour array
	col[0] = colour[0]; col[1] = colour[1]; col[2] = colour[2]; //r,g,b values for each vertex
	col[3] = colour[0]; col[4] = colour[1]; col[5] = colour[2]; //r,g,b values for each vertex
	col[6] = colour[0]; col[7] = colour[1]; col[8] = colour[2]; //r,g,b values for each vertex
	col[9] = colour[0]; col[10] = colour[1]; col[11] = colour[2]; //r,g,b values for each vertex
	col[12] = colour[0]; col[13] = colour[1]; col[14] = colour[2]; //r,g,b values for each vertex
	col[15] = colour[0]; col[16] = colour[1]; col[17] = colour[2]; //r,g,b values for each vertex

	//VAO allocation
	glGenVertexArrays(1, &m_vaoID);

	// First VAO setup
	glBindVertexArray(m_vaoID);

	glGenBuffers(3, m_vboID); // we need three VBOs - one for the vertices and one for the colours
							//and an extra one for the texture coordinates
							 
	//Lets set up the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);

	//initialises data storage of vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, m_NumberOfVerts * 3 * sizeof(GLfloat), vert, GL_STATIC_DRAW);

	//set the position - linked to the position shader input
	GLint vertexLocation = glGetAttribLocation(shader.handle(), "in_Position");
	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Now set up the colours
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, m_NumberOfVerts * 3 * sizeof(GLfloat), col, GL_STATIC_DRAW);

	//set the colour - linked to the colour shader input.
	GLint colorLocation = glGetAttribLocation(shader.handle(), "in_Color");
	glEnableVertexAttribArray(colorLocation);
	//location in shader, number of componentns,  type, normalised, stride, pointer to first attribute
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Now set up the texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[2]);
	glBufferData(GL_ARRAY_BUFFER, m_NumberOfVerts * 3 * sizeof(GLfloat), tex, GL_STATIC_DRAW);

	//set the texture coords - linked to the texcoord shader input.
	GLint texLocation = glGetAttribLocation(shader.handle(), "in_TexCoord");
	glEnableVertexAttribArray(texLocation);
	//location in shader, number of componentns,  type, normalised, stride, pointer to first attribute
	glVertexAttribPointer(texLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//good practice to bind to 0.
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Sprite::Render(Shader& shader, glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix, glm::mat4& ModelMatrix)
{
	//std::cout << this->getHeight() << std::endl;
	/****UPDATE THE CORNER VALUES BASED ON TRANSFORMATION***/
	//std::cout << m_Height << std::endl;
	//std::cout << "X before = " << obb.vert[0].x << " Y before = " << obb.vert[0].y << std::endl;
	//std::cout << ModelMatrix[3][0] << ", " << ModelMatrix[3][1] << std::endl;
	obb.transformPoints(ModelMatrix);
	//std::cout << "X after = " << obb.vert[0].x << " Y after = " << obb.vert[0].y << std::endl;
	collide.transformPoints(ModelMatrix);
	//m_xpos = obb.xCentre(ModelViewMatrix);
	//m_ypos = obb.yCentre(ModelViewMatrix);
	//glm::vec4 vector = glm::vec4(m_xpos, m_ypos, 0.0f, 1.0f);
	//vector = ModelViewMatrix * vector;
	//std::cout << m_xpos << ", " << m_ypos << std::endl;
	//m_xpos = vector.x;
	//m_ypos = vector.y;
	/*******************************************************/

	glUseProgram(shader.handle());  // use the shader

	//set the DiffuseMap in GLSL to the texture unit 0.
	glUniform1i(glGetUniformLocation(shader.handle(), "DiffuseMap"), 0);

	glBindTexture(GL_TEXTURE_2D, m_TexName);

	//set the uniform for the projectionmatrix
	glUniformMatrix4fv(glGetUniformLocation(shader.handle(), "ProjectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	//pass the uniform for the ModelView matrix to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader.handle(), "ModelViewMatrix"), 1, GL_FALSE, &ModelViewMatrix[0][0]);

	//Draw the object
	glBindVertexArray(m_vaoID);		// select first VAO
	glDrawArrays(GL_TRIANGLES, 0, m_NumberOfVerts);	// draw first object

	glBindVertexArray(0); //unbind the vertex array object
	glUseProgram(0); //turn off the current shader
}

OBB& Sprite::GetOBB()
{
	return obb;
}

bool Sprite::IsInCollision(OBB &anotherOBB)
{
	if (obb.SAT2D(anotherOBB))
	{
		return true;
	}
	return false;
}