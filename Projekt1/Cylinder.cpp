#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Cylinder.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Cylinder::Cylinder(glm::vec3 color, glm::vec3 center, float radius, float height, unsigned int triangles)
{
	normalVectors = new glm::vec3[triangles * 4 + 2];
	int size = 6;

	this->color = color;
	this->center = center;
	this->radius = radius;
	this->triangles = triangles;

	vertices = new float[size * (2 + 4 * triangles * 2)];
	indices = new unsigned int[4 * triangles * 2 * 3 * 2];

	//vertices of bottom circe
	*(vertices) = center.x;
	*(vertices + 1) = center.y;
	*(vertices + 2) = center.z;

	float tmpX, tmpZ;

	vertices[size] = radius + center.x;
	vertices[size + 1] = center.y;
	vertices[size + 2] = center.z;

	vertices[(triangles)* size + size] = center.x;
	vertices[(triangles)* size + size + 1] = center.y;
	vertices[(triangles)* size + size + 2] = -radius + center.z;

	vertices[(2 * triangles) * size + size] = -radius + center.x;
	vertices[(2 * triangles) * size + size + 1] = center.y;
	vertices[(2 * triangles) * size + size + 2] = center.z;

	vertices[(3 * triangles) * size + size] = center.x;
	vertices[(3 * triangles) * size + size + 1] = center.y;
	vertices[(3 * triangles) * size + size + 2] = radius + center.z;

	for (int i = 1; i < triangles; ++i)
	{
		tmpX = radius*cos(i * 360 / (triangles * 4) * M_PI / 180.0);
		tmpZ = (-1)*radius*sin(i * 360 / (triangles * 4) * M_PI / 180.0);
		// æwiartka I
		*(vertices + size * (i + 1)) = tmpX + center.x;
		*(vertices + size * (i + 1) + 1) = center.y;
		*(vertices + size * (i + 1) + 2) = tmpZ + center.z;
		// æwiartka II
		*(vertices + size * (i)+(triangles)* size + size) = tmpZ + center.x;
		*(vertices + size * (i)+(triangles)* size + size + 1) = center.y;
		*(vertices + size * (i)+(triangles)* size + size + 2) = -tmpX + center.z;
		// æwiartka III 
		*(vertices + size * (i)+(2 * triangles) * size + size) = -tmpX + center.x;
		*(vertices + size * (i)+(2 * triangles) * size + size + 1) = center.y;
		*(vertices + size * (i)+(2 * triangles) * size + size + 2) = -tmpZ + center.z;
		// æwiartka IV
		*(vertices + size * (i)+(3 * triangles) * size + size) = -tmpZ + center.x;
		*(vertices + size * (i)+(3 * triangles) * size + size + 1) = center.y;
		*(vertices + size * (i)+(3 * triangles) * size + size + 2) = tmpX + center.z;
	}

	//vertices of bottom circe

		// INDICES of bottom layer
	unsigned int tmp = 1;
	for (int i = 0; i < triangles * 4 - 1; ++i)
	{
		*(indices + 3 * i) = 0;
		*(indices + 3 * i + 1) = tmp++;
		*(indices + 3 * i + 2) = tmp;
	}

	*(indices + (triangles * 4 - 1) * 3) = 0;
	*(indices + (triangles * 4 - 1) * 3 + 1) = tmp;
	*(indices + (triangles * 4 - 1) * 3 + 2) = 1;

	//--------------------
	unsigned int offside = size * (triangles * 4 + 1);

	// vertices of top layer
	for (int i = 0; i < (4 * triangles); ++i) {
		*(vertices + offside + i*size) = *(vertices + size + i*size);
		*(vertices + offside + i*size + 1) = *(vertices + size + i*size + 1) + height;
		*(vertices + offside + i*size + 2) = *(vertices + size + i*size + 2);
	}
	// vertices of top layer

		// INDECIES

	int tmp1;
	int tmp2;


	// trojkaty boczne 1
	offside = triangles * 4 * 3;

	tmp1 = 1 + 4 * triangles;
	tmp2 = 1;


	for (int i = 0; i < triangles * 4 - 1; ++i)
	{
		*(indices + offside + 3 * i) = tmp1++;
		*(indices + offside + 3 * i + 1) = tmp1;
		*(indices + offside + 3 * i + 2) = tmp2++;
	}

	*(indices + offside + (triangles * 4 - 1) * 3) = tmp1;
	*(indices + offside + (triangles * 4 - 1) * 3 + 1) = 1 + 4 * triangles;
	*(indices + offside + (triangles * 4 - 1) * 3 + 2) = tmp2;

	// trojkaty boczne w druga strone
	tmp1 = 1;
	tmp2 = 1 + 4 * triangles + 1;


	for (int i = 0; i < triangles * 4 - 1; ++i)
	{
		*(indices + offside + triangles * 4 * 3 + 3 * i) = tmp1++;
		*(indices + offside + triangles * 4 * 3 + 3 * i + 1) = tmp1;
		*(indices + offside + triangles * 4 * 3 + 3 * i + 2) = tmp2++;
	}

	*(indices + offside + (triangles * 4 * 2 * 3 - 3)) = tmp1;
	*(indices + offside + (triangles * 4 * 2 * 3 - 3 + 1)) = 1;
	*(indices + offside + (triangles * 4 * 2 * 3 - 3 + 2)) = 1 + 4 * triangles;

	// wierzcholek gora srodek
	*(vertices + size * (2 + 4 * triangles * 2) - 6) = center.x;
	*(vertices + size * (2 + 4 * triangles * 2) - 5) = center.y + height;
	*(vertices + size * (2 + 4 * triangles * 2) - 4) = center.z;

	//trojkaty na gornej plaszczyznie
	tmp = triangles * 4 + 1;
	offside = triangles * 4 * 3 * 3;
	for (int i = 0; i < triangles * 4 - 1; ++i)
	{
		*(indices + offside + 3 * i) = 2 * triangles * 4 + 1;
		*(indices + offside + 3 * i + 1) = tmp++;
		*(indices + offside + 3 * i + 2) = tmp;
	}

	*(indices + offside + (triangles * 4 - 1) * 3) = 2 * triangles * 4 + 1;
	*(indices + offside + (triangles * 4 - 1) * 3 + 1) = tmp;
	*(indices + offside + (triangles * 4 - 1) * 3 + 2) = triangles * 4 + 1;

	//wektory normalne w plaszczyznach
	*(normalVectors) = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
	*(normalVectors + triangles * 4 + 1) = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));

	for (int i = 1; i < triangles * 4 + 1; ++i) {
		*(normalVectors + i) =
			glm::normalize(
				glm::cross(
					glm::vec3(
						*(vertices + (i % (4 * triangles) + 1 + triangles * 4)*size) - *(vertices + (i % (4 * triangles) + 1)*size),
						*(vertices + (i % (4 * triangles) + 1 + triangles * 4)*size + 1) - *(vertices + (i % (4 * triangles) + 1)*size + 1),
						*(vertices + (i % (4 * triangles) + 1 + triangles * 4)*size + 2) - *(vertices + (i % (4 * triangles) + 1)*size + 2)
					),
					glm::vec3(
						*(vertices + (i)*size) - *(vertices + (i % (4 * triangles) + 1)*size),
						*(vertices + (i)*size + 1) - *(vertices + (i % (4 * triangles) + 1)*size + 1),
						*(vertices + (i)*size + 2) - *(vertices + (i % (4 * triangles) + 1)*size + 2)
					)
				));
	}

	//wektory normalne w wierzcholkach
	*(vertices + 3) = (*normalVectors).x;
	*(vertices + 4) = (*normalVectors).y;
	*(vertices + 5) = (*normalVectors).z;

	*(vertices + (triangles * 4 * 2 + 1)*size + 3) = (*(normalVectors + triangles * 4 + 1)).x;
	*(vertices + (triangles * 4 * 2 + 1)*size + 4) = (*(normalVectors + triangles * 4 + 1)).y;
	*(vertices + (triangles * 4 * 2 + 1)*size + 5) = (*(normalVectors + triangles * 4 + 1)).z;

	for (int i = 1; i < triangles * 4 + 1; ++i)
	{
		glm::vec3 tempVec = glm::normalize(*(normalVectors)+*(normalVectors + i) + *(normalVectors + (i + triangles * 4 - 2) % (triangles * 4) + 1));
		*(vertices + i*size + 3) = tempVec.x;
		*(vertices + i*size + 4) = tempVec.y;
		*(vertices + i*size + 5) = tempVec.z;
	}
	for (int i = triangles * 4 + 1; i < triangles * 4 * 2 + 1; ++i)
	{
		int t = triangles * 4;
		glm::vec3 tempVec = glm::normalize(*(normalVectors + t + 1) + *(normalVectors + i - t) + *(normalVectors + (i - 2) % (triangles * 4) + 1));
		*(vertices + i*size + 3) = tempVec.x;
		*(vertices + i*size + 4) = tempVec.y;
		*(vertices + i*size + 5) = tempVec.z;
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * (2 + 4 * triangles * 2), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * 4 * triangles * 2 * 3 * 2, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Cylinder::draw(ShaderProgram &program)
{
	program.Use();
	unsigned int ColorLoc = glGetUniformLocation(program.get_programID(), "Color");
	glUniform3f(ColorLoc, color.x, color.y, color.z);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 4 * triangles * 2 * 3 * 2, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//void Cylinder::draw(ShaderProgram &program, Bulb &bulb)
//{
//	program.Use();
//	unsigned int ColorLoc = glGetUniformLocation(program.get_programID(), "Color");
//	glUniform3f(ColorLoc, color.x, color.y, color.z);
//
//	program.setVec3("lightColor", bulb.getColor());
//	program.setVec3("lightPosition", bulb.getCoordinates());
//
//	glBindVertexArray(VAO);
//	glDrawElements(GL_TRIANGLES, 4 * triangles * 2 * 3 * 2, GL_UNSIGNED_INT, 0);
//	glBindVertexArray(0);
//}

Cylinder::~Cylinder()
{
	delete[]indices;
	delete[]vertices;
	delete[]normalVectors;
}

void Cylinder::shortenCylinder(GLfloat length) {
	int size = 6;

	for (int i = 0; i < (4 * triangles) + 1; ++i) {
		*(vertices + i*size + 1) = *(vertices + i*size + 1) - length;
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * (2 + 4 * triangles * 2), vertices, GL_STATIC_DRAW);

}
