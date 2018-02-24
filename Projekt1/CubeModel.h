#pragma once

#include "shprogram.h"


class CubeModel
{
	GLfloat vertices[40];
	GLuint indices[36];
	GLuint VAO;
	GLuint VBO;
	std::vector<GLfloat> cube_coord;
	const char * imgName;

public:
	CubeModel(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, const char *);
	CubeModel(const char *);
	~CubeModel();
	//void Draw(const glm::mat4&, GLuint modelLoc) const;
	void drawPrism();
	std::vector<GLfloat> getCoord();
};

