#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "obj-model.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include <glm/gtc/matrix_transform.hpp>
//#include "Game.h"
#include "DiceDetection.h"
#include <thread>
#include <iostream>

#define PI 3.14

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
DiceDetection dd;
std::shared_ptr<GameObject> model;
std::shared_ptr<GameObject> model3;
std::shared_ptr < std::list<std::shared_ptr<GameObject>>> objects = std::make_shared<std::list<std::shared_ptr<GameObject>>>();
ObjModel* model2;
glm::vec3 camPostion = glm::vec3(3.0f);
glm::vec3 camLookat = glm::vec3(0.0f);
int speed = 20;

void init();
void update();
void draw();
void tempDiceCallback(const std::vector<int>& dice);

std::vector<int> result = {};


int main(void)
{
	// make a new Game object and print the spaces
	//Game game = Game();
 //   std::vector<Space> spaces = game.getSpaces();
 //   // print all spaces
 //   for (Space space : spaces)
 //   {
	//	std::cout << space << std::endl;
	//}


	if (!glfwInit())
		throw "Could not initialize glwf";
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    window = glfwCreateWindow(mode->width, mode->height, "Hello World", monitor, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();

	init();

	while (!glfwWindowShouldClose(window))
	{
		update();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


	return 0;
}


void init()
{
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE) {
				glfwSetWindowShouldClose(window, true);
				dd.stop();
			}

		});
	tigl::shader->enableLighting(true);
	tigl::shader->setLightCount(1);

	tigl::shader->setLightDirectional(0, false);
	tigl::shader->setLightPosition(0, glm::vec3(0, 1000, 0));
	ObjectManager::ObjectManager(objects, "V1.goosegame");//game, 
	void (*callback)(const std::vector<int>&) = tempDiceCallback;
	dd = DiceDetection::DiceDetection();
	static std::thread dice_thread([callback]() {
		dd.startDetectionWrapper(callback);
		});
}
float rotation = 0;
double lastFrameTime = 0;

void update()
{
	double frameTime = glfwGetTime();
	float deltaTime = lastFrameTime - frameTime;
	lastFrameTime = frameTime;
	rotation += 0.1f * deltaTime;

	/*if (glfwGetKey(window, GLFW_KEY_UP)) {
		camLookat.y -= speed / 3 * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN)) {
		camLookat.y += speed / 3 * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT)) {
		camLookat.z -= speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
		camLookat.z += speed * deltaTime;
	}*/
	if (glfwGetKey(window, GLFW_KEY_W)) {
		camPostion.z += speed * deltaTime;
		camLookat.z += speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		camPostion.z -= speed * deltaTime;
		camLookat.z -= speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		camPostion.x -= speed * deltaTime;
		camLookat.x -= speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		camPostion.x += speed * deltaTime;
		camLookat.x += speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_E)) {
		camPostion.y -= speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q)) {
		camPostion.y += speed * deltaTime;
	}
	for (auto& object : *objects) {
		object->update(deltaTime);
	}
}

void draw()
{
	glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 500.0f);

	tigl::shader->setProjectionMatrix(projection);
	tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(camPostion.x, camPostion.y, camPostion.z), camLookat, glm::vec3(0, 1, 0)));
	tigl::shader->setModelMatrix(glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0)));

	tigl::shader->enableColor(true);

	glEnable(GL_DEPTH_TEST);


	glPointSize(10.0f);
	for (auto& object : *objects) {
		object->draw();
	}
}

void tempDiceCallback(const std::vector<int>& dice) {
	for (int i = 0; i < dice.size(); i++) {
		std::cout << "value of dice " << i << ": " << dice.at(i) << std::endl;
	}

}