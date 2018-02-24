#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

using namespace std;
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include "shprogram.h"
//#include "Bulb.h"

class Cylinder
{
	glm::vec3 color;
	glm::vec3 center;
	unsigned int triangles;
	float radius;

	glm::vec3 *normalVectors;
	float *vertices;
	unsigned int *indices;

	unsigned int size;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

public:
	Cylinder::Cylinder(glm::vec3 color, glm::vec3 center, float radius, float height, unsigned int triangles);
	~Cylinder();
	void draw(ShaderProgram &program);
	//void draw(ShaderProgram &program, Bulb &light);

	void shortenCylinder(GLfloat);
	glm::vec3 getCenter() {
		return center;
	}
};