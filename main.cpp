#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "obj-model.h"
#include "GameObject.h"
#include "ModelComponent.h"
#include "SpinComponent.h"
#include <glm/gtc/matrix_transform.hpp>
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
std::list<std::shared_ptr<GameObject>> objects;
ObjModel* model2;
glm::vec3 camPostion = glm::vec3(0.0f);
int speed = 20;

void init();
void update();
void draw();
void tempDiceCallback(const std::vector<int>& dice);

std::vector<int> result = {};


int main(void)
{
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
    for (int i = 0; i < 4; i++)
    {
        model = std::make_shared<GameObject>();
        model->position = glm::vec3(0, i, 5);
        model->rotation = glm::vec3(0, PI * i * 0.5, 0);
        model->addComponent(std::make_shared<ModelComponent>("models/Tiles/rail_straight.obj"));
        model->addComponent(std::make_shared<SpinComponent>(0.01));
        objects.push_back(model);
        model = std::make_shared<GameObject>();
        model->position = glm::vec3(0, i, 5);
        model->rotation = glm::vec3(0, PI * i * 0.5, 0);
        model->addComponent(std::make_shared<ModelComponent>("models/Tiles/tile.obj"));
        model->addComponent(std::make_shared<SpinComponent>(0.01));
        objects.push_back(model);
    }
    for (int i = 0; i < 4; i++)
    {
        model = std::make_shared<GameObject>();
        model->position = glm::vec3(4, i, 5);
        model->rotation = glm::vec3(0, PI * i * 0.5, 0);
        model->addComponent(std::make_shared<ModelComponent>("models/Tiles/rail_curved.obj"));
        model->addComponent(std::make_shared<SpinComponent>(0.01));
        objects.push_back(model);
        model = std::make_shared<GameObject>();
        model->position = glm::vec3(4, i, 5);
        model->rotation = glm::vec3(0, PI * i * 0.5, 0);
        model->addComponent(std::make_shared<ModelComponent>("models/Tiles/tile.obj"));
        model->addComponent(std::make_shared<SpinComponent>(0.01));
        objects.push_back(model);
    }
    
    model3 = std::make_shared<GameObject>();
    model3->position = glm::vec3(10, 1, 5);
    model3->addComponent(std::make_shared<ModelComponent>("models/car/honda_jazz.obj"));
    objects.push_back(model3);
    
    model2 = new ObjModel("models/steve/steve.obj");//add filepath here
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

    if (glfwGetKey(window, GLFW_KEY_UP)) {
        camPostion.z += speed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN)) {
        camPostion.z -= speed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT)) {
        camPostion.y -= speed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
        camPostion.y += speed * deltaTime;
    }
    for (auto& object : objects) {
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
    tigl::shader->setViewMatrix(glm::lookAt(glm::vec3(camPostion.x, camPostion.y, camPostion.z), glm::vec3(0, 1, 5), glm::vec3(0, 1, 0)));
    tigl::shader->setModelMatrix(glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0, 1, 0)));

    tigl::shader->enableColor(true);

    glEnable(GL_DEPTH_TEST);


    glPointSize(10.0f);
    for (auto& object : objects) {
        object->draw();
    }
}

void tempDiceCallback(const std::vector<int>& dice) {
    for (int i = 0; i < dice.size(); i++) {
        std::cout << "value of dice " << i << ": " << dice.at(i) << std::endl;
    }

}