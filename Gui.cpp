#include "Gui.h"
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <thread>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "DiceDetection.h"
#include "GameObject.h"
#include "obj-model.h"
#include "stb_image.h"
#include <map>
#include "ObjectManager.h"
#include "tigl.h"

Gui::Gui(GLFWwindow* window)

{
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    this->window = window;
    this->dices = std::vector<int>();
    objects = std::make_shared<std::list<std::shared_ptr<GameObject>>>();
    ObjectManager::ObjectManager(objects);//game, 
}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}


void Gui::updateDice(const std::vector<int>& dice) {
    if (!dice.empty()) {
        this->dices.assign(dice.begin(), dice.end() - 1);
        this->diceStatus = (ResultCode)dice.back();
    }
}

void Gui::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::draw()
{
    if (started) {
        drawGame();
        drawGameOverlay();
    }
    else {
        drawStartOverlay();
    }
}

void Gui::update()
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
    for (auto& object : *objects) {
        object->update(deltaTime);
    }
}

void Gui::drawGame() {
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
    for (auto& object : *objects) {
        object->draw();
    }
}

void Gui::drawStartOverlay() {
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

void Gui::drawGameOverlay() {
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

    //TODO: Add the player with te current turn

    ImGui::SameLine();

    ImGui::Columns(4, "myColumns", true);

    // First column
    ImGui::Text("Turn: ");

    for (int i = 0; i < numPlayers; i++) {
        std::string playerString = "Player ";
        playerString += std::to_string(i);
        playerString += " space x";
        ImGui::Text(playerString.c_str());
    }

    //Second column
    ImGui::NextColumn();


    ImGui::Image((void*)(intptr_t)texture_id, ImVec2(screenWidth * 0.33, screenHeight * 0.2), ImVec2(0, 1), ImVec2(1, 0));

    //Third column
    ImGui::NextColumn();
    ImGui::Text(resultCodeToString[diceStatus]);

    //Fourth column
    ImGui::NextColumn();

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