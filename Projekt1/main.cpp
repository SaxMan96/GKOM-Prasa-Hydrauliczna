#define GLEW_STATIC
#include <GL/glew.h>
#include "shprogram.h"
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
#include <vector>
using namespace std;
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "CubeModel.h"
#include "LongCube.h"
#include "Belt.h"
#include "Cylinder.h"
#include "Camera.h"


const GLuint WIDTH = 800, HEIGHT = 600;

// camera
Camera camera(glm::vec3(13.0f, 8.0f, 15.0f));

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// key buffer for camera movement
bool keys[1024];

GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

GLfloat craneHeight = 4.0f;
GLfloat maxCraneMovement = 2.0f;
GLfloat minCraneMovement = -4.0f;
GLfloat craneMovement = 0.0f;

GLfloat maxLineMovement = 2.0f;
GLfloat minLineMovement = -4.1f;
GLfloat lineMovement = 0.0f;

GLfloat craneRotation = 0.0f;
GLfloat maxCraneRotation = 180.0f;
GLfloat minCraneRotation = -180.0f;

GLfloat movement = 0.02f;

CubeModel *crane;
CubeModel *crane2;
CubeModel *crane3;
CubeModel *container;
CubeModel *line1;
CubeModel *verticalLine;



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}
float tlokVerticalTranslate = 4.5f;
float tlokVerticalMovement = 0.05f;
float mintlokVerticalTranslate = 3.05f;
float maxtlokVerticalTranslate = 4.5f;

float przesunieciePudelek = 0.05;
float krokPrzesunieciaPudelek = 0.05;
bool machineStopped = true;
bool tlokOpuszczany = false;
bool tlokPodnoszony = false;
bool tlokSplaszcza = false;
vector<float> splaszczenie;
int liczbaPudelek = 7;
int odlegloscPudelka = 0.0f;
int numerSplaszczonegoPudelka = 0;


void do_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 25.0f * deltaTime;
	if (keys[GLFW_KEY_UP])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_DOWN])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_LEFT])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_RIGHT])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_S])
		if (machineStopped && tlokVerticalTranslate - tlokVerticalMovement >= mintlokVerticalTranslate)
			tlokVerticalTranslate -= tlokVerticalMovement;
		
	if (keys[GLFW_KEY_W])
		if (machineStopped && tlokVerticalTranslate + tlokVerticalMovement <= maxtlokVerticalTranslate)
			tlokVerticalTranslate += tlokVerticalMovement;
		
	if (keys[GLFW_KEY_R]) {
		machineStopped = false;
		cout << "Is machine running: RUNNING" << endl;
	}
	if (keys[GLFW_KEY_T]) {
		machineStopped = true;
		cout << "Is machine running: STOP" << endl;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

GLuint LoadMipmapTexture(GLuint texId, const char* fname)
{
	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
		throw exception("Failed to load texture file");

	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(texId);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

int main()
{
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	try
	{
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - OpenGL 06", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");

		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		glViewport(0, 0, WIDTH, HEIGHT);
		glEnable(GL_DEPTH_TEST);

		// Build, compile and link shader program
		ShaderProgram theProgram("gl_06.vert", "gl_06.frag");
		//ShaderProgram theProgram("platformShader.vert", "platformShader.frag");
		//ShaderProgram theProgram("light.vs", "light.fs");

		GLfloat containterLevel = 2.0f;

		// Set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// prepare textures
		//GLuint texture0 = LoadMipmapTexture(GL_TEXTURE0, "iipw.png");
		//GLuint texture1 = LoadMipmapTexture(GL_TEXTURE1, "weiti.png");

		glEnable(GL_DEPTH_TEST); // let's use z-buffer
		CubeModel pomieszczenie(44.0f, 30.0f, 36.0f, -21.0f, 0.0f, -20.0f, "wall.jpg");
		CubeModel podloga(44.0f, 0.01f, 36.0f, -21.0f, 0.0f, -20.0f, "floormetal.jpg");
		CubeModel sufit(44.0f, -0.01f, 36.0f, -21.0f, 30.0f, -20.0f, "floormetal.jpg");

		CubeModel podstawa1(2.0f, 6.0f, 1.0f, 0.0f, 0.0f, 0.0f, "metal.jpg");
		CubeModel podstawa2(2.0f, 6.0f, 1.0f, 0.0f, 0.0f, -5.0f, "metal.jpg");
		CubeModel komora(4.0f, 5.0f, 4.0f, -1.0f, 5.0f, -4.0f, "metal.jpg");

		LongCube podtawaTasmy(44.0f, 3.0f, 4.0f, -21.0f, 0.0f, -4.0f, "conveyorBelt.jpg", 4.0f);
		LongCube tasma(44.0f, 3.05f, 4.0f, -21.0f, 0.0f, -4.0f, "conveyorBelt.jpg", 4.0f);

		for (int i = 0; i < liczbaPudelek; i++)
		{
			splaszczenie.push_back(0.0f);
		}
		// main event loop
		while (!glfwWindowShouldClose(window))
		{
			//cout << "Camera position\tx: " << camera.Position.x << "\ty: " << camera.Position.y << "\tz: " << camera.Position.z << endl;
			//cout << crane2.getHeight() << endl;
			//cout << "Line: " << lineMovement << "Crane: " << craneMovement << endl;
			//cout << "pozycja crane2 : " << crane2->getMovePosition().x<<" "<< crane2->getMovePosition().y<< " " << crane2->getMovePosition().z << endl;
			// Calculate deltatime of current frame
			GLfloat currentFrame = (GLfloat)glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();
			do_movement();

			// Clear the colorbuffer
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Bind Textures using texture units

			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, texture0);
			//glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture0"), 0);
			//glActiveTexture(GL_TEXTURE1);
			//glBindTexture(GL_TEXTURE_2D, texture1);
			//glUniform1i(glGetUniformLocation(theProgram.get_programID(), "Texture1"), 1);

			theProgram.Use();
			GLuint modelLoc = glGetUniformLocation(theProgram.get_programID(), "model");
			GLuint viewLoc = glGetUniformLocation(theProgram.get_programID(), "view");
			GLuint projLoc = glGetUniformLocation(theProgram.get_programID(), "projection");

			// set perspective projection & pass it to GPU
			//glm::mat4 projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 120.0f);
			glm::mat4 view = camera.GetViewMatrix();
			glm::mat4 model;
			model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));


			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

			// set view transformation & pass it to GPU
			//glm::mat4 view;
			//view = glm::lookAt(cameraPos, glm::vec3(0.0, 0.0, 0.0), cameraUp);
			//view = camera.GetViewMatrix();


			glm::mat4 trans;


			CubeModel tlok(1.0f, 4.0f, 1.0f, 1.0f, tlokVerticalTranslate, -2.5f, "metal2.jpg");
			
			vector<CubeModel> pudelka;

			odlegloscPudelka = 0.0f;
			for (int i = 0; i < liczbaPudelek; i++)
			{
				//				  -		+	  -		+     -     -     
				CubeModel element(0.5f, 0.5f-splaszczenie[i], 0.5f, 2.5f + odlegloscPudelka - przesunieciePudelek, 3.05f, -2.25f, "cardboard.jpg");
				pudelka.push_back(element);
				odlegloscPudelka += 3.0f;
			}
			odlegloscPudelka == 3.0f;
			machineStopped = false;
			if (!machineStopped) {
				//Jezeli tlok sie nie rusza oraz jezeli kolejne pudelko nie jest pod tlokiem to przesuwaj
				float polozenie = pudelka[numerSplaszczonegoPudelka].getCoord()[0];
				if (!tlokPodnoszony && !tlokOpuszczany && polozenie >= 1.25f)
					przesunieciePudelek += krokPrzesunieciaPudelek;
				//Je¿eli pudelko jest na miejscu opuszczaj tlok
				else if(!tlokPodnoszony)
					tlokOpuszczany = true;
				//Jezeli tlok jest opuszczany to zmniejszaj ypos tloka
				if (tlokOpuszczany) {
					if (!machineStopped && tlokVerticalTranslate - tlokVerticalMovement >= mintlokVerticalTranslate)
						tlokVerticalTranslate -= tlokVerticalMovement;
				}
				//Jezeli jest podnoszony to zwiekszaj ypos tloka
				else if (tlokPodnoszony) {
					if (!machineStopped && tlokVerticalTranslate + tlokVerticalMovement <= maxtlokVerticalTranslate)
						tlokVerticalTranslate += tlokVerticalMovement;
				}
				//rozpoczecie splaszczania
				if (tlokOpuszczany && tlokVerticalTranslate <= 3.55f) {
					tlokSplaszcza = true;
					splaszczenie[numerSplaszczonegoPudelka] += tlokVerticalMovement;
				}
				//rozpoczynam podnoszenie
				if (tlokOpuszczany && tlokVerticalTranslate <= 3.25f) {
					tlokSplaszcza = false;
					tlokOpuszczany = false;
					numerSplaszczonegoPudelka++;
					tlokPodnoszony = true;
				}
				//zakonczenie podnoszenia
				if (tlokPodnoszony && tlokVerticalTranslate >= 4.5f) {
					tlokPodnoszony = false;
				}
			}
			

			//			   x     y     z     xp    yp    zp

			/*glm::vec3 containerCenter = glm::vec3(1.0f, 9.90f, 2.0f);
			glm::vec3 LineColor = glm::vec3(107, 106, 97);
			Cylinder line(LineColor, containerCenter, 1.5f, 16.5f, 3);
*/
			//CubeModel templateka(.0f, .0f, .0f, .0f, .0f, .0f, "metal.jpg");

			podstawa1.drawPrism();
			podstawa2.drawPrism();
			komora.drawPrism();
			tlok.drawPrism();
			pomieszczenie.drawPrism();
			podloga.drawPrism();
			sufit.drawPrism();
			podtawaTasmy.drawPrism();			
			tasma.drawPrism();
			//cout << "pudelko";
			for (CubeModel pudelko : pudelka) {
				pudelko.drawPrism();
				//vector<GLfloat> f = pudelko.getCoord();
				//cout << "posx: " << f[0] << "posy: " << f[1] << "posz: " << f[2] << endl;
			}
			//cout << "\n\n\n\\n\n\n";
			//line.draw(tlokShader);
			//cube.Draw(trans, modelLoc);

			/*outerCube.draw(theProgram);
			concreteBase.draw(theProgram);
			crane->draw(theProgram);
			container->draw(theProgram);
			crane2->draw(theProgram);
			crane3->draw(theProgram);
			line1->draw(theProgram);*/

			// Swap the screen buffers
			glfwSwapBuffers(window);
		}
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	glfwTerminate();

	//system("PAUSE");
	return 0;
}