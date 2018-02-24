#pragma once

#include "shprogram.h"


class Belt
{
	GLfloat vertices[40];
	GLuint indices[36];
	GLuint VAO;
	GLuint VBO;
	std::vector<GLfloat> cube_coord;
	const char * imgName;

public:
	Belt(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, const char *,float);
	Belt(const char *);
	~Belt();
	//void Draw(const glm::mat4&, GLuint modelLoc) const;
	void drawPrism();
};

