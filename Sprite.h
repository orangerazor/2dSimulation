#pragma once

#include <string>
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "gl\glew.h"

#include "shaders\Shader.h"
#include "OBB.h"

class Sprite
{
private:
	unsigned int m_vaoID;		// id for Vertex Array Object
	unsigned int m_vboID[3];	// ids for Vertex Buffer Objects
	unsigned int m_NumberOfVerts;//number of vertices in the object

	//Dimensions of the sprite.
	float m_Width;
	float m_Height;

	//position of the sprite
	float m_xpos;
	float m_ypos;
	glm::mat4 objectRotation;

	GLuint m_TexName; //identifier for the texture

	OBB obb; //obb object for oriented bounding box collision

public:
	Sprite();					//default constructor
	void Init(Shader& shader, float colour[3], std::string filename);
	void SetWidth(float size);
	void SetHeight(float size);
	void SetXpos(float x);
	void SetYpos(float y);
	void setMatrix(glm::mat4 matrix);
	float GetXPos();
	float GetYPos();
	void IncPos(float x, float y);
	glm::mat4 transform(float speed, int direction);
	void Render(Shader& shader, glm::mat4& ModelViewMatrix, glm::mat4& ProjectionMatrix);
	OBB& GetOBB();
	bool IsInCollision(OBB &anotherOBB);
};