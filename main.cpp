#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "obj-model.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "stb_image.h"
//#include "imgui_impl_opengl3_loader.h"
//#include "Game.h"
#include "DiceDetection.h"
#include <thread>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>

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
int diceValue = 0;
bool started = false;
int numPlayers = 0;
cv::VideoCapture capture(0);
GLuint cameraTextureId = 0;

void init();
void update();
void draw();
void drawCamera();
void drawGame();
void drawGameOverlay();
void drawStartOverlay();
void tempDiceCallback(const std::vector<int>& dice);
void diceCallBack(int width, int height, unsigned char* imgData, const std::vector<int>& dice);

std::vector<int> result = {};


int main(void)
{


	if (!glfwInit())
		throw "Could not initialize glwf";
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    window = glfwCreateWindow(mode->width, mode->height, "The Goose Game", monitor, NULL);
	if (!window)
	{
		glfwTerminate();
		throw "Could not initialize glwf";
	}
	glfwMakeContextCurrent(window);

	tigl::init();

	init();


    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    while (!glfwWindowShouldClose(window))
    {
        // Update
        update();

        // Draw
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        draw();
        

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
    if (started) {
        drawGame();
        drawGameOverlay();
    }
    else {
        drawStartOverlay();
    }
}

void drawStartOverlay() {
    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //image data
    GLuint texture_id;
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image_data = stbi_load("images/titleScreen.png", &width, &height, &channels, 0);
    if (image_data == nullptr)
    {
        // Error loading the image
        // Handle the error accordingly
        std::cout << "Error loading image\n";
    }

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    int screenWidth, screenHeight;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);
    // Create a simple GUI window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight));
    ImGui::Begin("GUI Window", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    // Add GUI elements here

    // Create a bullet option menu for selecting the number of players
    ImGui::Text("Select the number of players:");
    ImGui::RadioButton("2 players", &numPlayers, 2);
    ImGui::RadioButton("3 players", &numPlayers, 3);
    ImGui::RadioButton("4 players", &numPlayers, 4);
    ImGui::RadioButton("5 players", &numPlayers, 5);
    ImGui::RadioButton("6 players", &numPlayers, 6);

    if (ImGui::Button("Start Game"))
    {
        if (numPlayers > 0) {
            started = true;
        }
        std::cout << "Starting game with " << numPlayers << " players!" << std::endl;
    }
    ImGui::End();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glDeleteTextures(1, &texture_id);
    stbi_image_free(image_data);
}

void drawGameOverlay() {
    int screenWidth, screenHeight;
    glfwGetWindowSize(window, &screenWidth, &screenHeight);

    //image data
    GLuint texture_id;
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image_data = stbi_load("images/titleScreen.png", &width, &height, &channels, 0);
    if (image_data == nullptr)
    {
        // Error loading the image
        // Handle the error accordingly
        std::cout << "Error loading image\n";
    }

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create a simple GUI window
    ImGui::SetNextWindowPos(ImVec2(0, screenHeight * 0.75));
    ImGui::SetNextWindowSize(ImVec2(screenWidth, screenHeight * 0.25));
    ImGui::Begin("Goosegame", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::SameLine();

    ImGui::Columns(3, "myColumns", true);

    // First column
    //TODO: Add the player with the current turn
    ImGui::Text("Turn: ");

    for (int i = 0; i < numPlayers; i++) {
        std::string playerString = "Player ";
        playerString += std::to_string(i);
        playerString += " space x";
        ImGui::Text(playerString.c_str());
    }


    ImGui::NextColumn();

    ImGui::Image((void*)(intptr_t)texture_id, ImVec2(screenWidth * 0.33, screenHeight * 0.2), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::NextColumn();
    // Second column
    if (ImGui::Button("Quit")) {
        started = false;
    }

    ImGui::Columns(1); // Reset to single column layout

    ImGui::End();

    // Render ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glDeleteTextures(1, &texture_id);
    stbi_image_free(image_data);
}

void drawGame() {
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

    drawCamera();
}

void tempDiceCallback(const std::vector<int>& dice) {
	/*for (int i = 0; i < dice.size(); i++) {
		std::cout << "value of dice " << i << ": " << dice.at(i) << std::endl;
	}*/
	if (dice.size() == 2) {
		std::cout << "wow" << std::endl;
		diceValue = dice.at(0) + dice.at(1);
	}
}

void diceCallBack(int width, int height, unsigned char* imgData, const std::vector<int>& dice) {
    diceValue = dice.at(0) + dice.at(1);
}

void drawCamera() {
    cv::Mat frame;
    capture.read(frame);
    if (!frame.empty()) {
        // If the camera is available
        glDeleteTextures(1, &cameraTextureId);

        cv::flip(frame, frame, 0);
        glGenTextures(1, &cameraTextureId);
        glBindTexture(GL_TEXTURE_2D, cameraTextureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Set texture clamping method
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


        glTexImage2D(GL_TEXTURE_2D,     // Type of texture
            0,                 // Pyramid level (for mip-mapping) - 0 is the top level
            GL_RGB,            // Internal colour format to convert to
            frame.cols,          // Image width  i.e. 640 for Kinect in standard mode
            frame.rows,          // Image height i.e. 480 for Kinect in standard mode
            0,                 // Border width in pixels (can either be 1 or 0)
            GL_BGR, // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
            GL_UNSIGNED_BYTE,  // Image data type
            frame.ptr());        // The actual image data itself

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    if (cameraTextureId == 0)
    {
        return;
    }

    // Unbind and rebind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindTexture(GL_TEXTURE_2D, cameraTextureId);

    // Render a quad with the texture
    tigl::shader->enableTexture(true);
    tigl::shader->setColorMult(glm::vec4(1, 1, 0, 1));
    tigl::begin(GL_QUADS);
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(0, 1, 0), glm::vec2(0, 0)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, 1, 0), glm::vec2(1, 0)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(1, 1, 1), glm::vec2(1, 1)));
    tigl::addVertex(tigl::Vertex::PT(glm::vec3(0, 1, 1), glm::vec2(0, 1)));
    tigl::end();
    tigl::shader->enableTexture(false);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}