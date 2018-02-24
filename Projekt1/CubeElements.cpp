#define GLEW_STATIC
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CubeElements.h"
#include <Soil.h>
using namespace std;

static GLfloat unit_cube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f
};
CubeElements::CubeElements(const char * imgname)
{
	imgName = imgname;
}

CubeElements::CubeElements(GLfloat width, GLfloat height, GLfloat depth, GLfloat xpos, GLfloat ypos, GLfloat zpos, const char * imgname)
{
	imgName = imgname;
	GLfloat v[] = {
		// coordinates								// texture
		0.0f + xpos, 0.0f + ypos, depth + zpos,	0.0f,  0.0f,
		width + xpos, 0.0f + ypos, depth + zpos,	1.0f,  0.0f,
		width + xpos, height + ypos, depth + zpos,	1.0f,  1.0f,
		0.0f + xpos, height + ypos, depth + zpos,	0.0f,  1.0f,
		0.0f + xpos, 0.0f + ypos, 0.0f + zpos,	1.0f,  1.0f,
		width + xpos, 0.0f + ypos, 0.0f + zpos,	0.0f,  1.0f,
		width + xpos, height + ypos, 0.0f + zpos,	0.0f,  0.0f,
		0.0f + xpos, height + ypos, 0.0f + zpos,	1.0f,  0.0f
	};
	std::copy(v, v + 40, vertices);

	GLuint i[] = {
		//front
		0, 1, 2,
		0, 2, 3,
		//right
		1, 5, 6,
		1, 6, 2,
		//top
		2, 3, 7,
		2, 7, 6,
		//left
		4, 0, 3,
		4, 3, 7,
		//bottom
		1, 0, 4,
		1, 4, 5,
		//back
		5, 4, 7,
		5, 7, 6
	};
	std::copy(i, i + 36, indices);

}

//void CubeElements::Draw(const glm::mat4& modelTrans, GLuint modelLoc) const
//{
//	glBindVertexArray(VAO);
//
//	// no internal transformations for now
//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelTrans));
//
//	glDrawArrays(GL_TRIANGLES, 0, (GLuint) cube_coord.size());
//
//	// all is drawn - unbind vertex array
//	glBindVertexArray(0);
//}

void CubeElements::drawPrism() {

	GLuint VBO, EBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // vertices indices
	glBufferData(GL_ARRAY_BUFFER, 40 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), indices, GL_STATIC_DRAW);

	// vertex geometry data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// vertex texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

	glBindVertexArray(0);
	// Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// prepare textures
	int imgWidth, imgHeight;
	unsigned char* image = SOIL_load_image(imgName, &imgWidth, &imgHeight, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
		throw exception("Failed to load texture file");


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	// freeing unnecessary texture stuff
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// all is drawn - unbind vertex array
	glBindVertexArray(0);
}

CubeElements::~CubeElements()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}
