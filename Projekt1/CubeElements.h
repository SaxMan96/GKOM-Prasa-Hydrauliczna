#pragma once

#include "shprogram.h"


class CubeElements
{
	GLfloat vertices[40];
	GLuint indices[36];
	GLuint VAO;
	GLuint VBO;
	std::vector<GLfloat> cube_coord;
	const char * imgName;

public:
	CubeElements(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, const char *);
	CubeElements(const char *);
	~CubeElements();
	//void Draw(const glm::mat4&, GLuint modelLoc) const;
	void drawPrism();
};

