#pragma once

#include "shprogram.h"


class LongCube
{
	GLfloat vertices[40];
	GLuint indices[36];
	GLuint VAO;
	GLuint VBO;
	std::vector<GLfloat> cube_coord;
	const char * imgName;

public:
	LongCube(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, const char *,float);
	LongCube(const char *);
	~LongCube();
	//void Draw(const glm::mat4&, GLuint modelLoc) const;
	void drawPrism();
};

