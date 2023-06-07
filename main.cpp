#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <stdio.h>
#include <iostream>
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;

void init();
void update();
void draw();

int main(void)
{
    // make a new Game object and print the spaces
    Game game = Game();
    Player player1 = Player(0, "Green", &game);

    std::cout << "PLayer 1 is at: " << player1.getCurrentSpaceIndex() << std::endl;
    for (int i = 0; i < 63; i++) {
        int roll = rand() % 11 + 2;
        std::cout << "Rolled: " << roll << std::endl;
        // roll random number between 2 and 12
        player1.roll(roll);
        std::cout << "PLayer 1 is at: " << player1.getCurrentSpaceIndex() << std::endl;
    }


    std::cout << "PLayer 1 is at: " << player1.getCurrentSpaceIndex() << std::endl;

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
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, true);
    });

}


void update()
{

}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}