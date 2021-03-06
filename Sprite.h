#pragma once

#include <string>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "gl\glew.h"

#include "shaders\Shader.h"
#include "OBB.h"

class Sprite
{

protected:
	unsigned int m_vaoID;		// id for Vertex Array Object
	unsigned int m_vboID[3];	// ids for Vertex Buffer Objects
	unsigned int m_NumberOfVerts;//number of vertices in the object

	GLuint m_TexName; //identifier for the texture

	OBB obb; //obb object for oriented bounding box collision
	OBB collide;
	OBB spawnOBB;

	//Dimensions of the sprite.
	float m_Width;
	float m_Height;

	//position of the sprite
	float m_xpos;
	float m_ypos;

	glm::mat4 objectRotation;

public:
	Sprite();					//default constructor
	void Init(Shader& shader, float colour[3], std::string filename);
	void SetWidth(float size);
	void SetHeight(float size);
	void SetXpos(float x);
	void SetYpos(float y);
	void setMatrix(glm::mat4 matrix);
	glm::mat4 getMatrix();
	float GetXPos();
	float GetYPos();
	float getWidth();
	float getHeight();
	void IncPos(float x, float y);
	glm::mat4 transform(float speed, int direction);
	virtual void Render(Shader& shader, glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix, glm::mat4& ModelMatrix, int hour);
	OBB& GetOBB();
	OBB& getCollide();
	OBB& getSpawnOBB();
	bool IsInCollision(OBB &anotherOBB);
};