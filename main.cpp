#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "obj-model.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "stb_image.h"
#include "Game.h"
#include <stdio.h>
#include "DiceDetection.h"
#include <thread>
#include <map>
#include <string>
#include <iostream>
#include "Gui.h"

#define PI 3.14
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
DiceDetection dd;
std::shared_ptr<GameObject> model;
std::shared_ptr<GameObject> model3;
std::shared_ptr < std::list<std::shared_ptr<GameObject>>> objects = std::make_shared<std::list<std::shared_ptr<GameObject>>>();
ObjModel* model2;

int speed = 20;
bool started = false;
int numPlayers = 0;
Gui* gui;

void init();
void update();
void tempDiceCallback(const std::vector<int>& dice);

std::vector<int> result = {};
    

int main(void) {

    if (!glfwInit())
        throw "Could not initialize glwf";
    window = glfwCreateWindow(1400, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
     
    glfwMakeContextCurrent(window);


    tigl::init();

    gui = new Gui(window);

    init();
    

    while (!glfwWindowShouldClose(window))
    {
        // Update
        gui->update();

        // Draw
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gui->draw();
        

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Terminate GLFW
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

    gui->initGame();

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
    gui->update();
}

void tempDiceCallback(const std::vector<int>& dice) {
    for (int i = 0; i < dice.size() - 1; i++) {
        std::cout << "CALLBACK: value of dice " << i << ": " << dice.at(i) << std::endl;
    }

    gui->updateDice(dice);  // Call the updateDice method of the Gui object
}